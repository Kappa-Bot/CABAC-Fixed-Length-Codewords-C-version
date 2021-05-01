#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ByteStream.h"

#define DEFAULT_P1 1000L
#define DEFAULT_P2 1000000L
#define DEFAULT_P3 ((long) INITIAL_ALLOCATION)

#define pack4B(a, b, c, d) *((int*) (signed char[4] ) {a, b, c, d} )

/*
 * Sequentially executes the function getByte to get
 * a ByteStream BS's position equal to targetIndex
 */
inline signed char lookUpByte(ByteStream *BS, int targetIndex) {
  for (int i = 0; i < targetIndex; ++i) {
    getByte_0(BS);
  }
  return getByte_0(BS);
}

/*
 * Fills the ByteStream BS's buffer with 1..1
 * param limit: the number of bytes added to the stream
 */
inline void fillByteStream(ByteStream *BS, int limit) {
  for (int i = 0; i < limit; ++i) {
    putByte(BS, (signed char) i & 0xFF);
  }
}

int main(int argc, char *argv[]) {
  ByteStream *BS;

  ByteBuffer BB;
  FileChannel FC;

  int OP = 0;
  if (argc > 1) OP = atol(argv[1]);

  long P1 = DEFAULT_P1, P2 = DEFAULT_P2, P3 = DEFAULT_P3;
  if (argc > 2) P1 = atol(argv[2]);
  if (argc > 3) P2 = atol(argv[3]);
  if (argc > 4) P3 = atol(argv[4]);

  switch (OP) {
    case 0:
      printf("Performance test for putByte() with:" \
          "\n\tRepetitions: %ld\n\tOperations:  %ld\n\tAllocation:  %ld\n",
          P1, P2, P3);

      for (int i = 0; i < P1; ++i) {
        BS = ByteStream_1(P3);
        fillByteStream(BS, P2);
        destroy(BS);
        free(BS);
      }
      break;

    case 1:
      printf("Performance test for putBytes_0() with:" \
          "\n\tRepetitions: %ld\n\tOperations:  %ld\n\tAllocation:  %ld\n",
          P1, P2, P3);

      BB = (ByteBuffer) { malloc(P2), P2 };

      for (int i = 0; i < P1; ++i) {
        BS = ByteStream_1(P3);
        putBytes_0(BS, BB, 0, P2);
        destroy(BS);
        free(BS);
      }
      break;

    case 2:
      printf("Performance test for getByte_0() with:" \
          "\n\tRepetitions: %ld\n\tOperations:  %ld\n\tAllocation:  %ld\n",
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

    case 3:
      printf("Performance test (readFile mode) for getByte_0() with:" \
          "\n\tRepetitions: %ld\n\tOperations:  %ld\n\tAllocation:  %ld\n",
          P1, P2, P3);



      /*
      putFileSegment(BS, 0, FC.stat.st_size);
      for (int i = 0; i < FC.stat.st_blocks; i++) {
        putFileSegment(BS, i * FC.stat.st_blksize, FC.stat.st_blksize);
      }
      putFileSegment(BS, FC.stat.st_blocks, P2 % FC.stat.st_blksize);
      */

      for (int i = 0; i < P1; ++i) {
        FC = FileChannel_0("../../files/w0_c.tmp", "r");
        BS = ByteStream_2(FC);

        putFileSegment(BS, 0, FC.stat.st_size);

        /*
        for (int j = 0; j < (FC.stat.st_size / FC.stat.st_blksize); j++) {
          putFileSegment(BS, j * FC.stat.st_blksize, FC.stat.st_blksize);
        }
        putFileSegment(BS, FC.stat.st_size / FC.stat.st_blksize, FC.stat.st_size % FC.stat.st_blksize);
        */

        lookUpByte(BS, P2);

        destroy(BS);
        free(BS);
      }

      break;

    default:
      break;
  }
  printf("Finish\n");
  fflush(stdout);
  return 0;
}
