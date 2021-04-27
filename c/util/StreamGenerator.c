#include <stdlib.h>
#include <stdio.h>
#include "StreamGenerator.h"

StreamGenerator sgConfig(char *path, char *ext) {
  StreamGenerator object;

  object.path = path;
  object.extension = ext;

  return object;
}

void sgGenerate(StreamGenerator object, int fileSize, int count) {
  for (int i = 0; i < count; ++i) {
    char tmpFileName[count + 5];
    sprintf(tmpFileName, "%ssg_%d%s", object.path, i, object.extension);

    FILE *tmpFile = fopen(tmpFileName, "w");

    for (int j = 0; j < fileSize; ++j) {
      fprintf(tmpFile, "%c", (unsigned char) (rand() % 255));
    }

    fclose(tmpFile);
  }
}
