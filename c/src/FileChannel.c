#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#ifndef FC_HH
#include "FileChannel.h"
#endif

FileChannel fcConfig(char *fileName, char *mode) {
  FileChannel object;
  object.file = fopen(fileName, mode);
  return object;
}

long fcGetPosition(FileChannel object) {
  assert(object.file != NULL);

  return ftell(object.file);
}

void fcPosition(FileChannel object, long position) {
  assert(object.file != NULL && position >= 0);

  fseek(object.file, position, SEEK_SET);
}

long fcSize(FileChannel object) {
  assert(object.file != NULL);

  long position = fcGetPosition(object);
  fseek(object.file, 0L, SEEK_END);
  long size = fcGetPosition(object);
  fcPosition(object, position);

  return size;
}

void fcTransferFrom(FileChannel object, FileChannel src, long position, long count) {
  assert(position >= 0 && count >= 0);

  fcPosition(src, position);

  char *data = (char *) malloc(count);

  fread(data, 1, count, src.file);
  fwrite(data, 1, count, object.file);

  free(data);
}

void fcRead(FileChannel object, ByteBuffer src, long position) {
  assert(position >= 0);

  long limit = (fcSize(object) - position);
  limit = limit > src.length ? src.length : limit;

  fread(src.array, 1, limit, object.file);
}

void fcWrite(FileChannel object, ByteBuffer src) {
  fwrite(src.array, 1, src.length, object.file);
}

void fcClose(FileChannel object) {
  fclose(object.file);
}
