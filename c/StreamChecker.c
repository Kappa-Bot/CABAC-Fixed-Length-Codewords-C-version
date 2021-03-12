#include "stdio.h"
#include "StreamChecker.h"

StreamChecker scConfig(char *fileName1, char *fileName2) {
  StreamChecker object;

  object.file1 = fopen(fileName1, "r");
  object.file2 = fopen(fileName2, "r");

  fseek(object.file1, 0L, SEEK_END);
  fseek(object.file2, 0L, SEEK_END);

  long size1 = ftell(object.file1);
  long size2 = ftell(object.file2);

  fseek(object.file1, 0L, SEEK_SET);
  fseek(object.file2, 0L, SEEK_SET);

  object.size = size1 == size2 ? size1 : 0;

  return object;
}


int scCheckEquals(StreamChecker object) {
  fseek(object.file1, 0L, SEEK_SET);
  fseek(object.file2, 0L, SEEK_SET);

  int verif = 0;

  if (object.size) {
    verif = 1;

    char c1 = 0;
    char c2 = 0;

    for (int i = 0; (i < object.size) && verif; ++i) {
      c1 = fgetc(object.file1);
      c2 = fgetc(object.file2);

      verif = c1 == c2;
    }
  }

  fseek(object.file1, 0L, SEEK_SET);
  fseek(object.file2, 0L, SEEK_SET);

  return verif;
}

long scCheckError(StreamChecker object) {
  fseek(object.file1, 0L, SEEK_SET);
  fseek(object.file2, 0L, SEEK_SET);

  long error = object.size;

  if (object.size) {
    error = -1;

    char c1 = 0;
    char c2 = 0;

    for (int i = 0; (i < object.size) && (error == -1); ++i) {
      c1 = fgetc(object.file1);
      c2 = fgetc(object.file2);

      error = c1 == c2 ? error : i;
    }
  }

  fseek(object.file1, 0L, SEEK_SET);
  fseek(object.file2, 0L, SEEK_SET);

  return error;
}

void scFinish(StreamChecker object) {
  fclose(object.file1);
  fclose(object.file2);
}
