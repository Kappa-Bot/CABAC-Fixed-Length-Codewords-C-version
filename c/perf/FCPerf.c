#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/FileChannel.h"

#define FILENAME_READ "../../files/sg_0.tmp"
#define FILENAME_WRITE "../../files/perf_w.tmp"

#define DEFAULT_P1 1000L
#define DEFAULT_P2 1000000L

int main(int argc, char *argv[]) {
  FileChannel FC1, FC2;
  ByteBuffer BB;

  int OP = 0;
  if (argc > 1) OP = atol(argv[1]);

  long long P1 = DEFAULT_P1, P2 = DEFAULT_P2;
  if (argc > 2) P1 = atol(argv[2]);
  if (argc > 3) P2 = atol(argv[3]);

  switch (OP) {
    case 0:
      printf("Performance test for fcRead() with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n",
          P1, P2);

      FC1 = FileChannel_0(FILENAME_READ, FC_READ);
      BB = (ByteBuffer) {(signed char *) malloc(P2 * sizeof(signed char)), P2};

      for (int i = 0; i < P1; ++i) {
        fcRead(&FC1, BB, 0);
      }

      free(BB.array);
      fcClose(&FC1);
      break;

    case 1:
      printf("Performance test for fcRead1B() (sequential) with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n",
          P1, P2);

      FC1 = FileChannel_0(FILENAME_READ, FC_READ);
      BB = (ByteBuffer) {(signed char *) malloc(P2 * sizeof(signed char)), P2};

      for (int i = 0; i < P1; ++i) {
        for (int j = 0; j < P2; ++j) {
          fcRead1B(&FC1, BB, j);
        }
      }

      free(BB.array);
      fcClose(&FC1);
      break;

    case 2:
      printf("Performance test for fcWrite() with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n",
          P1, P2);

      FC2 = FileChannel_0(FILENAME_WRITE, FC_WRITE);
      BB = (ByteBuffer) {(signed char *) malloc(P2 * sizeof(signed char)), P2};

      for (int i = 0; i < P2; ++i) {
        BB.array[i] = (signed char) (i % 256);
      }

      for (int i = 0; i < P1; ++i) {
        fcWrite(&FC2, BB, 0, P2);
      }

      fcClose(&FC2);
      break;

    case 3:
      printf("Performance test for fcTransferFrom() with:" \
          "\n\tRepetitions: %lld\n\tOperations:  %lld\n",
          P1, P2);

      FC1 = FileChannel_0(FILENAME_READ, FC_READ);
      FC2 = FileChannel_0(FILENAME_WRITE, FC_WRITE);
      off_t transferPos = 0;

      for (int i = 0; i < P1; ++i) {
        fcTransferFrom(&FC2, &FC1, &transferPos, P2);
      }

      fcClose(&FC1);
      fcClose(&FC2);
      break;

    default:
      break;
  }
  printf("Finish\n");
  return 0;
}
