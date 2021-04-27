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
  object.position = 0;
  return object;
}

long fcGetPosition(FileChannel object) {
  assert(object.file != NULL);
  object.position = ftell(object.file);
  return(object.position);
}

void fcPosition(FileChannel object, long position) {
  assert(object.file != NULL && position >= 0);
  object.position = lseek(object.descriptor, position, SEEK_SET);
}

long fcSize(FileChannel object) {
  assert(object.file != NULL);

  object.position = ftell(object.file);
  lseek(object.descriptor, 0L, SEEK_END);
  long size = ftell(object.file);
  lseek(object.descriptor, object.position, SEEK_SET);

  return(size);
}

/**
 * Exact equivalent of (JAVA SE 7) FileChannel.fcTransferFromKernel transfer
 * Does not transfer content through memory, its a kernel operation
 */
long fcTransferFrom(FileChannel object, FileChannel src, long position, long count) {
  assert(position >= 0 && count >= 0);
  return(sendfile(object.descriptor, src.descriptor, &position, count));
}

long fcRead(FileChannel object, ByteBuffer src, long position) {
  assert(position <= src.array.length && position >= 0);
  lseek(object.descriptor, position, SEEK_SET);
  long bytesRead = read(object.descriptor, src.array, src.length);
  lseek(object.descriptor, object.position, SEEK_SET);
  return(bytesRead);
}

long fcRead1B(FileChannel object, ByteBuffer src, long position) {
  assert(position <= src.array.length && position >= 0);
  fseek(object.file, position, SEEK_SET);
  long byteRead = fread(src.array, 1, 1, object.file);
  return(byteRead);
}

long fcWrite(FileChannel object, ByteBuffer src, long position, long count) {
  assert(position <= src.array.length && position >= 0 && count <= src.length - position);
  return(write(object.descriptor, src.array + position, count));
}

void fcClose(FileChannel object) {
  fclose(object.file);
}
