# -*- coding: utf-8 -*-
import json
import pyspark.sql.functions as f
from pyspark.sql import SparkSession
import argparse

class DecisionTreeClassifier:
    def __init__(self, input_columns=None, target_column=None, max_depth=3, min_samples_leaf=1, max_bins=10):
        self.input_columns = input_columns
        self.target_column = target_column
        self.max_depth = max_depth
        self.min_samples_leaf = min_samples_leaf
        self.max_bins = max_bins

        self.tree = {}

    def fit(self, df):
        self.tree['percentiles'] = {}

        df = df.cache()
        df_size = df.count()

        minmax = df.select(
            *([f.min(attr).alias('min_' + attr) for attr in self.input_columns] +
              [f.max(attr).alias('max_' + attr) for attr in self.input_columns]
            )
        ).first()

        df = df.select(
            self.target_column,
            *(((f.col(attr) - minmax['min_' + attr]) / (minmax['max_' + attr] - minmax['min_' + attr]) * self.max_bins).cast('int').alias(attr)
              for attr in self.input_columns)
        )


        spark = (SparkSession.builder.appName("Solution")
                 .getOrCreate())

        df = spark.createDataFrame(
            df.rdd.map(lambda row: [row[0]] + [min(x, self.max_bins - 1) for x in row[1:]]),
            df.schema
        ).cache()

        self.tree['percentiles'] = {
            attr: [minmax['min_' + attr] + i * (minmax['max_' + attr] - minmax['min_' + attr]) / self.max_bins for i in range(self.max_bins)]
            for attr in self.input_columns
        }

        self.init_node(self.tree, 0, df)

        del self.tree['percentiles']

        return self

    def init_node(self, node, depth, part):
        n = part.count()
        best_gain = float('-inf')
        best_attr = None
        best_cutoff = None
        best_cutoff_index = None

        if depth < self.max_depth and n > self.min_samples_leaf:

            for attr in self.input_columns:
                stats = part.groupBy(attr).agg(
                    f.count('*').alias('count'),
                    f.avg(self.target_column).alias('avg')
                )

                stats = sorted(tuple(x) for x in stats.collect())
                cutoff = [x[0] for x in stats]
                count = [x[1] for x in stats]
                weight = [x[2] for x in stats]

                for i in range(1, len(stats)):
                    n_l = sum(count[:i])
                    n_r = sum(count[i:])
                    if n_l < self.min_samples_leaf or n_r < self.min_samples_leaf:
                        continue

                    p_l = sum(a * b for a, b in zip(count[:i], weight[:i])) / n_l
                    p_r = sum(a * b for a, b in zip(count[i:], weight[i:])) / n_r

                    q_l = 1 - p_l
                    q_r = 1 - p_r

                    gain = -(1 - p_l ** 2 - q_l ** 2) * n_l / n - (1 - p_r ** 2 - q_r ** 2) * n_r / n

                    if gain > best_gain:
                        best_gain  = gain
                        best_attr = attr
                        best_cutoff = self.tree['percentiles'][attr][cutoff[i]]
                        best_cutoff_index = cutoff[i]

        if best_attr is not None:
            node['attr'], node['cutoff'] = best_attr, best_cutoff
            node['left'], node['right'] = {}, {}

            left_part = part.filter(part[best_attr] < best_cutoff_index)
            self.init_node(node['left'], depth + 1, left_part)
            right_part = part.filter(part[best_attr] >= best_cutoff_index)
            self.init_node(node['right'], depth + 1, right_part)
        else:
            node['weight'] = part.groupBy().avg(self.target_column).collect()[0][0]

    def predict_proba(self, df):
        return df.rdd.map(lambda row: predict_node(self.tree, row))

    def save(self, filename, spark):
        spark.sparkContext.parallelize((json.dumps(self.tree), )).saveAsTextFile(filename)
        return self

    def load(self, filename, spark):
        self.tree = json.loads(spark.sparkContext.textFile(filename).first())
        return self

def predict_node(node, row):
    if 'attr' in node:
        choice = row[node['attr']] >= node['cutoff']
        next_node = node['right'] if choice else node['left']
        return predict_node(next_node, row)
    else:
        return (1 - node['weight'], node['weight'])

def parse_list(l):
    return l.split(',')

def ftrain(spark, args):
    train_data = spark.read.csv(args.train_data, header=True)
    train_data = train_data.select(
        train_data[args.target_column].cast('int'),
        *(train_data[col].cast('float') for col in args.input_columns)
    )

    clf = DecisionTreeClassifier(input_columns=args.input_columns,
                                 target_column=args.target_column,
                                 max_depth=args.max_depth,
                                 min_samples_leaf=args.min_samples_leaf,
                                 max_bins=args.max_bins
                                )
    clf.fit(train_data)
    clf.save(args.tree_filename, spark)
    return clf

def fpredict(spark, args):
    test_data = spark.read.csv(args.test_data, header=True)
    test_data = test_data.select(
        *(test_data[col].cast('float') for col in test_data.columns)
    )

    clf = DecisionTreeClassifier().load(args.tree_filename, spark)
    proba = clf.predict_proba(test_data)
    proba.map(lambda x: (','.join(str(c) for c in x) )).saveAsTextFile(args.predict_filename)
    return proba

def main(params=None):
    parser = argparse.ArgumentParser('Correlation calculator')
    subparsers = parser.add_subparsers(help='train or predict')

    train = subparsers.add_parser('train')
    train.add_argument('train_data')
    train.add_argument('input_columns', type=parse_list)
    train.add_argument('target_column')
    train.add_argument('max_depth', type=int)
    train.add_argument('min_samples_leaf', type=int)
    train.add_argument('max_bins', type=int)
    train.add_argument('tree_filename')
    train.set_defaults(func=ftrain)

    predict = subparsers.add_parser('predict')
    predict.add_argument('test_data')
    predict.add_argument('tree_filename')
    predict.add_argument('predict_filename')
    predict.set_defaults(func=fpredict)

    if params is not None:
        args = parser.parse_args(params.split())
    else:
        args = parser.parse_args()

    spark = (SparkSession.builder.appName("Solution")
                .getOrCreate())

    return args.func(spark, args)

if __name__ == '__main__':
    main()
