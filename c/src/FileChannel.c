#include <sys/sendfile.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <unistd.h>

#ifndef FC_HH
#include "FileChannel.h"
#endif

FileChannel FileChannel_0(char *fileName, char *mode) {
  FileChannel object;
  object.file = fopen(fileName, mode);
  object.descriptor = fileno(object.file);
  fstat(object.descriptor, &object.stat);
  object.position = (off_t) 0;
  return object;
}

off_t fcGetPosition(FileChannel *object) {
  assert(object->file != NULL);
  object->position = ftell(object->file);
  return(object->position);
}

void fcPosition(FileChannel *object, long long position) {
  assert(object->file != NULL && position >= 0);
  object->position = fseek(object->file, position, SEEK_SET);
}

long long fcSize(FileChannel *object) {
  assert(object->file != NULL);
  object->position = ftell(object->file);
  fseek(object->file, 0L, SEEK_END);
  long long size = ftell(object->file);
  fseek(object->file, object->position, SEEK_SET);
  return(size);   // object->stat.st_size
}

/**
 * Exact equivalent of (JAVA SE 7) FileChannel.fcTransferFrom
 * Kernel operation: does not transfer content through user space
 */
long long fcTransferFrom(FileChannel *object, FileChannel *src, off_t *position, long long count) {
  assert(position >= 0 && count >= 0);
  return(sendfile(object->descriptor, src->descriptor, position, count));
}

long long fcRead(FileChannel *object, ByteBuffer src, off_t position) {
  assert(position <= src.array.length && position >= 0);
  return(pread(object->descriptor, src.array, src.length, position));
}

signed char fcRead1B(FileChannel *object, ByteBuffer src, off_t position) {
  assert(position <= src.array.length && position >= 0);

  object->position++;
  if (object->position != position) {
    object->position = position;
    fseek(object->file, object->position, SEEK_SET);
  }

  signed char readByte = src.array[0] = getc(object->file);
  return readByte;
}

long long fcWrite(FileChannel *object, ByteBuffer src, off_t position, long long count) {
  assert(position <= src.array.length && position >= 0 && count <= src.length - position);
  object->position += count;
  return(write(object->descriptor, src.array + position, count));
}

void fcClose(FileChannel *object) {
  fclose(object->file);
}
