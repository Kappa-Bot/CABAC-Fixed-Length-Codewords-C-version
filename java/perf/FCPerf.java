package perf;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.io.FileOutputStream;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;

import java.lang.Thread;

public class FCPerf {

  final static String FILENAME_READ = "../files/sg_0.tmp";
  final static String FILENAME_WRITE = "../files/perf_w.tmp";

  final static long DEFAULT_P1 = 1000L;
  final static long DEFAULT_P2 = 1000000L;

  final static int pthr = java.lang.Thread.activeCount();

  public static void main(String []args) throws IOException {
    long startTime = System.currentTimeMillis();

    FileChannel FC1, FC2; FileOutputStream FOS;
    ByteBuffer BB;

    System.out.printf("Numthreads: %d", Runtime.getRuntime().availableProcessors());

    int OP = 0;
    if (args.length > 0) OP = Integer.parseInt(args[0]);

    long P1 = DEFAULT_P1, P2 = DEFAULT_P2;
    if (args.length > 1) P1 = Long.parseLong(args[1]);
    if (args.length > 2) P2 = Long.parseLong(args[2]);

    switch (OP) {
      case 0:
        System.out.printf("Performance test for fcRead() with:\n\tRepetitions: %d\n\tOperations:  %d\n", P1, P2);

        FC1 = FileChannel.open(FileSystems.getDefault()
          .getPath(FILENAME_READ), StandardOpenOption.READ);
        BB = ByteBuffer.allocate((int) P2);

        for (int i = 0; i < P1; ++i) {
          FC1.read(BB, 0);
        }

        FC1.close();
        break;

      case 1:
        System.out.printf("Performance test for fcRead1B() (sequential) with:\n\tRepetitions: %d\n\tOperations:  %d\n", P1, P2);

        FC1 = FileChannel.open(FileSystems.getDefault()
          .getPath(FILENAME_READ), StandardOpenOption.READ);
        BB = ByteBuffer.allocate((int) P2);

        for (int i = 0; i < P1; ++i) {
          for (int j = 0; j < P2; ++j) {
            FC1.read(BB, j);
          }
        }

        FC1.close();
        break;

      case 3:
        System.out.printf("Performance test for fcWrite() with:\n\tRepetitions: %d\n\tOperations:  %d\n", P1, P2);

        FOS = new FileOutputStream(FILENAME_WRITE);

        byte Barr[] = new byte[(int) P2];

        for (int i = 0; i < P2; ++i) {
          Barr[i] = (byte) (i % 256);
        }

        for (int i = 0; i < P1; ++i) {
          FOS.write(Barr , 0, (int) P2);
        }

        FOS.close();
        break;

      case 4:
        System.out.printf("Performance test for fcTransferFrom() with:\n\tRepetitions: %d\n\tOperations:  %d\n", P1, P2);

        FC1 = FileChannel.open(FileSystems.getDefault()
          .getPath(FILENAME_READ), StandardOpenOption.READ);
        FC2 = FileChannel.open(FileSystems.getDefault()
          .getPath(FILENAME_WRITE), StandardOpenOption.WRITE);

        for (int i = 0; i < P1; ++i) {
          FC2.transferFrom(FC1, 0, P2);
        }

        FC1.close();
        FC2.close();
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
