
import java.io.IOException;

public class BSTest {
  public static void main(String[] args) throws IOException {
    System.out.println("**************************************************************************");
    System.out.println("Initializing ByteStream");
    ByteStream BS = new ByteStream();
    System.out.println("**************************************************************************");

    System.out.println("putByte Test");
    System.out.println("__________________________________________________________________________");
    byte[] ptr;
    boolean verif;
    for (int i = 0; i < 10; i++) {
      ptr = BS.getByteStream();
      verif = true;
      while (verif) {
        BS.putByte((byte) 1); // [0 - 255], overflow not allowed
        verif = ptr == BS.getByteStream() ? true : false;
      }
      System.out.printf("old: %s\tnew: %s\tlength: %d\n", ptr,  BS.getByteStream(), BS.getByteStream().length);
    }
    ptr = BS.getByteStream();
    System.out.println("__________________________________________________________________________");
    System.out.printf("0: %d\t\t65536: %d\t\t524287: %d\t\t1000000: %d\n",
    ptr[0], ptr[65536], ptr[524287], ptr[1000000]);
    System.out.println("**************************************************************************");

  }
}
