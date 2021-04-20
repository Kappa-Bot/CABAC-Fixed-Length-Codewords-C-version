
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;


public class BSTest {
  static final String H1 = "**************************************************************************";
  static final String H2 = "__________________________________________________________________________";

  public static int pack4B(byte b1, byte b2, byte b3, byte b4) {
    // IMPORTANT: BigEndian by default
    ByteBuffer dbuf = ByteBuffer.allocate(4);
    dbuf.put(b4);
    dbuf.put(b3);
    dbuf.put(b2);
    dbuf.put(b1);
    return dbuf.getInt(0);
  }

  public static byte lookUpByte_0(ByteStream BS, int targetIndex) throws Exception {
    for (int i = 0; i < targetIndex; ++i) {
      BS.getByte();
    }
    return BS.getByte();
  }

  public static void main(String[] args) throws IOException, Exception {
    System.out.println(H1);
    System.out.println("Initializing ByteStream");
    ByteStream BS = new ByteStream();
    System.out.println(H1);

    System.out.println("putByte Test");
    System.out.println(H2);
    byte[] ptr;
    boolean verif;
    for (int i = 0; i < 10; i++) {
      ptr = BS.getByteStream();
      verif = true;
      while (verif) {
        BS.putByte((byte) 1); // [0 - 255], overflow not allowed
        verif = ptr == BS.getByteStream();
      }
      System.out.printf("old: %s\tnew: %s\tlength: %d\n", ptr,  BS.getByteStream(), BS.getByteStream().length);
    }
    ptr = BS.getByteStream();
    System.out.println(H2);
    System.out.printf("0: %d\t\t65536: %d\t\t524287: %d\t\t1000000: %d\n",
    ptr[0], ptr[65536], ptr[524287], ptr[1000000]);
    System.out.println(H1);
    /* **************************************************** */
    System.out.printf("%42s\n", "putBytes_0 Test");
    System.out.printf("%s\n", H2);
    BS = new ByteStream();
    byte[] arr = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    for (int i = 0; i < 10; i++) {
      ptr = BS.getByteStream();
      verif = true;
      while (verif) {
        BS.putBytes(arr, 0, 16); // Whole array, sequences of [0-16]
        verif = ptr == BS.getByteStream();
      }
      System.out.printf("old: %s\tnew: %s\tlength: %d\n",
        ptr, BS.getByteStream(), BS.getByteStream().length);
    }
    System.out.printf("%s\n", H2);
    ptr = BS.getByteStream();
    System.out.printf("0: %d\t\t67541: %d\t\t540639: %d\t\t1000000: %d\n",
              ptr[0], ptr[67541], ptr[540639], ptr[1000000]);
    System.out.printf("%s\n", H1);
    /* **************************************************** */
    System.out.printf("%42s\n", "putBytes_1 Test");
    System.out.printf("%s\n", H2);
    BS = new ByteStream();
    for (int i = 0; i < 10; i++) {
      ptr = BS.getByteStream();
      verif = true;
      while (verif) {
        /*
         * Just to remind the byte format of Java (same as C's signed char)
         * UINT8    BYTE
         *   1       1
         *  127     127
         *  128    -128 (0 from another alternative dimension, somehow)
         *  129    -127
         *  255     -1
         */
        BS.putBytes(2147483647, 256); // Max 4B value
        verif = ptr == BS.getByteStream();
      }
      System.out.printf("old: %s  \tnew: %s  \tlength: %d\n",
        ptr, BS.getByteStream(), BS.getByteStream().length);
    }
    System.out.printf("%s\n", H2);
    ptr = BS.getByteStream();
    System.out.printf("0: %d\t\t65534: %d\t\t524287: %d\t\t1000000: %d\n",
              ptr[0], ptr[65534], ptr[524287], ptr[1000000]);
    System.out.printf("%s\n", H1);
    /* **************************************************** */
    BS.reset();
    System.out.printf("%36s\n", "getByte_1 Test (normal mode)");
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", BS.getByte(0), BS.getByteStream()[0]);
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", BS.getByte(65534), BS.getByteStream()[65534]);
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", BS.getByte(524287), BS.getByteStream()[524287]);
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 1000000\t\t Got: %d\t\t\tExpected: %d\n", BS.getByte(1000000), BS.getByteStream()[1000000]);

    System.out.printf("%s\n", H1);
    /* **************************************************** */
    System.out.printf("%36s\n", "getByte_0 Test (normal mode)");
    System.out.printf("%s\n", H2);

    // Just don't want employ resets here for future tests
    System.out.printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 0), BS.getByteStream()[0]);
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 65534 - 1), BS.getByteStream()[65534]);
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 524287 - 65535), BS.getByteStream()[524287]);
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 1000000\t\t Got: %d\t\t\tExpected: %d\n", lookUpByte_0(BS, 1000000 - 524288), BS.getByteStream()[1000000]);
    System.out.printf("%s\n", H2);
    /* **************************************************** */
    System.out.printf("%36s\n", "getBytes Test (normal mode)");
    System.out.printf("%s\n", H2);
    BS.reset(); // lol I lied

    System.out.printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", BS.getBytes(1), pack4B(BS.getByteStream()[0], (byte) 0, (byte) 0, (byte) 0));
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 1-2\t\t Got: %d\t\tExpected: %d\n", BS.getBytes(2), pack4B(BS.getByteStream()[2], BS.getByteStream()[1], (byte) 0, (byte) 0));
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 3-5\t\t Got: %d\t\tExpected: %d\n", BS.getBytes(3), pack4B(BS.getByteStream()[5], BS.getByteStream()[4], BS.getByteStream()[3], (byte) 0));
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 6-9\t\t Got: %d\t\tExpected: %d\n", BS.getBytes(4), pack4B(BS.getByteStream()[9], BS.getByteStream()[8], BS.getByteStream()[7], BS.getByteStream()[6]));
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 10-14\t\t Got: %d \t\tExpected: %d\n", BS.getBytes(5),
      pack4B(BS.getByteStream()[14], BS.getByteStream()[13], BS.getByteStream()[12], BS.getByteStream()[11]) + pack4B(BS.getByteStream()[10], (byte) 0, (byte) 0, (byte) 0));
    System.out.printf("%s\n", H2);

    System.out.printf("Index: 15-22\t\t Got: %d \tExpected: %d\n", BS.getBytes(8),
      pack4B(BS.getByteStream()[22], BS.getByteStream()[21], BS.getByteStream()[20], BS.getByteStream()[19])
      + pack4B(BS.getByteStream()[18], BS.getByteStream()[17], BS.getByteStream()[16], BS.getByteStream()[15]));
    System.out.printf("%s\n", H2);
    /* Code for displaying the bits of the output number step by step in function getBytes(BS, num)

        // Simplified status of the buffer
        System.out.printf("\t\t%d\t%d|%d|%d|%d\n", (char) getByte, (num >> 0) & 0xff, (num >> 8) & 0xff, (num >> 16) & 0xff, (num >> 24) & 0xff);

        // New byte added
        System.out.printf("(%d) -> %d = ", (char) getByte, b * 8);
        for (int j = 7; j >= 0; j--) {
          System.out.printf("%c", (((char) getByte) & (1 << j)) ? '1' : '0');
        }
        System.out.printf(" ");
        System.out.printf("\n");

        // Detailed status of the buffer
        System.out.printf("(%d) %*s = ", num, 8, "->");
        for (int i = 0; i <= 3; i++) {
          for (int j = 7; j >= 0; j--) {
            System.out.printf("%c", ((num >> (i * 8)) & (1 << j)) ? '1' : '0');
          }
          System.out.printf(" ");
        }
        System.out.printf("\n");
    */
    /* **************************************************** */
    System.out.printf("%36s\n", "write_1 Test");
    System.out.printf("%s\n", H2);
    BS.reset(); // lol I lied

    boolean verifOut;
    ByteBuffer tmpBuff0 = ByteBuffer.allocate((int) (BS.getLength() / 2));

    System.out.printf("Writing [0 - %d] bytes into w1_0_j.tmp\n", (int) (BS.getLength() / 2));
    FileOutputStream fcOut1_0 = new FileOutputStream("../files/w1_0_j.tmp");
    BS.write(fcOut1_0, 0, BS.getLength() / 2);
    fcOut1_0.close();

    System.out.printf("Checking integrity -> w1_0_j.tmp\n");
    FileChannel fcIn1_0 = FileChannel.open(FileSystems.getDefault()
        .getPath("../files/w1_0_j.tmp"), StandardOpenOption.READ);
    fcIn1_0.read(tmpBuff0, 0);
    verifOut = true;
    for (int i = 0; i < (int) (BS.getLength() / 2) && verifOut; ++i) {
      verifOut = BS.getByteStream()[i] == tmpBuff0.array()[i];
    }
    System.out.printf("Result: %s\n", verifOut ? "OK" : "Error");
    fcIn1_0.close();
    System.out.printf("%s\n", H2);
    System.out.printf("%s\n", H1);
    /* **************************************************** */
    System.out.printf("%36s\n", "write_0 Test");
    System.out.printf("%s\n", H2);
    BS.reset(); // lol I lied
    ByteBuffer tmpBuff2 = ByteBuffer.allocate((int) BS.getLength());

    System.out.printf("Writing everything into w0_j.tmp\n");
    FileOutputStream fcOut0 = new FileOutputStream("../files/w0_j.tmp");
    BS.write(fcOut0);
    fcOut0.close();

    System.out.printf("Checking integrity -> w0_j.tmp\n");
    FileChannel fcIn0 = FileChannel.open(FileSystems.getDefault()
        .getPath("../files/w0_j.tmp"), StandardOpenOption.READ);
    fcIn0.read(tmpBuff2, 0);
    verifOut = true;
    for (int i = 0; i < BS.getLength() && verifOut; ++i) {
      verifOut = BS.getByteStream()[i] == tmpBuff2.array()[i];
    }
    System.out.printf("Result: %s\n", verifOut ? "OK" : "Error");
    fcIn0.close();

    System.out.printf("%s\n", H1);
    /* **************************************************** */
  }
}
