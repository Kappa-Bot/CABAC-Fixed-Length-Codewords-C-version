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
  '\0',                           // temporalFileName
  {NULL, 0}                       // temporalBuffer
  // NULL               // lock
};

/**
 * Allocates memory for the buffer. The stream enters in the normal mode.
 */
ByteStream *ByteStream_0() {
  ByteStream *object = ByteStreamInit();
  object->buffer.array = (unsigned char *) malloc(INITIAL_ALLOCATION);
  object->buffer.length = INITIAL_ALLOCATION;
  object->streamMode = 0;
  return object;
}

/**
 * Allocates memory for the buffer, with a specified limit. The stream enters in the normal mode.
 *
 * @param initialAllocation number of bytes reserved in the buffer
 */
ByteStream *ByteStream_1(int initialAllocation) {
  ByteStream *object = ByteStreamInit();
  object->buffer.array = (unsigned char *) malloc(initialAllocation);
  object->buffer.length = initialAllocation;
  object->streamMode = 0;
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
  object->readFileChannel = fc;
  object->readFileNumSegments = 0;
  object->readFileSegments.array = (long **) malloc(INITIAL_NUM_SEGMENTS * 2 * sizeof(long));
  object->readFileSegments.length = INITIAL_NUM_SEGMENTS * 2;
  object->streamMode = 1;
  return object;
}

/**
 * Puts a byte into the stream. This function can only be called when the stream is in
 * normal mode.
 *
 * @param b the added byte
 */
void putByte(ByteStream *object, unsigned char b) {
  assert(object->streamMode == 0);
  if(object->limit == object->buffer.length) {
    unsigned char *bufferTMP = (unsigned char *) malloc(object->buffer.length * 2);
    memcpy(bufferTMP, object->buffer.array, object->limit);
    free(object->buffer.array);
    object->buffer.array = bufferTMP;

    // Optimization: reduces memory paging and cloning
    // object->buffer.array = (unsigned char *) realloc(object->buffer.array, object->buffer.length * 2);

    object->buffer.length = object->buffer.length * 2;
  }
  object->buffer.array[(int) object->limit] = b;
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
  if(object->limit + (long) length > (long) object->buffer.length) {
    unsigned char *bufferTMP = (unsigned char*) malloc((object->buffer.length + length) * 2);
    memcpy(bufferTMP, object->buffer.array, (int) object->limit);
    free(object->buffer.array);
    object->buffer.array = bufferTMP;

    // Optimization: reduces memory paging and cloning
    // object->buffer.array = (unsigned char *) realloc(object->buffer.array, (object->buffer.length + length) * 2);
    object->buffer.length = (object->buffer.length + length) * 2;
  }
  memcpy(object->buffer.array + object->limit, array.array + offset, length);
  object->limit += (long) length;
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
    unsigned char addedByte = (unsigned char) ((num >> (8 * b)) & 0x000000FF);
    putByte(object, addedByte);
  }
}

/**
 * Puts one file segment containing some bytes into the stream. This function can only be
 * called when the stream is in readFile mode.
 *
 * @param begin position of the first byte of the file that is added to the stream
 * @param length length of the segment
 * @throws Exception when some problem introducing the segment occurs
 */
void putFileSegment(ByteStream *object, long begin, long length) {
  assert(object->streamMode == 1);
  assert(object->readFileChannel != NULL);
  assert((begin >= 0) && (length > 0));
  assert(begin + length <= object->readFileChannel.size);
  assert(object->readFileNumSegments <= object->readFileSegments.length);

  if(object->readFileNumSegments == object->readFileSegments.length) {
    long** fcSegmentsTMP = (long **) malloc(object->readFileSegments.length * 2 * 2 * sizeof(long));
    for(int segment = 0; segment < object->readFileNumSegments; segment++) {
      fcSegmentsTMP[segment][0] = object->readFileSegments.array[segment][0];
      fcSegmentsTMP[segment][1] = object->readFileSegments.array[segment][1];
    }
    free(fcSegmentsTMP);
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
  if(object->position > object->limit) {
    object->position = object->limit;
  }
}

/**
 * Gets the byte in the current position. This function can only be called when the stream
 * is in normal or readFile mode.
 *
 * @return the corresponding byte
 * @throws Exception when the end of the stream is reached
 */
unsigned char getByte_0(ByteStream *object) {
  assert((object->streamMode == 0) || (object->streamMode == 1));
  assert(object->position >= 0);

  unsigned char getByte = getByte_1(object, object->position);
  object->position++;
  return(getByte);
}

/**
 * Gets the indicated byte. This function can only be called when the stream is in
 * normal or readFile mode.
 *
 * @param index the index of the byte (starting from 0)
 * @return the corresponding byte
 * @throws Exception when the indicated index is not valid
 */
unsigned char getByte_1(ByteStream *object, long index) {
  assert((object->streamMode == 0) || (object->streamMode == 1));
  assert(index < 0 || index >= object->limit);

  unsigned char getByte = 0;
  if(object->streamMode == 0) {
    getByte = object->buffer.array[(int) index];
  } else if(object->streamMode == 1) {
    //Determines the segment in which this index lies
    int segment = 0;
    long accBytes = 0;
    while(index >= accBytes + object->readFileSegments.array[segment][1]) {
      accBytes += object->readFileSegments.array[segment][1];
      segment++;
    }
    assert(segment < object->readFileNumSegments);

    //Determines the position in the file
    long fcPosition = index - accBytes + object->readFileSegments.array[segment][0];
    assert(fcPosition < fcSize(object->readFileChannel));

    //Gets the byte
    fcRead(&object->readFileChannel, object->temporalBuffer, fcPosition);
    getByte = object->temporalBuffer.array[0];
  } else {
    assert(1 == 0);
  }
  return(getByte);
}

/**
 * Gets the bytes in the current position, and places them to an integer. This function
 * can only be called when the stream is in normal or readFile mode.
 *
 * @param numBytes number of bytes read
 * @return the integer having the corresponding bytes
 * @throws Exception when the end of the stream is reached
 */
int getBytes(ByteStream *object, int numBytes) {
  assert((object->streamMode == 0) || (object->streamMode == 1));
  int num = 0;
  for(int b = numBytes - 1; b >= 0; b--) {
    unsigned char getByte = getByte_0(object);
    for(int i = 7; i >= 0; i--) {
      int bitMask = 1 << i;
      num += ((int) getByte & bitMask) << (b * 8);
    }
  }
  return(num);
}

/**
 * Checks whether <code>get</code> functions can get more bytes. This function can only
 * be called when the stream is in normal or readFile mode.
 *
 * @return true when there are one or more bytes
 * @throws IOException when some problem with the file occurs
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
long getLength(ByteStream *object) {
  return(object->limit);
}

/**
 * Returns the current position in the stream.
 *
 * @return {@link #position}
 */
long getPosition(ByteStream *object) {
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
void skip(ByteStream *object, long numBytes) {
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
 * Returns a stream that was created in readFile mode and that was converted to the normal
 * mode afterwards to the readFile mode again. Attention! Be aware that once this function
 * is called, the stream returns to the state left when the function <code>endReadFileMode</code> was
 * called. This means that ALL changes (such as deleting or adding bytes) that were carried out while
 * the stream was in normal mode are lost.
 */
void returnReadFileMode(ByteStream *object) {
  assert(object->streamMode == 0);
  assert(object->readFileChannel.descriptor != -1);
  assert(object->readFileNumSegments != -1);
  assert(object->readFileSegments.array != NULL);

  free(object->buffer.array);
  object->buffer.array = NULL;
  object->buffer.length = 0;

  object->limit = 0;
  for(int segment = 0; segment < object->readFileNumSegments; segment++) {
    object->limit += (long) ((int) object->readFileSegments.array[segment][1]);
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

  unsigned char *bufferTMP = (unsigned char *) malloc(object->limit);
  memcpy(bufferTMP, object->buffer.array, object->limit);
  free(object->buffer.array);

  // Optimization (cold-operation): reduces memory and paging operations
  // object->buffer.array = (unsigned char *)
  //             realloc (object->buffer.array, object->limit);
  object->buffer.array = bufferTMP;
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
 * Saves the stream to a file and frees allocated memory. Before using the stream again,
 * the function <code>loadFromTemporalFile</code> must be called. This function can only be
 * called when the stream is in normal mode.
 *
 * @param temporalDirectory temporal directory in which the file is saved
 * @throws Exception when some problem writing the file occurs
 */
void saveToTemporalFile(ByteStream *object, char *temporalDirectory) {
  assert(object->streamMode == 0);
  int num = 0;

  // omp_set_lock(lock);
  //Checks whether is the first time an object of this class is saved to the temporal file
  if(object->temporalFileName == NULL) {
    do {
      int verif = temporalDirectory[strlen(temporalDirectory) - 1] == '/';
      sprintf(object->temporalFileName, "%s%s%d.tmp",
            temporalDirectory, verif ? "/TMP-" : "TMP-", num);
      num += 1;
    } while(!access(object->temporalFileName, F_OK));
  }

  FileChannel fc = {fopen(object->temporalFileName, "w")};
  object->temporalFilePosition = fcGetPosition(&fc);
  fcWrite(&fc, object->buffer);
  fcClose(&fc);
  // omp_unset_lock(object->lock);
  free(object->buffer.array);
  object->streamMode = 2;
}

/**
 * Loads the stream from the file in which it was saved. Data of the stream is not deleted
 * from the file. This function can only be called when the stream is in temporalFile mode.
 *
 * @throws Exception when some problem reading the file occurs
 */
void loadFromTemporalFile(ByteStream *object) {
  assert(object->streamMode == 2);

  object->buffer.array = (unsigned char *) malloc((int) object->limit);
  object->buffer.length = object->limit;

  FileChannel fc = {fopen(object->temporalFileName, "r")};
  fcPosition(&fc, object->temporalFilePosition);
  fcRead(&fc, object->buffer, 0);
  fcClose(&fc);
  object->temporalFilePosition = -1;
  object->streamMode = 0;
}

/**
 * Frees the memory occupied by this stream. Call this function when this object is no
 * longer needed.
 */
void destroy(ByteStream *object) {
  free(object->buffer.array);
  free(object->readFileSegments.array);
}

/**
 * Destroys the temporal file shared by all objects of this class. Call this function only
 * when objects of this class are no longer needed.
 *
 * @throws Exception when some problem removing the temporary file occurs
 */
void destroyTemporalFile(ByteStream *object) {
  if(object->temporalFileName != NULL) {
    assert(remove(object->temporalFileName));
    object->temporalFileName = NULL;
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
  if(object->streamMode == 0) {
    memory = object->readFileSegments.length * 2 * (sizeof(long) / 8);
  }
  return(memory);
}
