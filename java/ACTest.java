import java.io.FileInputStream;
import java.io.IOException;
import java.nio.ByteBuffer;

public class ACTest {
    static final String H1 = "**************************************************************************";
    static final String H2 = "__________________________________________________________________________";

    static float[] probSet = { 0.01f, 0.25f, 0.33f, 0.5f, 0.66f, 0.99f };
    static int[] precisionSet = { 2, 3, 8, 15, 17, 24 };


    public static int pack4B(byte b1, byte b2, byte b3, byte b4) {
      // IMPORTANT: BigEndian by default
      ByteBuffer dbuf = ByteBuffer.allocate(4);
      dbuf.put(b4);
      dbuf.put(b3);
      dbuf.put(b2);
      dbuf.put(b1);
      return dbuf.getInt(0);
    }

    public static void main(String[] args) throws IOException, Exception {
     System.out.printf("%s\n", H1);
     System.out.printf("Initializing ArithmeticCoderFLW\n");
     System.out.printf("%s\n", H1);

      // Basically default initialization, but... You know... Better.
      ArithmeticCoderFLW ACFLW = new ArithmeticCoderFLW(32, 15, 1);
      ByteStream BS = new ByteStream();
      ACFLW.changeStream(BS);

     System.out.printf("%s\n", H1);
     System.out.printf("%41s\n", "prob0ToFLW + FLWToProb0 Test");
      for (int i = 0; i < 6; i++) {
        float prob0 = probSet[i];
       System.out.printf("%s\n", H2);
       System.out.printf("Probability: %f\n", prob0);

        for (int j = 0; j < 6; j++) {
          int pBits = precisionSet[j];
          int xpFLW = ACFLW.prob0ToFLW(prob0, pBits);
          float xpProb = ACFLW.FLWToProb0(xpFLW, pBits);

         System.out.printf("Bits: %-16d FLW: %-10d \t<->\t Prob: %f\n",
                pBits, xpFLW, xpProb);
        }
      }
     System.out.printf("%s\n", H2);
     System.out.printf("%s\n", H1);

      /* **************************************************** */
     System.out.printf("%42s\n", "encodeBitProb Test");
     System.out.printf("%s\n", H2);

     System.out.printf("Encoding 0|1 (0.25|0.33|0.50|0.66)\n");
      for (int i = 1; i < 5; ++i) {
        int prob0 = ACFLW.prob0ToFLW(probSet[i], 15);
        for (int j = 0; j < 8; ++j) {
          ACFLW.encodeBitProb(j % 2 == 0, prob0);
        }
      }

     System.out.printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n",
          BS.getByteStream()[(int) BS.getLength() - 4], BS.getByteStream()[(int) BS.getLength() - 3],
          BS.getByteStream()[(int) BS.getLength() - 2], BS.getByteStream()[(int) BS.getLength() - 1]);
     System.out.printf("%s\n", H2);
     System.out.printf("%s\n", H1);
      /* **************************************************** */
     System.out.printf("%42s\n", "encodeBit Test");
     System.out.printf("%s\n", H2);

     System.out.printf("Encoding 0|1, 4 bytes\n");
      for (int i = 0; i < 32; ++i) {
        ACFLW.encodeBit(i % 2 == 0);
      }

     System.out.printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n",
          BS.getByteStream()[(int) BS.getLength() - 4], BS.getByteStream()[(int) BS.getLength() - 3],
          BS.getByteStream()[(int) BS.getLength() - 2], BS.getByteStream()[(int) BS.getLength() - 1]);
     System.out.printf("%s\n", H2);
     System.out.printf("%s\n", H1);
      /* **************************************************** */
     System.out.printf("%42s\n", "encodeBitContext Test");
     System.out.printf("%s\n", H2);

     System.out.printf("Encoding 0|1, 4 bytes, context 0 (1 total)\n");
      for (int i = 0; i < 32; ++i) {
        ACFLW.encodeBitContext(i % 2 == 0, 0);
      }

     System.out.printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n",
          BS.getByteStream()[(int) BS.getLength() - 4], BS.getByteStream()[(int) BS.getLength() - 3],
          BS.getByteStream()[(int) BS.getLength() - 2], BS.getByteStream()[(int) BS.getLength() - 1]);

     System.out.printf("%s\n", H2);
     System.out.printf("%s\n", H1);
      /* **************************************************** */
     System.out.printf("%42s\n", "encodeInteger Test");
     System.out.printf("%s\n", H2);

     System.out.printf("Encoding (2^24 - 1) in 3 bytes\n");
      ACFLW.encodeInteger(0xFFFFFF, 24);

     System.out.printf("Encoding 0|32|64|96, 5 bytes\n");
      for (int i = 0; i < 5; ++i) {
        ACFLW.encodeInteger(32 * (i % 4), 8);
      }

     System.out.printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n\t%-4d | %-4d | %-4d | %-4d\n",
          BS.getByteStream()[(int) BS.getLength() - 8], BS.getByteStream()[(int) BS.getLength() - 7],
          BS.getByteStream()[(int) BS.getLength() - 6], BS.getByteStream()[(int) BS.getLength() - 5],
          BS.getByteStream()[(int) BS.getLength() - 4], BS.getByteStream()[(int) BS.getLength() - 3],
          BS.getByteStream()[(int) BS.getLength() - 2], BS.getByteStream()[(int) BS.getLength() - 1]);
     System.out.printf("%s\n", H2);
     System.out.printf("%s\n", H1);
      /* **************************************************** */
      System.out.printf("%42s\n", "terminate Test");
      System.out.printf("%s\n", H2);

      System.out.printf("ByteStream buffer has currently %d bytes, encoding another 5K bits (1..1)\n", (int) BS.getLength());
      for (int i = 0; i < 5000; ++i) {
        ACFLW.encodeBit(true);
      }
      System.out.printf("%s\n", H2);
      System.out.printf("Terminating with %d bytes on buffer\n", (int) BS.getLength());
      ACFLW.terminate();
      System.out.printf("ByteStream limit  \t\tgot: %d bytes\texpected: %d\n", (int) BS.getLength(), 687);
      System.out.printf("ByteStream length \t\tgot: %d bytes\texpected: %d\n", (int) BS.getByteStream().length, 1024);
      System.out.printf("%s\n", H2);
      System.out.printf("Terminating (again) with %d bytes on buffer\n", (int) BS.getLength());
      ACFLW.terminate();
      System.out.printf("ByteStream limit  (again)\tgot: %-10d bytes\texpected: %d\n", (int) BS.getLength(), 689);
      System.out.printf("ByteStream length (again)\tgot: %-10d bytes\texpected: %d\n", (int) BS.getByteStream().length, 1024);
      System.out.printf("%s\n", H2);
      System.out.printf("ByteStream buffer has currently %d bytes, encoding another 1M bits (1..1)\n", (int) BS.getLength());
      for (int i = 0; i < 1000000; ++i) {
        ACFLW.encodeBit(true);
      }
      System.out.printf("%s\n", H2);
      System.out.printf("Terminating with %d bytes on buffer\n", (int) BS.getLength());
      ACFLW.terminate();
      System.out.printf("ByteStream limit  \t\tgot: %-10d bytes\texpected: %d\n", (int) BS.getLength(), 134024);
      System.out.printf("ByteStream length \t\tgot: %-10d bytes\texpected: %d\n", (int) BS.getByteStream().length, 262144);
      System.out.printf("%s\n", H2);
      /* **************************************************** */
      ACFLW.restartDecoding();
      byte[] decodedBuffer = new byte[(int) BS.getLength()];
      int idx = 0;
      /* **************************************************** */
      System.out.printf("%42s\n", "decodeBitProb Test");
      System.out.printf("%s\n", H2);

      System.out.printf("Decoding 0-%d of %d bytes\n", 3, (int) BS.getLength());
      for (int i = 1; i < 5; ++i) {
        for (int j = 7; j >= 0; --j) {
          int decodedAux = ACFLW.decodeBitProb(ACFLW.prob0ToFLW(probSet[i], 15)) == true ? 1 : 0;
          decodedBuffer[idx] |= (decodedAux << j);
        }
        idx++;
      }

      System.out.printf("Decoded result:\n");
      for (int i = 0; i < idx; i+=4) {
        System.out.printf("\t%-4d | %-4d | %-4d | %-4d\n",
            decodedBuffer[i    ], decodedBuffer[i + 1],
            decodedBuffer[i + 2], decodedBuffer[i + 3]);
      }

      System.out.printf("%s\n", H2);
      System.out.printf("%s\n", H1);
      /* **************************************************** */
      System.out.printf("%42s\n", "decodeBit Test");
      System.out.printf("%s\n", H2);

      System.out.printf("Decoding %d->%d bytes\n", 4, 7);
      for (int i = 0; i < 32; ++i) {
        for (int j = 7; j >= 0; --j) {
          int decodedAux = ACFLW.decodeBit() == true ? 1 : 0;
          decodedBuffer[idx] |= decodedAux << j;
        }
        idx++;
      }

      System.out.printf("Decoded result:\n");
      for (int i = 4; i < 8; i += 4) {
        System.out.printf("\t%-4d | %-4d | %-4d | %-4d\n",
            decodedBuffer[i    ], decodedBuffer[i + 1],
            decodedBuffer[i + 2], decodedBuffer[i + 3]);
      }

      System.out.printf("%s\n", H2);
      System.out.printf("%s\n", H1);
      /* **************************************************** */
      System.out.printf("%42s\n", "decodeBitContext Test");
      System.out.printf("%s\n", H2);

      System.out.printf("Decoding %d->%d bytes\n", 8, 11);
      for (int i = 0; i < 32; ++i) {
        for (int j = 7; j >= 0; --j) {
          int decodedAux = ACFLW.decodeBitContext(0) == true ? 1 : 0;
          decodedBuffer[idx] |= decodedAux << j;
        }
        idx++;
      }

      System.out.printf("Decoded result:\n");
      for (int i = 8; i < 12; i += 4) {
        System.out.printf("\t%-4d | %-4d | %-4d | %-4d\n",
            decodedBuffer[i    ], decodedBuffer[i + 1],
            decodedBuffer[i + 2], decodedBuffer[i + 3]);
      }

      System.out.printf("%s\n", H2);
      System.out.printf("%s\n", H1);
      /* **************************************************** */
      System.out.printf("%42s\n", "decodeInteger Test");
      System.out.printf("%s\n", H2);

      System.out.printf("Decoding %d->%d bytes\n", 12, 19);

      int i24 = ACFLW.decodeInteger(24);
      decodedBuffer[idx    ] = (byte) (i24       & 0xFF);
      decodedBuffer[idx + 1] = (byte) (i24 >> 8  & 0xFF);
      decodedBuffer[idx + 2] = (byte) (i24 >> 16 & 0xFF);
      idx += 3;
      for (int i = 0; i < 5; ++i) {
        int decodedAux = ACFLW.decodeInteger(8);
        for (int j = 7; j >= 0; --j) {
          decodedBuffer[idx] |= (0b1 & decodedAux) << j;
        }
        idx++;
      }

      System.out.printf("Decoded result:\n");
      for (int i = 12; i < 20; i += 4) {
        System.out.printf("\t%-4d | %-4d | %-4d | %-4d\n",
            decodedBuffer[i    ], decodedBuffer[i + 1],
            decodedBuffer[i + 2], decodedBuffer[i + 3]);
      }

      System.out.printf("%s\n", H2);
      System.out.printf("%s\n", H1);
      /* **************************************************** */
    }
}
