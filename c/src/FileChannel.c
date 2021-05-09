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
  fflush(object.file);
  object.descriptor = fileno(object.file);
  fstat(object.descriptor, &object.stat);
  object.position = (off_t) 0;
  return object;
}

off_t fcGetPosition(FileChannel *object) {
  object->position = lseek(object->descriptor, 0, SEEK_CUR);
  return(object->position);
}

void fcPosition(FileChannel *object, long long position) {
  object->position = lseek(object->descriptor, position, SEEK_SET);
}

long long fcSize(FileChannel *object) {
  fstat(object->descriptor, &object->stat);
  return(object->stat.st_size);
}

/**
 * Exact equivalent of Java FileChannel.fcTransferFrom
 * Kernel operation: does not transfer data through user space
 */
long long fcTransferFrom(FileChannel *object, FileChannel *src, off_t *position, long long count) {
  return(sendfile(object->descriptor, src->descriptor, position, count));
}

long long fcRead(FileChannel *object, ByteBuffer src, off_t position) {
  return(pread(object->descriptor, src.array, src.length, position));
}

signed char fcRead1B(FileChannel *object, ByteBuffer src, off_t position) {
  if (object->position != position) {
    object->position = position;
    fseek(object->file, position, SEEK_SET);
  }
  object->position++;
  src.array[0] = getc(object->file);
  return(src.array[0]);
}

long long fcWrite(FileChannel *object, ByteBuffer src, off_t position, long long count) {
  long long bytesWritten = write(object->descriptor, src.array + position, count);
  return(bytesWritten);
}

void fcClose(FileChannel *object) {
  fclose(object->file);
}
