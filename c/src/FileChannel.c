#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/sendfile.h>
#include <sys/types.h>

#ifndef FC_HH
#include "FileChannel.h"
#endif

FileChannel FileChannel_0(char *fileName, char *mode) {
  FileChannel object;
  object.file = fopen(fileName, mode);
  object.descriptor = fileno(object.file);
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

/**
 * Exact equivalent of (JAVA SE 7) FileChannel.fcTransferFromKernel transfer
 * Does not transfer content through memory, its a kernel operation
 */
long fcTransferFrom(FileChannel object, FileChannel src, long position, long count) {
  assert(position >= 0 && count >= 0);
  return sendfile(object.descriptor, src.descriptor, &position, count);
}

long fcRead(FileChannel object, ByteBuffer src, long position) {
  assert(position <= src.array.length && position >= 0);

  long limit = fcSize(object) - position;
  if (limit < 0) return -1;
  limit = limit >= src.length ? src.length : limit;

  long prevPosition = fcGetPosition(object);
  fcPosition(object, position);
  long bytesRead = fread(src.array, 1, limit, object.file);
  fcPosition(object, prevPosition);

  return bytesRead;
}

void fcWrite(FileChannel object, ByteBuffer src, long position, long count) {
  assert(position <= src.array.length && position >= 0 && count <= src.length - position);
  fwrite(src.array + position, 1, count, object.file);
}

void fcClose(FileChannel object) {
  fclose(object.file);
}
