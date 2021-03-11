#include <stdlib.h>
#include <stdio.h>
#include "StreamGenerator.h"

void sgGenerate(StreamGenerator object, int fileSize, int count) {
  for (int i = 0; i < count; ++i) {
    char tmpFileName[count];
    sprintf(tmpFileName, "%s%d%s", object.path, i, object.extension);
    FILE *tmpFile = fopen(tmpFileName, "w");
    for (int j = 0; j < fileSize; ++j) {
      fprintf(tmpFile, "%c", (unsigned char) (rand() % 255));
    }
    fclose(tmpFile);
  }
}