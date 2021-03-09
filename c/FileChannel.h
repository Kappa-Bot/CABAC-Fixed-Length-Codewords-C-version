#ifndef FC_HH
#define FC_HH

#include <assert.h>
#include "ByteArray.h"

struct BA_s {
  unsigned char *array;
  int length;
};
typedef struct BA_s ByteArray;

struct FC_s {
  FILE *file;
};
typedef struct FC_s FileChannel;

long fcGetPosition(FileChannel *object);
void fcPosition(FileChannel *object, long position);
long fcSize(FileChannel *object);
void transferFrom(FileChannel *object, FileChannel *src, long position, long count);
void fcRead(FileChannel *object, ByteArray src, long position);
void fcWrite(FileChannel *object, ByteArray src);
void fcCLose(FileChannel *object);

long fcGetPosition(FileChannel *object) {
  assert(object->file != NULL);
  return ftell(object->file);
}

void fcPosition(FileChannel *object, long position) {
  assert(object->file != NULL && position >= 0);
  fseek(object->file, position, SEEK_SET);
}

long fcSize(FileChannel *object) {
  assert(object->file != NULL);
  long position = fcGetPosition(object);
  fseek(object->file, 0L, SEEK_END);
  long size = fcGetPosition(object);
  fcPosition(object, position);
  return size;
}

void fcTransferFrom(FileChannel *object, FileChannel *src, long position, long count) {
  assert(position >= 0 && count >= 0);
  long pos = fcGetPosition(object);
  fcPosition(object, position);
  char *data = "";
  fread(data, 1, count, src->file);
  fwrite(data, 1, count, object->file);
  fcPosition(object ,pos);
}

void fcRead(FileChannel *object, ByteArray src, long position) {
  assert(position >= 0);
  long limit = (fcSize(object) - position);
  limit = limit > src.length ? src.length : limit;
  fread(src.array, 1, limit, object->file);
}

void fcWrite(FileChannel *object, ByteArray src) {
  fwrite(src.array, 1, src.length, object->file);
}

void fcClose(FileChannel *object) {
  fclose(object->file);
}

#endif /* FC_HH */
