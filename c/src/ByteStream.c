#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h>

#ifndef BS_HH
    #include "ByteStream.h"
#endif

ByteStream *ByteStreamInit() {
  ByteStream *object = (ByteStream *) malloc(sizeof(ByteStream));
  memcpy(object, &BS_default, sizeof(ByteStream));
  return object;
}

struct BS_s BS_default = {                  // Default values for initial "instance"
  -1,                             // streamMode
  {NULL, 0},                      // buffer
  0,                              // limit
  0,                              // position
  {NULL},                         // readFileChannel
  -1,                             // readFileNumSegments
  {NULL, 0},                      // readFileSegments
  -1,                             // temporalFilePosition
  NULL,                           // temporalFileName
  {NULL, 0}                       // temporalBuffer
                                  // lock (no need to initialize here)
};

/**
 * Allocates memory for the buffer. The stream enters in the normal mode.
 */
ByteStream *ByteStream_0() {
  ByteStream *object = ByteStreamInit();
  object->buffer.array = (signed char *) malloc(INITIAL_ALLOCATION);
  object->buffer.length = INITIAL_ALLOCATION;
  object->streamMode = 0;
  #ifdef _OPENMP
    omp_init_lock(&object->lock);
  #endif
  return object;
}

/**
 * Allocates memory for the buffer, with a specified limit. The stream enters in the normal mode.
 *
 * @param initialAllocation number of bytes reserved in the buffer
 */
ByteStream *ByteStream_1(int initialAllocation) {
  ByteStream *object = ByteStreamInit();
  object->buffer.array = (signed char *) malloc(initialAllocation);
  object->buffer.length = initialAllocation;
  object->streamMode = 0;
  #ifdef _OPENMP
    omp_init_lock(&object->lock);
  #endif
  return object;
}

/**
 * Initializes this stream with a file from which bytes are read. The stream enters in
 * the readFile mode.
 *
 * @param fc the file from which bytes are read
 */
ByteStream *ByteStream_2(FileChannel fc) {
  ByteStream *object = ByteStreamInit();
  object->temporalBuffer.array = (signed char *) malloc(1);
  object->temporalBuffer.length = 1;
  object->readFileChannel = fc;
  object->readFileNumSegments = 0;
  // object->readFileSegments.array = (long long **) malloc(INITIAL_NUM_SEGMENTS * 2 * sizeof(long long));

  object->readFileSegments.array = (long long **) malloc(INITIAL_NUM_SEGMENTS * sizeof(long long *));
  object->readFileSegments.length = INITIAL_NUM_SEGMENTS;
  for (int i = 0; i < INITIAL_NUM_SEGMENTS; ++i) {
    object->readFileSegments.array[i] = (long long *) malloc(2 * sizeof(long long));
  }


  object->streamMode = 1;
  #ifdef _OPENMP
    omp_init_lock(&object->lock);
  #endif
  return object;
}

/**
 * Puts a byte into the stream. This function can only be called when the stream is in
 * normal mode.
 *
 * @param b the added byte
 */
void putByte(ByteStream *object, signed char b) {
  assert(object->streamMode == 0);
  if(object->limit == object->buffer.length) {
    signed char *bufferTMP = (signed char *) malloc(object->buffer.length * 2);
    memcpy(bufferTMP, object->buffer.array, object->limit);
    free(object->buffer.array);
    object->buffer.array = bufferTMP;

    // Optimization: reduces memory paging and copying
    // object->buffer.array = realloc(object->buffer.array, object->buffer.length * 2);

    object->buffer.length = object->buffer.length * 2;
  }
  object->buffer.array[object->limit] = b;
  object->limit++;
}

/**
 * Puts one or more bytes into the stream. This function can only be called when the stream
 * is in normal mode.
 *
 * @param array the array of bytes that will be added
 * @param offset first position of the array that is added
 * @param length number of bytes added
 */
void putBytes_0(ByteStream *object, ByteBuffer array, int offset, int length) {
  assert(object->streamMode == 0);
  assert((offset >= 0) && (length > 0) && (offset + length <= array.length));
  if(object->limit + (long long) length > (long long) object->buffer.length) {
    signed char *bufferTMP = (signed char*) malloc((object->buffer.length + length) * 2);
    memcpy(bufferTMP, object->buffer.array, object->limit);
    free(object->buffer.array);
    object->buffer.array = bufferTMP;

    // Optimization: reduces memory paging and cloning
    // object->buffer.array = realloc(object->buffer.array, (object->buffer.length + length) * 2);
    object->buffer.length = (object->buffer.length + length) * 2;
  }
  memcpy(object->buffer.array + object->limit, array.array + offset, length);
  object->limit += (long long) length;
}

/**
 * Puts one or more bytes into the stream. This function can only be called when the
 * stream is in normal mode.
 *
 * @param num integer containing the bytes that will be added
 * @param numBytes number of bytes added
 */
void putBytes_1(ByteStream *object, int num, int numBytes) {
  assert(object->streamMode == 0);
  assert((numBytes > 0) && (numBytes <= 4));
  for(int b = numBytes - 1; b >= 0; b--) {
    signed char addedByte = (signed char) ((num >> (8 * b)) & 0x000000FF);
    putByte(object, addedByte);
  }
}

/**
 * Puts one file segment containing some bytes into the stream. This function can only be
 * called when the stream is in readFile mode.
 *
 * @param begin position of the first byte of the file that is added to the stream
 * @param length length of the segment
 */
void putFileSegment(ByteStream *object, long long begin, long long length) {
  assert(object->streamMode == 1);
  assert(object->readFileChannel.file != NULL);
  assert((begin >= 0) && (length > 0));
  assert(begin + length <= fcSize(&object->readFileChannel));
  assert(object->readFileNumSegments <= object->readFileSegments.length);

  if(object->readFileNumSegments == object->readFileSegments.length) {

    long long **fcSegmentsTMP = (long long **) malloc(object->readFileSegments.length * 2 * sizeof(long long *));
    for (int segment = 0; segment < object->readFileSegments.length * 2; ++segment) {
      fcSegmentsTMP[segment] = (long long *) malloc(2 * sizeof(long long));
    }

    for(int segment = 0; segment < object->readFileNumSegments; segment++) {
      fcSegmentsTMP[segment][0] = object->readFileSegments.array[segment][0];
      fcSegmentsTMP[segment][1] = object->readFileSegments.array[segment][1];
    }
    //free(fcSegmentsTMP);

    // Temporal code
    free(object->readFileSegments.array);
    object->readFileSegments.array = fcSegmentsTMP;
    object->readFileSegments.length = object->readFileSegments.length * 2;
  }
  object->readFileSegments.array[object->readFileNumSegments][0] = begin;
  object->readFileSegments.array[object->readFileNumSegments][1] = length;
  object->readFileNumSegments++;
  object->limit += length;
}

/**
 * Removes one byte from the end of the stream. This function can only be called when the
 * stream is in normal mode.
 */
void removeByte(ByteStream *object) {
  assert(object->streamMode == 0);
  object->limit = (object->limit - 1 <= 0) ? 0: (object->limit - 1);
  // object->limit = (object->limit - 1) * (object->limit > 1);
  // object->position = object->position > object->limit ? object->limit : object->position;
  if(object->position > object->limit) {
    object->position = object->limit;
  }
}

/**
 * Removes one or more bytes from the end of the stream. This function can only be called
 * when the stream is in normal mode.
 *
 * @param num number of removed bytes
 */
void removeBytes(ByteStream *object, int num) {
  assert(object->streamMode == 0);
  object->limit = (object->limit - num) <= 0 ? 0: (object->limit - num);
  // object->limit = (object->limit - num) * ((object->limit - num) > 0);
  // object->position = object->position > object->limit ? object->limit : object->position;
  if(object->position > object->limit) {
    object->position = object->limit;
  }
}

/**
 * Gets the byte in the current position. This function can only be called when the stream
 * is in normal or readFile mode.
 *
 * @return the corresponding byte
 */
signed char getByte_0(ByteStream *object) {
  assert((object->streamMode == 0) || (object->streamMode == 1));
  assert(object->position >= 0);

  signed char getByte = getByte_1(object, object->position);
  object->position++;
  return(getByte);
}

/**
 * Gets the indicated byte. This function can only be called when the stream is in
 * normal or readFile mode.
 *
 * @param index the index of the byte (starting from 0)
 * @return the corresponding byte
 */
signed char getByte_1(ByteStream *object, long long index) {
  assert((object->streamMode == 0) || (object->streamMode == 1));
  assert(index >= 0 && index <= object->limit);

  signed char getByte = 0;
  if(object->streamMode == 0) {
    getByte = object->buffer.array[index];
  } else if(object->streamMode == 1) {
    //Determines the segment in which this index lies
    int segment = 0;
    long long accBytes = 0;
    while(index >= accBytes + object->readFileSegments.array[segment][1]) {
      accBytes += object->readFileSegments.array[segment][1];
      segment++;
    }
    /*
     * Optimization: faster way to search where the index lies
    while (index >= object->readFileSegments.array[segment][0]) { segment++; }
    long long fcPosition = index
                - object->readFileSegments.array[segment][1])
     */
    assert(segment < object->readFileNumSegments);
    //Determines the position in the file
    long long fcPosition = index - accBytes
                + object->readFileSegments.array[segment][0];
    assert(fcPosition < object->readFileChannel.stat.st_size);

    //Gets the byte
    fcRead1B(&object->readFileChannel, object->temporalBuffer, fcPosition);
    getByte = object->temporalBuffer.array[0];
  }
  return(getByte);
}

/**
 * Gets the bytes in the current position, and places them to an integer. This function
 * can only be called when the stream is in normal or readFile mode.
 *
 * @param numBytes number of bytes read
 * @return the integer having the corresponding bytes
 */
int getBytes(ByteStream *object, int numBytes) {
  assert((object->streamMode == 0) || (object->streamMode == 1));
  int num = 0;
  for(int b = numBytes - 1; b >= 0; b--) {
    signed char getByte = getByte_0(object);
    for(int i = 7; i >= 0; i--) {
      int bitMask = 1 << i;
      num += (getByte & bitMask) << (b * 8);
    }
  }
  return(num);
}

/**
 * Checks whether <code>get</code> functions can get more bytes. This function can only
 * be called when the stream is in normal or readFile mode.
 *
 * @return true when there are one or more bytes
 */
int hasMoreBytes(ByteStream *object) {
  assert((object->streamMode == 0) || (object->streamMode == 1));
  return(object->position < object->limit);
}

/**
 * Returns the array of bytes. The length of the array is equal or greater than
 * the bytes put in the stream. This function can only be called when the stream is in normal mode.
 *
 * @return the byte array
 */
ByteBuffer getByteStream(ByteStream *object) {
  assert(object->streamMode == 0);
  return(object->buffer);
}

/**
 * Returns the number of bytes in the stream.
 *
 * @return number of bytes
 */
long long getLength(ByteStream *object) {
  return(object->limit);
}

/**
 * Returns the current position in the stream.
 *
 * @return {@link #position}
 */
long long getPosition(ByteStream *object) {
  return(object->position);
}

/**
 * Clears the stream removing all bytes in it.
 */
void clear(ByteStream *object) {
  object->limit = 0;
  object->position = 0;
}

/**
 * Sets the position to the first byte.
 */
void ByteStream_reset(ByteStream *object) {
  object->position = 0;
}

/**
 * Advances the position of the stream in the specified number of bytes.
 *
 * @param numBytes number of skipped bytes (can be positive or negative)
 */
void skip(ByteStream *object, long long numBytes) {
  if(numBytes >= 0) {
    if(object->position + numBytes > object->limit) {
      object->position = object->limit;
    } else {
      object->position += numBytes;
    }
  } else {
    if(object->position + numBytes < 0) {
      object->position = 0;
    } else {
      object->position += numBytes;
    }
  }
}

/**
 * Ends the readFile mode putting all segments of the file in the real byte buffer,
 * entering in the normal mode. This stream can return to the readFile mode
 * calling <code>returnReadFileMode</code>.
 *
 */
void endReadFileMode(ByteStream *object) {
  assert(object->streamMode == 1);

  object->buffer.array = malloc(object->limit);
  object->buffer.length = object->limit;
  int readBytes = 0;
  for(int segment = 0; segment < object->readFileNumSegments; segment++){
    long long begin = object->readFileSegments.array[segment][0];
    long long length = object->readFileSegments.array[segment][1];

    object->temporalBuffer.array = malloc(length);
    object->temporalBuffer.length = length;

    fcRead(&object->readFileChannel, object->temporalBuffer, begin);
    memcpy(object->buffer.array + readBytes, object->temporalBuffer.array, length);

    readBytes += length;
  }
  object->streamMode = 0;
}

/**
 * Returns a stream that was created in readFile mode and that was converted to the normal
 * mode afterwards to the readFile mode again. Attention! Be aware that once this function
 * is called, the stream returns to the state left when the function <code>endReadFileMode</code> was
 * called. This means that ALL changes (such as deleting or adding bytes) that were carried out while
 * the stream was in normal mode are lost.
 */
void returnReadFileMode(ByteStream *object) {
  assert(object->streamMode == 0);
  assert(object->readFileChannel.file != NULL);
  assert(object->readFileNumSegments != -1);
  assert(object->readFileSegments.array != NULL);

  free(object->buffer.array);
  object->buffer.length = 0;
  object->limit = 0;

  for(int segment = 0; segment < object->readFileNumSegments; segment++) {
    object->limit += object->readFileSegments.array[segment][1];
  }
  if(object->position >= object->limit) {
    object->position = object->limit -1;
  }

  object->streamMode = 1;
}

/**
 * Removes unnecessary bytes allocated for this buffer. This function can only be called
 * when the stream is in normal mode.
 */
void packetize(ByteStream *object) {
  assert(object->streamMode == 0);

  signed char *bufferTMP = (signed char *) malloc(object->limit);
  memcpy(bufferTMP, object->buffer.array, object->limit);
  free(object->buffer.array);

  // Optimization (cold-operation): reduces memory and paging operations
  // object->buffer.array = realloc (object->buffer.array, object->limit);
  object->buffer.array = bufferTMP;
  object->buffer.length = object->limit;
}

/**
 * Returns whether this stream is in normal mode or not.
 *
 * @return true when it is
 */
int isInReadNormalMode(ByteStream *object) {
  return(object->streamMode == 0);
}

/**
 * Returns whether this stream is in readFile mode or not.
 *
 * @return true when it is
 */
int isInReadFileMode(ByteStream *object) {
  return(object->streamMode == 1);
}

/**
 * Returns whether this stream is in a temporal file or not.
 *
 * @return true when it is
 */
int isInTemporalFile(ByteStream *object) {
  return(object->streamMode == 2);
}

/**
 * Writes the stream to a file. This function can only be called when the stream is in
 * normal or readFile mode.
 *
 * @param fos file in which is written
 */
void write_0(ByteStream *object, FileChannel fc) {
  assert((object->streamMode == 0) || (object->streamMode == 2));
  write_1(object, fc, 0, object->limit);
}

/**
 * Writes a segment of the stream to a file. This function can only be called when the
 * stream is in normal or readFile mode.
 *
 * @param fos file in which is written
 * @param begin first byte of the stream written
 * @param length length written
 */
void write_1(ByteStream *object, FileChannel outputFC, long long begin, long long length) {
  assert((object->streamMode == 0) || (object->streamMode == 2));
  assert(begin + length <= object->limit);
  if(!isInTemporalFile(object)) {
    fcWrite(&outputFC, object->buffer, begin, length);
  } else {
    FileChannel streamFC = FileChannel_0(object->temporalFileName, "r");

    long long bytesWritten = 0;
    off_t positionOutputFC = fcGetPosition(&outputFC);
    fcPosition(&streamFC, begin + object->temporalFilePosition);
    while(bytesWritten < length) {
      bytesWritten += fcTransferFrom(&outputFC, &streamFC, &positionOutputFC, length - bytesWritten);
      positionOutputFC += bytesWritten;
      fcPosition(&streamFC, begin + object->temporalFilePosition + bytesWritten);
    }
    fcPosition(&outputFC, positionOutputFC);
    fcClose(&streamFC);
  }
}

/**
 * Saves the stream to a file and frees allocated memory. Before using the stream again,
 * the function <code>loadFromTemporalFile</code> must be called. This function can only be
 * called when the stream is in normal mode.
 *
 * @param temporalDirectory temporal directory in which the file is saved
 */
void saveToTemporalFile(ByteStream *object, char *temporalDirectory) {
  assert(object->streamMode == 0);

  int verif = temporalDirectory[strlen(temporalDirectory) - 1] == '/';

  #ifdef _OPENMP
  omp_set_lock(&object->lock);
  #endif

  //Checks whether is the first time an object of this class is saved to the temporal file
  if(object->temporalFileName == NULL) {
    // This size because... Why not
    object->temporalFileName = (char *) malloc(strlen(temporalDirectory) + 21 + verif);
    // TMP-728858622658.tmp
    do {
      //memset(object->temporalFileName, 0, strlen(object->temporalFileName));
      sprintf(object->temporalFileName, "%s%s%012d.tmp",
          temporalDirectory, verif ? "/TMP-" : "TMP-", rand());
    } while(!access(object->temporalFileName, F_OK));
  }

  FileChannel fc = FileChannel_0(object->temporalFileName, "w");

  object->temporalFilePosition = fcGetPosition(&fc);
  fcWrite(&fc, object->buffer, 0, object->limit);
  fcClose(&fc);

  #ifdef _OPENMP
  omp_unset_lock(&object->lock);
  #endif

  if (object->buffer.array > 0) {
    free(object->buffer.array);
  }
  object->buffer.length = -1; // Useful for destroy()

  object->streamMode = 2;
}

/**
 * Loads the stream from the file in which it was saved. Data of the stream is not deleted
 * from the file. This function can only be called when the stream is in temporalFile mode.
 *
 */
void loadFromTemporalFile(ByteStream *object) {
  assert(object->streamMode == 2);

  object->buffer.array = malloc(object->limit);
  object->buffer.length = object->limit;

  FileChannel fc = FileChannel_0(object->temporalFileName, "r");

  fcPosition(&fc, object->temporalFilePosition);
  fcRead(&fc, object->buffer, 0);
  fcClose(&fc);
  object->temporalFilePosition = -1;
  object->streamMode = 0;
}

/**
 * Frees the memory occupied by this stream. Call this function when this object is no
 * long longer needed.
 */
void destroy(ByteStream *object) {
  if(object->buffer.length > 0) { // normal | (readFile + end() + ...)
    free(object->buffer.array);
  }
  if(object->streamMode == 1) { // readFile
    free(object->temporalBuffer.array);
    for(int i = 0; i < object->readFileSegments.length; ++i) {
      free(object->readFileSegments.array[i]);
    }
    free(object->readFileSegments.array);
    fcClose(&object->readFileChannel);
  }
  #ifdef _OPENMP
    omp_destroy_lock(&object->lock);
  #endif
}

/**
 * Destroys the temporal file shared by all objects of this class. Call this function only
 * when objects of this class are no long longer needed.
 */
void destroyTemporalFile(ByteStream *object) {
  if(object->temporalFileName != NULL) {
    int verif = remove(object->temporalFileName);
    assert(verif == 0);
    if (verif == 0) object->temporalFileName = NULL;
  }
}

/**
 * Computes the memory employed to keep the segments of the stream in the file, when the
 * stream is in file read mode. This function is for debugging purposes.
 *
 * @return the number of bytes employed
 */
int getMemorySegments(ByteStream *object) {
  int memory = 0;
  // if(object->streamMode == 0) {
  // Temporal Code
  if(object->streamMode == 1) {
    memory = object->readFileSegments.length * 2 * sizeof(long long);
  }
  return(memory);
}
