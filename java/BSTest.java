
import java.io.IOException;


public class BSTest {
  static final String H1 = "**************************************************************************";
  static final String H2 = "__________________________________________________________________________";

  public static void main(String[] args) throws IOException {
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
         * Just to remind the byte format of Java
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
  }
}
