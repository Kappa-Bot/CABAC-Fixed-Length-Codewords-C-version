import streams.ByteStream;
import java.io.FileInputStream;
import java.io.IOException;

public class ACTest {
    static int filesize = 16 * 1024;
    static int count = 1000;

    static String dir = "../files/";
    static String ext = ".txt";

    static float[] probSet = {-0.5f, 0.0f, 0.01f,
                              0.25f, 0.33f, 0.5f, 0.66f,
                              0.99f, 1.0f, 1.5f};
    static int[] precisionSet = {-1, 0, 1, 2, 3, 8, 17, 64, 512, 2048};


    public static void main(String[] args) throws IOException {
      System.out.println("Initializing ArithmeticCoderFLW");
      StreamGenerator fileGen = new StreamGenerator(dir, ext);
      fileGen.generate(512, 1);
      FileInputStream fis = new FileInputStream(dir + "file_0" + ext);
      ArithmeticCoderFLW AC = new ArithmeticCoderFLW();
      AC.changeStream(new ByteStream(fis.getChannel()));
      AC.restartEncoding();

      // TODO: test every function behaviour


      System.out.println("prob0ToFLW + FLWToProb0 Test");
      for (int i = 0; i < 10; i++) {
        float prob0 = probSet[i];
        System.out.print("-----------\nProbability: " + prob0 + "\n");

        for (int j = 0; j < 10; j++) {
          int precisionBits = precisionSet[j];
          System.out.print("Bits: " + precisionBits);

          int expectedFLW = AC.prob0ToFLW(prob0, precisionBits);
          System.out.print(" -> " + expectedFLW);

          float expectedProb = AC.FLWToProb0(expectedFLW, precisionBits);
          System.out.print(" -> " + expectedProb);

          int realFLW = AC.prob0ToFLW(expectedProb, precisionBits);
          System.out.print(" -> " + realFLW);

          float realProb = AC.FLWToProb0(realFLW, precisionBits);
          System.out.println(" -> " + realProb);
        }
        System.out.println("---------------------------------------");
      }

      System.out.println("File Stream Test");
      fileGen.generate(filesize, count);
      for (int i = 0; i < count; i++) {
        // TODO: test some stuff for the AC using its BS
        // FileInputStream fis = new FileInputStream(dir + "file_" + i + ext);
      }

      System.out.println("Seems like this has finished.");
    }
}
