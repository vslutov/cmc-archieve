bin/hdfs dfs -mkdir /user
bin/hdfs dfs -mkdir /user/vs
bin/hdfs dfs -mkdir /user/vs/mm
bin/hdfs dfs -put B /user/vs/mm
bin/hdfs dfs -put A /user/vs/mm
./start.sh
