#include <stdio.h>

#include "resources.h"


static float probSet[] = {-0.5f, 0.0f, 0.01f,
                          0.25f, 0.33f, 0.5f, 0.66f,
                          0.99f, 1.0f, 1.5f};
static int precisionSet[] = {-1, 0, 1, 2, 3, 8, 17, 64, 512, 2048};

int main() {
  printf("Initializing ArithmeticCoderFLW\n");

  // Mayb replace w a param
  readFileChannel = fopen("../files/file_0.txt", "rb");

  defaultInitACFLW();
  // Dont need the BS for now

  printf("prob0ToFLW + FLWToProb0 Test\n");
  for (int i = 0; i < 10; i++) {
    float prob0 = probSet[i];
    printf("-----------\nProbability: %f\n", prob0);

    for (int j = 0; j < 10; j++) {
      int pBits = precisionSet[j];
      printf("Bits: %d", pBits);

      int xpFLW = prob0ToFLW(prob0, pBits);
      printf(" -> %d", xpFLW);

      float xpProb = FLWToProb0(xpFLW, pBits);
      printf(" -> %f", xpProb);

      int realFLW = prob0ToFLW(xpProb, pBits);
      printf(" -> %d", realFLW);

      float realProb = FLWToProb0(realFLW, pBits);
      printf(" -> %f\n", realProb);
    }
    printf("---------------------------------------\n");
  }
}
