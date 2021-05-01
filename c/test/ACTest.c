#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ArithmeticCoderFLW.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"
#define pack4B(a, b, c, d) *((int*) (signed char[4] ) {a, b, c, d} )

const float probSet[] = { 0.01f, 0.25f, 0.33f, 0.5f, 0.66f, 0.99f };
const int precisionSet[] = { 2, 3, 8, 15, 17, 24 };

int main() {
  printf("%s\n", H1);
  printf("Initializing ArithmeticCoderFLW\n");
  printf("%s\n", H1);

  // Basically default initialization, but... You know... Better.
  ArithmeticCoderFLW *ACFLW = ArithmeticCoderFLW_3(32, 15, 1);
  ByteStream *BS = ByteStream_0();
  changeStream(ACFLW, BS);
  signed char *ptr = (signed char *)BS->buffer.array;

  printf("%s\n", H1);
  printf("%41s\n", "prob0ToFLW + FLWToProb0 Test");
  for (int i = 0; i < 6; i++) {
    float prob0 = probSet[i];
    printf("%s\n", H2);
    printf("Probability: %f\n", prob0);

    for (int j = 0; j < 6; j++) {
      int pBits = precisionSet[j];
      int xpFLW = prob0ToFLW(prob0, pBits);
      float xpProb = FLWToProb0(xpFLW, pBits);

      printf("Bits: %-16d FLW: %-10d \t<->\t Prob: %f\n",
            pBits, xpFLW, xpProb);
    }
  }
  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%42s\n", "encodeBitProb Test");
  printf("%s\n", H2);

  printf("Encoding 0|1 (0.25|0.33|0.50|0.66)\n");
  for (int i = 1; i < 5; ++i) {
    int prob0 = prob0ToFLW(probSet[i], 15);
    for (int j = 0; j < 8; ++j) {
      encodeBitProb(ACFLW, j % 2 == 0, prob0);
    }
  }

  printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n",
      ptr[BS->limit - 4], ptr[BS->limit - 3],
      ptr[BS->limit - 2], ptr[BS->limit - 1]);
  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "encodeBit Test");
  printf("%s\n", H2);

  printf("Encoding 0|1, 4 bytes\n");
  for (int i = 0; i < 32; ++i) {
    encodeBit(ACFLW, i % 2 == 0);
  }

  printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n",
      ptr[BS->limit - 4], ptr[BS->limit - 3],
      ptr[BS->limit - 2], ptr[BS->limit - 1]);
  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "encodeBitContext Test");
  printf("%s\n", H2);

  printf("Encoding 0|1, 4 bytes, context 0 (1 total)\n");
  for (int i = 0; i < 32; ++i) {
    encodeBitContext(ACFLW, i % 2 == 0, 0);
  }

  printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n",
      ptr[BS->limit - 4], ptr[BS->limit - 3],
      ptr[BS->limit - 2], ptr[BS->limit - 1]);

  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "encodeInteger Test");
  printf("%s\n", H2);

  printf("Encoding (2^24 - 1) in 3 bytes\n");
  encodeInteger(ACFLW, 0xFFFFFF, 24);

  printf("Encoding 0|32|64|96, 5 bytes\n");
  for (int i = 0; i < 5; ++i) {
    encodeInteger(ACFLW, 32 * (i % 4), 8);
  }

  printf("Result in Buffer:\n\t%-4d | %-4d | %-4d | %-4d\n\t%-4d | %-4d | %-4d | %-4d\n",
      ptr[BS->limit - 8], ptr[BS->limit - 7],
      ptr[BS->limit - 6], ptr[BS->limit - 5],
      ptr[BS->limit - 4], ptr[BS->limit - 3],
      ptr[BS->limit - 2], ptr[BS->limit - 1]);
  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "terminate Test");
  printf("%s\n", H2);

  printf("ByteStream buffer has currently %lld bytes, encoding another 5K bits (1..1)\n", BS->limit);
  for (int i = 0; i < 5000; ++i) {
    encodeBit(ACFLW, 1);
  }
  printf("%s\n", H2);
  printf("Terminating with %lld bytes on buffer\n", BS->limit);
  terminate(ACFLW);
  printf("ByteStream limit  \t\tgot: %lld bytes\texpected: %d\n", BS->limit, 687);
  printf("ByteStream length \t\tgot: %lld bytes\texpected: %d\n", BS->buffer.length, 1024);
  printf("%s\n", H2);
  printf("Terminating (again) with %lld bytes on buffer\n", BS->limit);
  terminate(ACFLW);
  printf("ByteStream limit  (again)\tgot: %-10lld bytes\texpected: %d\n", BS->limit, 689);
  printf("ByteStream length (again)\tgot: %-10lld bytes\texpected: %d\n", BS->buffer.length, 1024);
  printf("%s\n", H2);
  printf("ByteStream buffer has currently %lld bytes, encoding another 1M bits (1..1)\n", BS->limit);
  for (int i = 0; i < 1000000; ++i) {
    encodeBit(ACFLW, 1);
  }
  printf("%s\n", H2);
  printf("Terminating with %lld bytes on buffer\n", BS->limit);
  terminate(ACFLW);
  printf("ByteStream limit  \t\tgot: %-10lld bytes\texpected: %d\n", BS->limit, 134024);
  printf("ByteStream length \t\tgot: %-10lld bytes\texpected: %d\n", BS->buffer.length, 262144);
  printf("%s\n", H2);
  /* **************************************************** */
  restartDecoding(ACFLW);
  signed char *decodedBuffer = (signed char *) malloc(BS->limit);
  memset(decodedBuffer, 0, BS->limit);
  int idx = 0;
  /* **************************************************** */
  printf("%42s\n", "decodeBitProb Test");
  printf("%s\n", H2);

  printf("Decoding 0-%d of %lld bytes\n", 3, BS->limit);
  for (int i = 1; i < 5; ++i) {
    for (int j = 7; j >= 0; --j) {
      int decodedAux = decodeBitProb(ACFLW, prob0ToFLW(probSet[i], 15)) == 1 ? 1 : 0;
      decodedBuffer[idx] |= (decodedAux << j);
    }
    idx++;
  }

  printf("Decoded result:\n");
  for (int i = 0; i < idx; i+=4) {
    printf("\t%-4d | %-4d | %-4d | %-4d\n",
        decodedBuffer[i    ], decodedBuffer[i + 1],
        decodedBuffer[i + 2], decodedBuffer[i + 3]);
  }

  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "decodeBit Test");
  printf("%s\n", H2);

  printf("Decoding %d->%d bytes\n", 4, 7);
  for (int i = 0; i < 32; ++i) {
    for (int j = 7; j >= 0; --j) {
      int decodedAux = decodeBit(ACFLW) == 1 ? 1 : 0;
      decodedBuffer[idx] |= decodedAux << j;
    }
    idx++;
  }

  printf("Decoded result:\n");
  for (int i = 4; i < 8; i += 4) {
    printf("\t%-4d | %-4d | %-4d | %-4d\n",
        decodedBuffer[i    ], decodedBuffer[i + 1],
        decodedBuffer[i + 2], decodedBuffer[i + 3]);
  }

  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "decodeBitContext Test");
  printf("%s\n", H2);

  printf("Decoding %d->%d bytes\n", 8, 11);
  for (int i = 0; i < 32; ++i) {
    for (int j = 7; j >= 0; --j) {
      int decodedAux = decodeBitContext(ACFLW, 0) == 1 ? 1 : 0;
      decodedBuffer[idx] |= decodedAux << j;
    }
    idx++;
  }

  printf("Decoded result:\n");
  for (int i = 8; i < 12; i += 4) {
    printf("\t%-4d | %-4d | %-4d | %-4d\n",
        decodedBuffer[i    ], decodedBuffer[i + 1],
        decodedBuffer[i + 2], decodedBuffer[i + 3]);
  }

  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "decodeInteger Test");
  printf("%s\n", H2);

  printf("Decoding %d->%d bytes\n", 12, 19);

  int i24 = decodeInteger(ACFLW, 24);

  decodedBuffer[idx    ] = (i24      ) & 0xFF;
  decodedBuffer[idx + 1] = (i24 >> 8 ) & 0xFF;
  decodedBuffer[idx + 2] = (i24 >> 16) & 0xFF;
  idx += 3;
  for (int i = 0; i < 5; ++i) {
    int decodedAux = decodeInteger(ACFLW, 8);
    for (int j = 7; j >= 0; --j) {
      decodedBuffer[idx] |= (0b1 & decodedAux) << j;
    }
    idx++;
  }

  printf("Decoded result:\n");
  for (int i = 12; i < 20; i += 4) {
    printf("\t%-4d | %-4d | %-4d | %-4d\n",
        decodedBuffer[i    ], decodedBuffer[i + 1],
        decodedBuffer[i + 2], decodedBuffer[i + 3]);
  }

  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */

  free(decodedBuffer);
  destroy(BS);
  free(BS);
  free(ACFLW);
  return(0);
}
