#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ByteStream.h"

#define DEFAULT_P1 1000L
#define DEFAULT_P2 1000000L
#define DEFAULT_P3 ((long long) INITIAL_ALLOCATION)

/*
 * Sequentially executes the function getByte to get
 * a ByteStream BS's position equal to targetIndex
 */
signed char lookUpByte(ByteStream *BS, long long targetIndex) {
  for (int i = 0; i < targetIndex; ++i) {
    getByte_0(BS);
  }
  return getByte_0(BS);
}

/*
 * Fills the ByteStream BS's buffer with 1..1
 * param limit: the number of bytes added to the stream
 */
inline void fillByteStream(ByteStream *BS, long long limit) {
  for (int i = 0; i < limit; ++i) {
    putByte(BS, (signed char) i & 0xFF);
  }
}

int main(int argc, char *argv[]) {
  ByteStream *BS;
  FileChannel FC;

  int OP = 0;
  if (argc > 1) OP = atol(argv[1]);

  long long P1 = DEFAULT_P1, P2 = DEFAULT_P2, P3 = DEFAULT_P3;
  if (argc > 2) P1 = atol(argv[2]);
  if (argc > 3) P2 = atol(argv[3]);
  if (argc > 4) P3 = atol(argv[4]);

  switch (OP) {
    case 0:
      printf("Performance test for putByte() with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n\tAllocation:  %lld\n",
          P1, P2, P3);

      BS = ByteStream_1(P3);

      for (int i = 0; i < P1; ++i) {
        for (int i = 0; i < P2; ++i) {
          putByte(BS, (signed char) (i & 0xFF));
        }
        clear(BS);
      }

      destroy(BS);
      free(BS);
      break;

    case 1:
      printf("Performance test for getByte_0(normal mode) with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n\tAllocation:  %lld\n",
          P1, P2, P3);

      BS = ByteStream_1(P3);
      fillByteStream(BS, P2);

      for (int i = 0; i < P1; ++i) {
        lookUpByte(BS, P2);
        ByteStream_reset(BS);
      }

      destroy(BS);
      free(BS);
      break;

    case 2:
      printf("Performance test for getByte_0(readFile mode) with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n\tSegments:  %lld\n",
          P1, P2, P3);

      FC = FileChannel_0("../../files/sg_0.tmp", "r");
      BS = ByteStream_2(FC);

      for (int i = 0; i < P3; i++) { // Multiple segments may be better on multithreading
        putFileSegment(BS, i * fcSize(&FC) / P3, fcSize(&FC) / P3);
      }

      for (int i = 0; i < P1; ++i) {
        lookUpByte(BS, P2);
        ByteStream_reset(BS);
      }

      destroy(BS);
      free(BS);
      break;

    default:
      break;
  }
  printf("Finish\n");
  return 0;
}
