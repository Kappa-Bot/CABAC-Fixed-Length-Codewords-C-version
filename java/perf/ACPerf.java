package perf;

import core.*;

import java.io.File;
import java.io.IOException;
import java.nio.ByteBuffer;
import java.nio.channels.FileChannel;
import java.nio.file.FileSystems;
import java.nio.file.StandardOpenOption;


public class ACPerf {

  final static int DEFAULT_P1 = 1000;
  final static int DEFAULT_P2 = 1000000;
  final static int DEFAULT_P3 = 1024;

  /*
   * Sequentially executes the function decodeBit repeteadly.
   * param targetIndex: the number of bytes decoded
   */
  static void lookUpByte(ArithmeticCoderFLW ACFLW, int targetIndex) throws Exception {
    for (int i = 0; i < targetIndex * 8; ++i) {
      ACFLW.decodeBit();
    }
  }

  /*
   * Fills the ArithmeticCoderFLW ACFLW with 1..1
   * param limit: the number of bytes encoded
   */
  static void fillArithmeticCoderFLW(ArithmeticCoderFLW ACFLW, int limit) throws Exception {
    for (int i = 0; i < limit * 8; ++i) {
      ACFLW.encodeBit(((i % 256) & 0x1) == 1 ? true : false);
    }
  }

  public static void main(String []args) throws Exception {
    ArithmeticCoderFLW ACFLW;
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
        System.out.printf("Performance test for encodeBit() with:\n\tRepetitions: %d\n\tOperations:  %d\n\tAllocation:  %d\n", P1, P2, P3);

        ACFLW = new ArithmeticCoderFLW(32, 15, 1);
        BS = new ByteStream(P3);
        ACFLW.changeStream(BS);

        for (int i = 0; i < P1; ++i) {
          fillArithmeticCoderFLW(ACFLW, P2);
          ACFLW.restartEncoding();
        }
        break;

      case 1:
        System.out.printf("Performance test for decodeBit(normal mode) with:\n\tRepetitions: %d\n\tOperations:  %d\n\tAllocation:  %d\n", P1, P2, P3);

        ACFLW = new ArithmeticCoderFLW(32, 15, 1);
        BS = new ByteStream(P3);
        ACFLW.changeStream(BS);
        fillArithmeticCoderFLW(ACFLW, P2);
        ACFLW.restartDecoding();

        for (int i = 0; i < P1; ++i) {
          lookUpByte(ACFLW, P2);
          ACFLW.restartDecoding();
        }
        break;

      case 2:
        System.out.printf("Performance test for decodeBit(readFile mode) with:\n\tRepetitions: %d\n\tOperations:  %d\n\tAllocation:  %d\n", P1, P2, P3);

        ACFLW = new ArithmeticCoderFLW(32, 15, 1);
        FC = FileChannel.open(FileSystems.getDefault()
          .getPath("../../files/w0_c.tmp"), StandardOpenOption.READ);
        BS = new ByteStream(FC);

        for (int i = 0; i < FC.size() / 512; i++) {
          BS.putFileSegment(i * 512, 512);
        }
        ACFLW.changeStream(BS);
        ACFLW.restartDecoding();

        for (int i = 0; i < P1; ++i) {
          lookUpByte(ACFLW, P1);
          ACFLW.restartDecoding();
        }
        break;

      case 4:
        System.out.printf("Performance test for prob0ToFLW() with:\n\tRepetitions (factor 1 and 2): %d %d\n", P1, P2);

        ACFLW = new ArithmeticCoderFLW(32, 15, 1);
        for (long i = 0; i < P1; ++i) {
          for (long j = 0; j < P2; ++j) {
            ACFLW.prob0ToFLW(0.12345f, 15);
          }
        }
        break;

      case 5:
        System.out.printf("Performance test for FLWToProb0() with:\n\tRepetitions (factor 1 and 2): %d %d\n", P1, P2);

        ACFLW = new ArithmeticCoderFLW(32, 15, 1);
        for (long i = 0; i < P1; ++i) {
          for (long j = 0; j < P2; ++j) {
            ACFLW.FLWToProb0(12345, 15);
          }
        }
        break;

      default:
        break;
    }
    System.out.printf("Finish\n");
  }
}
