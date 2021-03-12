import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;

public class FCTest {

  final static String FILENAME1 = "../files/fc1.tmp";
  final static String FILENAME2 = "../files/fc2.tmp";

  public static void main(String []args) throws IOException {
    FileChannel FC1, FC2;
    ByteBuffer BB;

    FC1 = FileChannel.open(FileSystems.getDefault()
          .getPath(FILENAME1),
          StandardOpenOption.READ);

    long size = FC1.size();
    long before = FC1.position();
    FC1.position(size / 2);
    long after = FC1.position();

    System.out.printf("Size of file: %-8d\tStarting at: %-10d\tMid point at: %d\n",
                        size, before, after);

    BB = ByteBuffer.allocate((int) size);
    BB.clear();
    FC1.read(BB, size / 2);
    for (int i = 0; i < size; i++) {
      System.out.printf("%c", BB.array()[i]);
    }
    FC1.position(0);

    FC2 = FileChannel.open(FileSystems.getDefault()
          .getPath(FILENAME2),
          StandardOpenOption.WRITE);
    FC2.transferFrom(FC1, 0, size / 2);

    FC1.close();
    FC2.close();
  }
}
