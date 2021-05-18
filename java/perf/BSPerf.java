package perf;

import core.*;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;


public class BSPerf {

  final static int DEFAULT_P1 = 1000;
  final static int DEFAULT_P2 = 1000000;
  final static int DEFAULT_P3 = 1024;

  /*
   * Sequentially executes the function getByte to get
   * a ByteStream BS's position equal to targetIndex
   */
  static byte lookUpByte(ByteStream BS, int targetIndex) throws Exception {
    for (int i = 0; i < targetIndex; ++i) {
      BS.getByte();
    }
    return BS.getByte();
  }

  /*
   * Fills the ByteStream BS's buffer with 1..1
   * param limit: the number of bytes added to the stream
   */
  static void fillByteStream(ByteStream BS, int limit) {
    for (int i = 0; i < limit; ++i) {
      BS.putByte((byte) (i & 0xFF));
    }
  }

  public static void main(String []args) throws Exception {
    long startTime = System.currentTimeMillis();

    ByteStream BS;
    FileChannel FC;

    int OP = 0;
    if (args.length > 1) OP = Integer.parseInt(args[0]);

    int P1 = DEFAULT_P1, P2 = DEFAULT_P2, P3 = DEFAULT_P3;
    if (args.length > 2) P1 = Integer.parseInt(args[1]);
    if (args.length > 3) P2 = Integer.parseInt(args[2]);
    if (args.length > 4) P3 = Integer.parseInt(args[3]);

    switch (OP) {
      case 0:
        System.out.printf("Performance test for putByte() with:\n\tRepetitions: %d\n\tOperations:  %d\n\tAllocation:  %d\n", P1, P2, P3);

        BS = new ByteStream(P3);

        for (int i = 0; i < P1; ++i) {
          for (int j = 0; j < P2; ++j) {
            BS.putByte((byte) (j & 0xFF));
          }
          BS.clear();
        }
        break;

      case 1:
        System.out.printf("Performance test for getByte_0(normal mode) with:\n\tRepetitions: %d\n\tOperations:  %d\n\tAllocation:  %d\n", P1, P2, P3);

        BS = new ByteStream(P3);
        fillByteStream(BS, P2);

        for (int i = 0; i < P1; ++i) {
          lookUpByte(BS, P2);
          BS.reset();
        }
        break;

      case 2:
        System.out.printf("Performance test for getByte_0(readFile mode) with:\n\tRepetitions: %d\n\tOperations:  %d\n\tSegments:  %d\n", P1, P2, P3);

        FC = FileChannel.open(FileSystems.getDefault()
          .getPath("../../files/sg_0.tmp"), StandardOpenOption.READ);
        BS = new ByteStream(FC);

        for (int i = 0; i < P3; i++) { // See C version
          BS.putFileSegment(i * fcSize(&FC) / P3, fcSize(&FC) / P3);
        }

        for (int i = 0; i < P1; ++i) {
          lookUpByte(BS, P2);
          BS.reset();
        }
        break;

      default:
        break;
    }

    long endTime = System.currentTimeMillis();
    long timeElapsed = endTime - startTime;
    System.out.printf("Execution time in milliseconds: %d\n", timeElapsed);

    System.out.printf("Finish\n");
  }
}
