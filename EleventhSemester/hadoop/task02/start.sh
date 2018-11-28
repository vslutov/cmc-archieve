#!/usr/bin/env bash

for i in {5..80..5}
do
  hdfs dfs -rm -r output.csv
  /usr/bin/time -o "result.$i.txt" -f '%E' spark-submit correlation.py "wasb://rts@bigdatasets.blob.core.windows.net/deals" output.csv --securities='(SVH1)|(EDH1)' --date_from=20110111 --date_to=20110112 --time_from=1000 --time_to=1800 --widths=1,5,10 --shifts=0,1,2,3,4,5 --num_reducers="$i"
done
