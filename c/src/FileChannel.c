#include <sys/mman.h>
#include <sys/sendfile.h>
#include <sys/types.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#ifndef FC_HH
#include "FileChannel.h"
#endif

/**
 * Initialize a new FileChannel by setting a file stream, file stat
 * and a memory-mapped region for optimal file reading purposes.
 *
 * @param fileName: the path to the target file
 * @param mode: file opening mode ("r" | "w")
 *
 * returns: the new constructed FileChannel object
 */
FileChannel FileChannel_0(char *fileName, char *mode) {
  FileChannel object;
  object.position = (off_t) 0;
  object.file = fopen(fileName, mode);
  object.descriptor = fileno(object.file);
  fstat(object.descriptor, &object.stat);

  /*
   * This hurts my eyes, but on machine code, this should
   * get replaced with a conditional move
   */
  if (strcmp(mode, FC_READ) == 0) { // Avoid unnecessary overhead in write mode
    object.buffer = (signed char *) mmap(NULL, object.stat.st_size,
      PROT_READ, MAP_SHARED, object.descriptor, 0);
  } else {
    object.buffer = 0;
  }

  return object;
}

/**
 * Gets the actual position of the file, the lseek() system call
 * returns the real gotten offset, so setting and offset 0 from the
 * current file position will give the actual position.
 * The function ftell(object->file) has way more overhead than this call.
 *
 * returns: the updated position of the file.
 */
off_t fcGetPosition(FileChannel *object) {
  object->position = lseek(object->descriptor, 0, SEEK_CUR);
  return(object->position);
}

/**
 * Positions to a given absolute offset of the file.
 * The available positioning modes are:
 *  - SEEK_CUR: from the current file position
 *  - SEEK_END: from the end of the file (reverse offset)
 *  - SEEK_SET: from the beginning of the file
 *
 * @param position: the desired absolute offset
 */
void fcPosition(FileChannel *object, long long position) {
  object->position = lseek(object->descriptor, position, SEEK_SET);
}

/**
 * Updates the stat of the FileChannel
 * This is useful when the object has been opened in write mode
 * and a file stat update is needed when an expansion occurs.
 *
 * returns: size of the file, obtained from the struct stat object->stat
 */
long long fcSize(FileChannel *object) {
  fstat(object->descriptor, &object->stat);
  return(object->stat.st_size);
}

/**
 * Exact equivalent of Java FileChannel.transferFrom
 * Kernel operation: does not transfer data through user space
 *
 * @param object: FileChannel (write) to attempt the transfer to
 * @param src: FileChannel (read) where the data comes from
 * @param position: offset of FileChannel src where the transfer has to begin
 * @param count: the number bytes to attempt
 *
 * returns: number of bytes transfered, depending on the min file size
 */
long long fcTransferFrom(FileChannel *object, FileChannel *src, off_t *position, long long count) {
  return(sendfile(object->descriptor, src->descriptor, position, count));
}

/**
 * Reads an amount of bytes from the given file position. In this mode (read),
 * A memory-mapped buffer corresponding to the file  is used and it behaves
 * as any other dynamic array. Since memcpy is the most efficient way in C
 * to transfer content, Java should do something like pointer swapping to avoid copying (tricky).
 *
 * returns: number of bytes read = min length between the file and ByteBuffer src
 */
long long fcRead(FileChannel *object, ByteBuffer src, off_t position) {
  long long limit = src.length > (object->stat.st_size - position) ? (object->stat.st_size - position) : src.length;
  memcpy(src.array, object->buffer + position, limit);
  return limit;
}

/*
 * Efficient alternative of fcRead to read a single byte.
 * This is called in the hotspot of the module ByteStream (readFile mode).
 *
 * returns: number of bytes read = 1
 */
signed char fcRead1B(FileChannel *object, ByteBuffer src, off_t position) {
  src.array[0] = object->buffer[position];
  return(1);
}

/*
 * Writes an amount of bytes from a ByteBuffer to this FileChannel
 *
 * @param src: ByteBuffer where the data comes from
 * @param position: position of the ByteBuffer where the write has to begin
 * @param count: the number of bytes to write
 *
 * returns: real number of bytes written
 */
long long fcWrite(FileChannel *object, ByteBuffer src, off_t position, long long count) {
  return(write(object->descriptor, src.array + position, count));
}

/*
 * Closes the FileChannel by closing the file stream. If it has been
 * opened in read mode, an unmap of the buffer is needed.
 */
void fcClose(FileChannel *object) {
  if (object->buffer != 0) munmap(object->buffer, object->stat.st_size);
  fclose(object->file);
}
