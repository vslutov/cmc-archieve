import java.io.IOException;
import java.util.StringTokenizer;
import java.util.Map;
import java.util.HashMap;
import java.io.OutputStreamWriter;
import java.io.BufferedWriter;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.TextInputFormat;
import org.apache.hadoop.mapreduce.lib.output.TextOutputFormat;
import org.apache.hadoop.util.GenericOptionsParser;
import org.apache.hadoop.fs.FileSystem;
import org.apache.hadoop.fs.FSDataInputStream;
import org.apache.hadoop.fs.FSDataOutputStream;

public class mm {
  public static class LineMapper
       extends Mapper<Object, Text, Text, Text>{

    private final static Text result = new Text();
    private Text reduceKey = new Text();

    public void map(Object __, Text value, Context context
                    ) throws IOException, InterruptedException {
      Configuration conf = context.getConfiguration();

      String[] itr = value.toString().split("\t");
      String tag = itr[0];
      String iString = itr[1];
      int i = Integer.parseInt(iString);
      String jString = itr[2];
      int j = Integer.parseInt(jString);
      String val = itr[3];

      if (conf.get("mm.a").equals(tag)) {
        tag = "a";
      } else {
        tag = "b";
      }

      int groups = conf.getInt("mm.groups", 1);

      if (tag.equals("a")) {
        int hSize = conf.getInt("mm.hSize", 1);

        for (int k = 0; k < groups; ++ k) {
          reduceKey.set(String.format("%d\t%d", i / hSize, k));
          result.set(String.format("%s\t%s\t%s\t%s", tag, iString, jString, val));
          context.write(reduceKey, result);
        }
      } else {
        int wSize = conf.getInt("mm.wSize", 1);

        for (int k = 0; k < groups; ++ k) {
          reduceKey.set(String.format("%d\t%d", k, j / wSize));
          result.set(String.format("%s\t%s\t%s\t%s", tag, iString, jString, val));
          context.write(reduceKey, result);
        }
      }
    }
  }

  public static class MultiplyReducer
       extends Reducer<Text,Text,Text,Text> {
    private Text result = new Text();
    private Text outputKey = new Text();

    public void reduce(Text key, Iterable<Text> values,
                       Context context
                       ) throws IOException, InterruptedException {
      Configuration conf = context.getConfiguration();

      int hSize = conf.getInt("mm.hSize", 1);
      int wSize = conf.getInt("mm.wSize", 1);

      int aHeight = conf.getInt("mm.a.height", 1);
      int aWidth = conf.getInt("mm.a.width", 1);

      int bHeight = conf.getInt("mm.b.height", 1);
      int bWidth = conf.getInt("mm.b.width", 1);

      HashMap<Integer, HashMap<Integer, Float>> aBlock = new HashMap<Integer, HashMap<Integer, Float>>();
      HashMap<Integer, HashMap<Integer, Float>> bBlock = new HashMap<Integer, HashMap<Integer, Float>>();

      String[] itr = key.toString().split("\t");
      int hStep = Integer.parseInt(itr[0]);
      int wStep = Integer.parseInt(itr[1]);

      String outputTag = conf.get("mm.c");
      String format = conf.get("mm.float-format", "%.3f");

      for (Text val : values) {
        itr = val.toString().split("\t");
        String tag = itr[0];
        int i = Integer.parseInt(itr[1]);
        int j = Integer.parseInt(itr[2]);
        float elem = Float.parseFloat(itr[3]);
        if (tag.equals("a")) {
          i = i % hSize;
          if (!aBlock.containsKey(i)) {
            aBlock.put(i, new HashMap<Integer, Float>());
          }
          aBlock.get(i).put(j, elem);
        } else {
          j = j % wSize;
          if (!bBlock.containsKey(j)) {
            bBlock.put(j, new HashMap<Integer, Float>());
          }
          bBlock.get(j).put(i, elem);
        }
      }

      for (Map.Entry<Integer, HashMap<Integer, Float>> aPair : aBlock.entrySet()) {
        int i = aPair.getKey();
        Map<Integer, Float> aRow = aPair.getValue();
        for (Map.Entry<Integer, HashMap<Integer, Float>> bPair : bBlock.entrySet()) {
          int j = bPair.getKey();
          Map<Integer, Float> bCol = bPair.getValue();

          int iResult = hStep * hSize + i;
          int jResult = wStep * wSize + j;

          if (iResult < aHeight && jResult < bWidth) {
            float cValue = 0;
            Boolean cSet = false;
            for(Map.Entry<Integer, Float> aElem : aRow.entrySet()) {
              int k = aElem.getKey();
              if (bCol.containsKey(k)) {
                float aValue = aElem.getValue();
                float bValue = bCol.get(k);

                cValue += aValue * bValue;
                cSet = true;
              }
            }

            if (cSet) {
              outputKey.set(String.format("%s\t%d\t%d", outputTag, iResult, jResult));
              result.set(String.format(format, cValue));
              context.write(outputKey, result);
            }
          }
        }
      }
    }
  }

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    String[] abc = new GenericOptionsParser(conf, args).getRemainingArgs();

    if (abc.length < 3) {
        System.err.println("usage: hadoop jar mm.jar mm -conf config.xml <path to matrix A> <path to matrix B> <path to matrix C>");
        System.exit(1);
    }

    Path path = new Path(abc[0] + "/size");
    FileSystem fs = path.getFileSystem(conf);
    FSDataInputStream stream = fs.open(path);
    String[] line = stream.readLine().split("\t");
    stream.close();
    int N = Integer.parseInt(line[0]);
    int M = Integer.parseInt(line[1]);

    conf.setInt("mm.a.height", N);
    conf.setInt("mm.a.width", M);

    path = new Path(abc[1] + "/size");
    stream = fs.open(path);
    line = stream.readLine().split("\t");
    stream.close();
    M = Integer.parseInt(line[0]);
    int K = Integer.parseInt(line[1]);

    conf.setInt("mm.b.height", M);
    conf.setInt("mm.b.width", K);

    int groups = conf.getInt("mm.groups", 1);
    conf.setInt("mm.hSize", (N + groups - 1) / groups);
    conf.setInt("mm.wSize", (M + groups - 1) / groups);

    path = new Path(abc[2] + "/size");
    BufferedWriter fout = new BufferedWriter(new
      OutputStreamWriter(path.getFileSystem(conf).create(path, true)));
    fout.write(String.format("%d\t%d\n", N, K));
    fout.close();


    String tags = conf.get("mm.tags", "ABC");
    conf.set("mm.a", tags.substring(0, 1));
    conf.set("mm.b", tags.substring(1, 2));
    conf.set("mm.c", tags.substring(2, 3));

    Job job = Job.getInstance(conf, "multiply matrices");
    job.setJarByClass(mm.class);
    job.setMapperClass(LineMapper.class);
    job.setReducerClass(MultiplyReducer.class);

    job.setMapOutputKeyClass(Text.class);
    job.setMapOutputValueClass(Text.class);
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(Text.class);

    TextInputFormat.addInputPath(job, new Path(abc[0] + "/data"));
    TextInputFormat.addInputPath(job, new Path(abc[1] + "/data"));
    TextOutputFormat.setOutputPath(job, new Path(abc[2] + "/data"));

    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }
}
