import java.io.FileOutputStream;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;

public class FULL {
  static final String H1 = "**************************************************************************";
  static final String H2 = "__________________________________________________________________________";

  // Message contains "ARITHMETIC"
  static char[] code_table = new char[256];

  static char lookupTable(int flw) {
    for (char i = 0; i < 256; ++i) {
      // The index(i) is the symbol
      if (code_table[(int) i] == flw) return i;
    }
    return (char) -1;
  }

  public static void main(String[] args) throws IOException, Exception {
    code_table['A'] = 4;
    code_table['R'] = 5;
    code_table['I'] = 6;
    code_table['T'] = 7;
    code_table['H'] = 8;
    code_table['M'] = 9;
    code_table['E'] = 10;
    code_table['C'] = 11;


    System.out.printf("Initializing ArithmeticCoderFLW\n");

    /*
     * Initialize with:
     *  - codewordLength ->  3
     *  - precisionBits  ->  8
     *  - numContexts    ->  1
     */
    ArithmeticCoderFLW ACN = new ArithmeticCoderFLW(4, 8, 1);

    System.out.printf("Initializing ByteStream (normal mode)\n");
    /*
     * Initialize with:
     *  - initialAllocation = INITIAL_ALLOCATION
     *  - streamMode        = 0
     */
    ByteStream BSN = new ByteStream(1024);

    // Links normal-mode ByteStream to the ArithmeticCoderFLW
    ACN.changeStream(BSN);
    /* **************************************************** */

    System.out.printf("Encoding the message: \"%s\"\n", "ARITHMETIC");

    /*
     * Encoding the Message
     *  A - R - I - T - H - M - E - T - I - C
     */
    for (int i = 0; i < 10; ++i) {
      ACN.encodeInteger(code_table['A'], 4);
      ACN.encodeInteger(code_table['R'], 4);
      ACN.encodeInteger(code_table['I'], 4);
      ACN.encodeInteger(code_table['T'], 4);
      ACN.encodeInteger(code_table['H'], 4);
      ACN.encodeInteger(code_table['M'], 4);
      ACN.encodeInteger(code_table['E'], 4);
      ACN.encodeInteger(code_table['T'], 4);
      ACN.encodeInteger(code_table['I'], 4);
      ACN.encodeInteger(code_table['C'], 4);
    }

    /* **************************************************** */
    ACN.terminate();
    ACN.restartDecoding();
    /* **************************************************** */

    System.out.printf("Decoding %d bytes\n", BSN.getLength());
    char []aux = new char[(int) BSN.getLength()];
    for (int i = 0; i < BSN.getLength(); ++i) {
      // Looks for a match in code_table[] for the 3-bits flw
      aux[i] = lookupTable(ACN.decodeInteger(4));
    }

    // Should print ARITHMETIC
    long pad = 0;
    System.out.printf("Showing %d decoded bytes:\n\t", BSN.getLength());
    for(int i = 0; i < BSN.getLength(); ++i) {
      System.out.printf("%c", aux[i]);
      // Line break every 10 signed characters (length of "ARITHMETIC")
      pad++;
      if (pad % 10 == 0) System.out.printf("\n\t");
    }
    System.out.printf("\n");

    // Writes the whole BSN.getLength() bytes into a file for next test
    FileOutputStream FCN = new FileOutputStream("../files/full_j.tmp");
    BSN.write(FCN);
    FCN.close();

    /* **************************************************** */

    /*
     * Initialize with:
     *  - codewordLength ->  3
     *  - precisionBits  ->  8
     *  - numContexts    ->  1
     */
    ArithmeticCoderFLW ACR = new ArithmeticCoderFLW(4, 8, 1);

    // Using the previously coded stream written into the file
    FileChannel FCR = FileChannel.open(FileSystems.getDefault()
        .getPath("../files/full_j.tmp"), StandardOpenOption.READ);
    System.out.printf("Initializing ByteStream (readFile mode)\n");
    /*
     * Initialize with:
     *  - readFileChannel = FCR
     *  - streamMode      = 1
     */
    ByteStream BSR = new ByteStream(FCR);

    // Use only one segment
    BSR.putFileSegment(0L, BSN.getLength());

    // Links normal-mode ByteStream to the ArithmeticCoderFLW
    ACR.changeStream(BSR);
    ACR.restartDecoding();
    /* **************************************************** */

    System.out.printf("Decoding %d bytes\n", BSR.getLength());
    char []aux2 = new char[(int) BSR.getLength()];
    for (int i = 0; i < BSR.getLength(); ++i) {
      // Looks for a match in code_table[] for the 3-bits flw
      aux2[i] = lookupTable(ACR.decodeInteger(4));
    }

    // Should print ARITHMETIC
    long pad2 = 0;
    System.out.printf("Showing %d decoded bytes:\n\t", BSR.getLength());
    for(int i = 0; i < BSR.getLength(); ++i) {
      System.out.printf("%c", (char) aux2[i]);
      // Line break every 10 signed characters (length of "ARITHMETIC")
      pad2++;
      if (pad2 % 10 == 0) System.out.printf("\n\t");
    }
    System.out.printf("\n");
    /* **************************************************** */
  }
}
