package test;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;

public class FCTest {

  final static String FILENAME1 = "../files/fc1.tmp";
  final static String FILENAME2 = "../files/fc2.tmp";

  final static String H1 = "**************************************************************************";
  final static String H2 = "__________________________________________________________________________";

  public static void main(String []args) throws IOException {
    System.out.printf("%s\n", H1);
    System.out.printf("%45s\n", "Initializing FileChannels");
    System.out.printf("FileChannel 1 |read|  fc1.tmp contains\t|\t\'lorem ipsum\'\n");
    System.out.printf("FileChannel 2 |write| fc2.tmp receives\t|\t\'lorem ipsum\'\n");
    System.out.printf("%s\n", H1);

    FileChannel FC1 = FileChannel.open(FileSystems.getDefault()
        .getPath(FILENAME1), StandardOpenOption.READ);

    /* **************************************************** */
    System.out.printf("%41s\n", "fcSize Test");
    System.out.printf("%s\n", H2);
    System.out.printf("Getting lenght of file 1\n");
    long size = FC1.size();
    System.out.printf("File Size\t|\tgot: %-6d\texpected: 13\n", (int) size);
    System.out.printf("%s\n", H2);
    System.out.printf("%s\n", H1);

    /* **************************************************** */
    System.out.printf("%51s\n", "fcGetPosition + fcPosition Test");
    System.out.printf("%s\n", H2);
    System.out.printf("Getting first and middle position (0) by moving to mid point (13 / 2)\n");
    long before = FC1.position();
    System.out.printf("Start Position\t|\tgot: %-6d\texpected: 0\n", (int) before);
    FC1.position(size / 2);
    long after = FC1.position();
    System.out.printf("Mid Position\t|\tgot: %-6d\texpected: 6\n", (int) after);
    System.out.printf("%s\n", H2);
    System.out.printf("%s\n", H1);

    /* **************************************************** */
    System.out.printf("%41s\n", "fcRead Test");
    System.out.printf("%s\n", H2);

    System.out.printf("Reading 6 bytes from position 0\nGot:      | ");
    FC1.position(0);
    ByteBuffer BB = ByteBuffer.allocate((int) after);
    FC1.read(BB, 0);
    for (int i = 0; i < size / 2; i++) {
      System.out.printf("%d | ", (int) BB.array()[i]);
    }
    System.out.printf("\nExpected: | 108 | 111 | 114 | 101 | 109 | 32 | = \'lorem \'\n");
    System.out.printf("%s\n", H2);

    System.out.printf("Reading 6 bytes from position 6\nGot:      | ");
    BB = ByteBuffer.allocate((int) after);
    FC1.read(BB, size / 2);
    for (int i = 0; i < size / 2; i++) {
      System.out.printf("%d | ", (int) BB.array()[i]);
    }
    System.out.printf("\nExpected: | 105 | 112 | 115 | 117 | 109 | 13 | = \'ipsum\\n\'\n");

    System.out.printf("%s\n", H2);
    System.out.printf("%s\n", H1);

    /* **************************************************** */
    System.out.printf("%45s\n", "fcTransferFrom Test");
    System.out.printf("%s\n", H2);
    FileChannel FC2 = FileChannel.open(FileSystems.getDefault()
        .getPath(FILENAME2), StandardOpenOption.WRITE);
    long written = FC2.transferFrom(FC1, 0, size * 2);
    System.out.printf("Tried to Write: %d\nFile Size: %d\nTotal Written: %d\n",
        (int) (size * 2), (int) size, (int) written);
    System.out.printf("%s\n", H2);
    System.out.printf("%s\n", H1);

    FC1.close();
    FC2.close();
  }
}
