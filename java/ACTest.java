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
      ArithmeticCoderFLW AC = new ArithmeticCoderFLW();

      System.out.println("prob0ToFLW + FLWToProb0 Test");
      for (int i = 0; i < 10; i++) {
        float prob0 = probSet[i];
        System.out.print("-----------\nProbability: " + prob0 + "\n");

        for (int j = 0; j < 10; j++) {
          int precisionBits = precisionSet[j];
          int expectedFLW = AC.prob0ToFLW(prob0, precisionBits);
          float expectedProb = AC.FLWToProb0(expectedFLW, precisionBits);
          int realFLW = AC.prob0ToFLW(expectedProb, precisionBits);
          float realProb = AC.FLWToProb0(realFLW, precisionBits);

          System.out.print("Bits: " + precisionBits
                           + " -> " + expectedFLW
                           + " -> " + expectedProb
                           + " -> " + realFLW
                           + " -> " + realProb + "\n");
        }
        System.out.println("---------------------------------------");
      }
    }
}
