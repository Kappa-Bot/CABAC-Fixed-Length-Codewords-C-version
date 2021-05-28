#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ArithmeticCoderFLW.h"

#define DEFAULT_P1 1000L
#define DEFAULT_P2 1000000L
#define DEFAULT_P3 ((long long) INITIAL_ALLOCATION)

/*
 * Sequentially executes the function decodeBit repeteadly.
 * param targetIndex: the number of bytes decoded
 */
inline void lookUpByte(ArithmeticCoderFLW *ACFLW, int targetIndex) {
  for (int i = 0; i < targetIndex * 8; ++i) {
    decodeBit(ACFLW);
  }
}

/*
 * Fills the ArithmeticCoderFLW ACFLW with 1..1
 * param limit: the number of bytes encoded
 */
inline void fillArithmeticCoderFLW(ArithmeticCoderFLW *ACFLW, int limit) {
  for (int i = 0; i < limit * 8; ++i) {
    encodeBit(ACFLW, (signed char) ((i % 256) & 0xFF));
  }
}

int main(int argc, char *argv[]) {
  ArithmeticCoderFLW *ACFLW;
  ByteStream *BS;
  FileChannel FC;

  int OP = 0;
  if (argc > 1) OP = atoll(argv[1]);

  long long P1 = DEFAULT_P1, P2 = DEFAULT_P2, P3 = DEFAULT_P3;
  if (argc > 2) P1 = atoll(argv[2]);
  if (argc > 3) P2 = atoll(argv[3]);
  if (argc > 4) P3 = atoll(argv[4]);


  switch (OP) {
    case 0:
      printf("Performance test for encodeBit() with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n\tAllocation:  %lld\n",
          P1, P2, P3);

      ACFLW = ArithmeticCoderFLW_3(32, 15, 1);
      BS = ByteStream_1(P3);
      changeStream(ACFLW, BS);

      for (int i = 0; i < P1; ++i) {
        fillArithmeticCoderFLW(ACFLW, P2);
        restartEncoding(ACFLW);
        ByteStream_reset(BS);
      }

      destroy(BS);
      free(BS);
      free(ACFLW);
      break;

    case 1:
      printf("Performance test for decodeBit(normal mode) with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n\tAllocation:  %lld\n",
          P1, P2, P3);

      ACFLW = ArithmeticCoderFLW_3(32, 15, 1);
      BS = ByteStream_1(P3);
      changeStream(ACFLW, BS);
      fillArithmeticCoderFLW(ACFLW, P2);
      restartDecoding(ACFLW);

      for (int i = 0; i < P1; ++i) {
        lookUpByte(ACFLW, P2);
        restartDecoding(ACFLW);
        ByteStream_reset(BS);
      }

      destroy(BS);
      free(BS);
      free(ACFLW);
      break;

    case 2:
      printf("Performance test for decodeBit(readFile mode) with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n\tSegments:  %lld\n",
          P1, P2, P3);

      ACFLW = ArithmeticCoderFLW_3(32, 15, 1);
      FC = FileChannel_0("../../files/sg_0.tmp", "r");
      BS = ByteStream_2(FC);

      for (int i = 0; i < P3; i++) {
        putFileSegment(BS, i * fcSize(&FC) / P3, fcSize(&FC) / P3);
      }
      changeStream(ACFLW, BS);
      restartDecoding(ACFLW);

      for (int i = 0; i < P1; ++i) {
        lookUpByte(ACFLW, P2);
        restartDecoding(ACFLW);
        ByteStream_reset(BS);
      }
      destroy(BS);
      free(BS);
      free(ACFLW);
      break;

    case 3:
      printf("Performance test for prob0ToFLW() with:" \
        "\n\tRepetitions (factor 1 and 2): %lld %lld\n", P1, P2);

      ACFLW = ArithmeticCoderFLW_3(32, 15, 1);
      for (long long i = 0; i < P1; ++i) {
        for (long long j = 0; j < P2; ++j) {
          prob0ToFLW(0.12345f, 15);
        }
      }
      free(ACFLW);
      break;

    case 4:
      printf("Performance test for FLWToProb0() with:" \
        "\n\tRepetitions (factor 1 and 2): %lld %lld\n", P1, P2);

      ACFLW = ArithmeticCoderFLW_3(32, 15, 1);
      for (long long i = 0; i < P1; ++i) {
        for (long long j = 0; j < P2; ++j) {
          FLWToProb0(12345, 15);
        }
      }
      free(ACFLW);
      break;

    default:
      break;
  }
  printf("Finish\n");
  return 0;
}
