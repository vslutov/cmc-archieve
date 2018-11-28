from pyspark.sql import SparkSession
import pyspark.sql.functions as f

spark = (SparkSession.builder.appName("Solution")
         .getOrCreate())

df_result = spark.read.parquet('parquet_result/result.parquet').orderBy(f.desc('corr'))
for line in df_result.collect():
    print(line.sec1, line.sec2, line.width, line.shift, line.corr)