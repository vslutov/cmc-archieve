#!/usr/bin/env bash
mvn clean package && ../../bin/hadoop jar target/mm-1.0-SNAPSHOT.jar mm -conf config.xml /user/vs/mm/A /user/vs/mm/B /user/vs/mm/C && ../../bin/hdfs dfs -cat "/user/vs/mm/C/data/*" | less
../../bin/hdfs dfs -rm -r /user/vs/mm/C
