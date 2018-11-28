from pyspark.sql import SparkSession, Window, DataFrame
import pyspark.sql.functions as f
from pyspark.sql.types import LongType, StringType, IntegerType, DoubleType, StructType, StructField
import argparse
from functools import reduce

def parse_list(l):
    return [int(v) for v in l.split(',')]

def to_str(line):
    return ' '.join([line[0], line[1], str(line[2]), str(line[3]), str(line[4])])

def parse_time(timestr):
    return (timestr // 100) * 60 + (timestr % 100)

def int_time(timeint):
    return ((timeint // 60) * 100 + timeint % 60) * 10 ** 5

def time_to_milliseconds(timenum):
    return parse_time(timenum) * 60 * 1000

def main():
    parser = argparse.ArgumentParser('Correlation calculator')
    parser.add_argument('input')
    parser.add_argument('output')
    parser.add_argument('--securities', type=str, default='(SVH1)|(EDH1)')
    parser.add_argument('--date_from', type=int, default=20110111)
    parser.add_argument('--date_to', type=int, default=20110112)
    parser.add_argument('--time_from', type=int, default=1000)
    parser.add_argument('--time_to', type=int, default=1800)
    parser.add_argument('--widths', type=parse_list, default=[1,5,10])
    parser.add_argument('--shifts', type=parse_list, default=[0,1,2,3,4,5])
    parser.add_argument('--num_reducers', type=int, default=40)

    args = parser.parse_args()

    spark = (SparkSession.builder.appName("Solution")
             .config("spark.sql.shuffle.partitions", str(args.num_reducers))
             .config("spark.default.parallelism", str(args.num_reducers))
             .getOrCreate())

    delta = max(args.shifts) * max(args.widths)

    delta_time_from = int_time(parse_time(args.time_from) - delta)
    delta_time_to = int_time(parse_time(args.time_to) + delta)

    date = (f.col('MOMENT') / 10 ** 9).cast(LongType())
    time = (f.col('MOMENT') % 10 ** 9).cast(LongType())

    dataset = spark.read.load(
        args.input, inferSchema="true", header="true", format="csv"
    ).filter(
        f.col('#SYMBOL').rlike(args.securities) &
        (args.date_from <= date) &
        (date < args.date_to) &
        (delta_time_from <= time) &
        (time < delta_time_to)
    ).select(
        'ID_DEAL',
        'PRICE_DEAL',
        f.col('#SYMBOL').alias('instrument'),
        ((time / 10 ** 7).cast(LongType()) * (60 * 60 * 1000) +
         ((time / 10 ** 5).cast(LongType()) % 100) * (60 * 1000) +
         (time % 10 ** 5)
        ).alias('milliseconds')
    )

    instruments = sorted(x.instrument for x in dataset.select('instrument').distinct().collect())
    shifts = sorted(set(args.shifts + [-s for s in args.shifts]))
    values_list = []

    for width in args.widths:
        width *= 10 ** 3
        from_candle_id = time_to_milliseconds(args.time_from) // width
        to_candle_id = time_to_milliseconds(args.time_to) // width
        candle_id = (f.col('milliseconds') / width).cast(LongType())
        candle_window = Window.partitionBy('instrument', candle_id).orderBy(f.desc('milliseconds'), f.desc('ID_DEAL')) # based on: https://stackoverflow.com/questions/1313120/retrieving-the-last-record-in-each-group-mysql#1313293

        df = dataset.select(
            'instrument',
            'ID_DEAL',
            'PRICE_DEAL',
            'milliseconds',
            candle_id.alias('candle_id'),
            f.row_number().over(candle_window).alias('inside_id')
        ).filter(
            f.col('inside_id') == 1
        )

        candle_map = {
            instrument: df.filter(f.col('instrument') == instrument).select(
                'PRICE_DEAL',
                'candle_id'
            )
            for instrument in instruments
        }

        for i, sec1 in enumerate(instruments):
            df1 = (candle_map[sec1]
                .filter(((from_candle_id <= f.col('candle_id')) & (f.col('candle_id') < to_candle_id)))
                .withColumnRenamed('PRICE_DEAL', 'left_price')
                .withColumnRenamed('candle_id', 'left_candle_id')
            )

            for sec2 in instruments[i + 1:]:
                for shift in shifts:
                    df2 = (candle_map[sec2]
                           .withColumnRenamed('PRICE_DEAL', 'right_price')
                           .withColumnRenamed('candle_id', 'right_candle_id')
                          )

                    joined = df1.join(df2, df1.left_candle_id == df2.right_candle_id + shift, 'inner')

                    values_list.append(joined
                                       .withColumn('sec1', f.lit(sec1))
                                       .withColumn('sec2', f.lit(sec2))
                                       .withColumn('width', f.lit(width // 10 ** 3))
                                       .withColumn('sshift', f.lit(shift))
                                      )

    price_window = Window.partitionBy('sec1', 'sec2', 'width', 'sshift').orderBy('left_candle_id')
    prev_left_price = f.lag('left_price').over(price_window)
    prev_right_price = f.lag('right_price').over(price_window)

    df_result = reduce(DataFrame.union, values_list).select(
        'sec1',
        'sec2',
        'width',
        'sshift',
        ((f.col('left_price') - prev_left_price) / prev_left_price).alias('left_coef'),
        ((f.col('right_price') - prev_right_price) / prev_right_price).alias('right_coef')
    ).groupBy(
        'sec1', 'sec2', 'width', 'sshift'
    ).agg(
        f.corr('left_coef', 'right_coef').alias('corr')
    ).select(
        'sec1', 'sec2', 'width',
        f.abs(f.col('sshift')).alias('shift'),
        'corr'
    )

    df_result.rdd.sortBy(lambda x: -abs(x[4]), numPartitions=1).map(to_str).saveAsTextFile(args.output)
    df_result.write.mode('overwrite').parquet('parquet_result/result.parquet')

if __name__ == "__main__":
    main()
