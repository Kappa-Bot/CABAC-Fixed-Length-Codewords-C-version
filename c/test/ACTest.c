#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ArithmeticCoderFLW.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"

const float probSet[] = {-0.5f, 0.0f, 0.01f, \
                          0.25f, 0.33f, 0.5f, 0.66f, \
                          0.99f, 1.0f, 1.5f};
const int precisionSet[] = {-1, 0, 1, 2, 3, 8, 17, 64, 512, 2048};

int main() {
  printf("%s\n", H1);
  printf("Initializing ByteStream\n");
  printf("%s\n", H1);

  ArithmeticCoderFLW *ACFLW = ArithmeticCoderFLW_0();
  ByteStream *BS = ByteStream_0();
  changeStream(ACFLW, BS);

  printf("%s\n", H1);
  printf("%41s\n", "prob0ToFLW + FLWToProb0 Test");
  for (int i = 0; i < 10; i++) {
    float prob0 = probSet[i];
    printf("%s\n", H2);
    printf("%-16s %f\n", "Probability:", prob0);

    for (int j = 0; j < 10; j++) {
      int pBits = precisionSet[j];
      int xpFLW = prob0ToFLW(prob0, pBits);
      float xpProb = FLWToProb0(xpFLW, pBits);
      int realFLW = prob0ToFLW(xpProb, pBits);
      float realProb = FLWToProb0(realFLW, pBits);

      printf("Bits: %d \t %6d \t %f \t %6d \t %f\n",
            pBits, xpFLW, xpProb, realFLW, realProb);
    }
  }
  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "encodeInteger Test");
  printf("%s\n", H2);

  char *ptr = (char *) BS->buffer.array;

  printf("0|32|64 ->  8 times\n");
  for (int i = 0; i < 8; ++i) {
    encodeInteger(ACFLW, 32 * (i % 3), 32);
  }

  printf("Buffer  -> 32 bytes\n");
  for (int i = 0; i < 8; i += 4) {
    printf(" %d | %d | %d | %d\n",
        ptr[i], ptr[i + 1], ptr[i + 2], ptr[i + 3]);
  }
  printf("%s\n", H2);
  /* **************************************************** */

  free(BS);
  free(ACFLW);
  return(0);
}
