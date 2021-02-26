#ifndef BS_HH
#define BS_HH

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
// #include <omp.h>

#define INITIAL_ALLOCATION 1024
#define INITIAL_NUM_SEGMENTS 32

struct BA_s {
  unsigned char *array;
  int length;
};
typedef struct BA_s ByteArray;

struct SA_s {
  long **array;
  int length;
};
typedef struct SA_s SegmentsArray;

struct FC_s {
  char *fileName;
  int descriptor;
  int size;
};
typedef struct FC_s FileChannel;

struct BS_s {
  int streamMode;                 // 0/1/2 <-> normal/readFile/temporalFile
  ByteArray buffer;               // Array in which the bytes are stored
  long limit;                     // X != 0
  long position;                  // position <= limit
  FileChannel readFileChannel;    // Only for readFile mode
  int readFileNumSegments;        // 0 < X
  SegmentsArray readFileSegments; // The indices are [segment][0- first byte, 1- length]
  long temporalFilePosition;      // Only for temporalFile mode
  char *temporalFileName;         // File name in which the stream is saved temporarily
  ByteArray temporalBuffer;       // Only for readFile mode
  // omp_lock_t lock;                // For lock access shared by multiple threads
} BS_default = {                  // Default values for initial "instance"
  -1,                             // streamMode
  {(unsigned char *) NULL, 0},    // buffer
  0,                              // limit
  0,                              // position
  {(char *) NULL, -1, 0},         // readFileChannel
  -1,                             // readFileNumSegments
  {(long **) NULL, 0},            // readFileSegments
  -1,                             // temporalFilePosition
  '\0',                           // temporalFileName
  {NULL, 0}                       // temporalBuffer
  // 0                               // lock
};

/**
 * Defines a default instance initialization using the following instruction:
 * ByteStream BS = BS_default;
 */
typedef struct BS_s ByteStream;

void ByteStream_0(ByteStream *object);
void ByteStream_1(ByteStream *object, int initialAllocation);
void ByteStream_2(ByteStream *object, FileChannel fc);

void putByte(ByteStream *object, unsigned char b);
void putBytes_0(ByteStream *object, ByteArray array, int offset, int length);
void putBytes_1(ByteStream *object, int num, int numBytes);
// void putFileSegment(ByteStream *object, long begin, long length);
void removeByte(ByteStream *object);
void removeBytes(ByteStream *object, int num);
unsigned char getByte_0(ByteStream *object);
// int getBytes(ByteStream *object, int numBytes);
int hasMoreBytes(ByteStream *object);
unsigned char getByte_1(ByteStream *object, long index);
ByteArray getByteStream(ByteStream *object);
long getLength(ByteStream *object);
long getPosition(ByteStream *object);
void clear(ByteStream *object);
void ByteStream_reset(ByteStream *object);
void skip(ByteStream *object, long numBytes);
// void endReadFileMode(ByteStream *object);
void returnReadFileMode(ByteStream *object);
void packetize(ByteStream *object);
int isInReadNormalMode(ByteStream *object);
int isInReadFileMode(ByteStream *object);
int isInTemporalFile(ByteStream *object);
// void write_0(ByteStream *object, FileOutputStream fos); // Need to implement FileOutputStream
// void write_1(ByteStream *object, FileOutputStream fos, long begin, long length);
// void saveToTemporalFile(ByteStream *object, String temporalDirectory);
// void loadFromTemporalFile(ByteStream *object);
void destroy(ByteStream *object);
// void destroyTemporalFile(ByteStream *object);
int getMemorySegments(ByteStream *object);

/**
 * Allocates memory for the buffer. The stream enters in the normal mode.
 */
void ByteStream_0(ByteStream *object) {
  object->buffer.array = (unsigned char *) malloc(INITIAL_ALLOCATION);
  object->buffer.length = INITIAL_ALLOCATION;
  object->streamMode = 0;
}

/**
 * Allocates memory for the buffer, with a specified limit. The stream enters in the normal mode.
 *
 * @param initialAllocation number of bytes reserved in the buffer
 */
void ByteStream_1(ByteStream *object, int initialAllocation) {
  object->buffer.array = (unsigned char *) malloc(initialAllocation);
  object->buffer.length = initialAllocation;
  object->streamMode = 0;
}

/**
 * Initializes this stream with a file from which bytes are read. The stream enters in
 * the readFile mode.
 *
 * @param fc the file from which bytes are read
 */
void ByteStream_2(ByteStream *object, FileChannel fc) {
  object->readFileChannel = fc;
  object->readFileNumSegments = 0;
  object->readFileSegments.array = (long **) malloc(INITIAL_NUM_SEGMENTS * sizeof(long *));
  for (int i = 0; i < INITIAL_NUM_SEGMENTS; ++i) {
    object->readFileSegments.array[i] = (long *) malloc(2 * sizeof(long));
  }
  object->readFileSegments.length = INITIAL_NUM_SEGMENTS * 2;
  object->streamMode = 1;
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
    object->buffer.length = object->limit;
  }
  object->buffer.array = (unsigned char *) realloc(object->buffer.array, object->limit + 1);
  object->buffer.array[(int) object->limit] = b;
  object->buffer.length++;
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
void putBytes_0(ByteStream *object, ByteArray array, int offset, int length) {
  assert(object->streamMode == 0);
  assert((offset >= 0) && (length > 0) && (offset + length <= array.length));
  if(object->limit + (long) length > (long) object->buffer.length) {
    unsigned char *bufferTMP = (unsigned char*) malloc((object->buffer.length + length) * 2);
    memcpy(bufferTMP, object->buffer.array, (int) object->limit);
    free(object->buffer.array);
    object->buffer.array = bufferTMP;
    object->buffer.length = object->limit;
  }
  memcpy(object->buffer.array + object->limit, array.array + offset, length);
  object->limit += (long) length;
  object->buffer.length += length;
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
 * Removes one byte from the end of the stream. This function can only be called when the
 * stream is in normal mode.
 */
void removeByte(ByteStream *object) {
  assert(object->streamMode == 0);
  int verif = object->limit - 1 <= 0;
  object->limit = verif ? 0: object->limit - 1;
  // object->buffer.length = verif ? 0: object->buffer.length - 1
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
  int verif = object->limit - num <= 0;
  object->limit = verif ? 0: object->limit - num;
  // object->buffer.length = verif ? 0: object->buffer.length - num;
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
 * Gets the indicated byte. This function can only be called when the stream is in
 * normal or readFile mode.
 *
 * @param index the index of the byte (starting from 0)
 * @return the corresponding byte
 * @throws Exception when the indicated index is not valid
 */
unsigned char getByte_1(ByteStream *object, long index) {
  assert((object->streamMode == 0) || (object->streamMode == 1));

  if((index < 0) || (index >= object->limit)) {
    fprintf(stderr, "Invalid position.");
  }
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
    assert(fcPosition < object->readFileChannel.size);

    //Gets the byte
    // This needs a fseek() before the read() call
    // readFileChannel.read(temporalBuffer, fcPosition);
    getByte = object->temporalBuffer.array[0];
  } else {
    assert(1 == 0);
  }
  return(getByte);
}


/**
 * Returns the array of bytes. The length of the array is equal or greater than
 * the bytes put in the stream. This function can only be called when the stream is in normal mode.
 *
 * @return the byte array
 */
ByteArray getByteStream(ByteStream *object) {
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
    long length = (int) object->readFileSegments.array[segment][1];
    object->limit += length;
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
 * Frees the memory occupied by this stream. Call this function when this object is no
 * longer needed.
 */
void destroy(ByteStream *object) {
  free(object->buffer.array);
  free(object->readFileChannel.fileName);
  for (int i = 0; i < object->readFileSegments.length; ++i) {
    free(object->readFileSegments.array[i]);
  }
  free(object->readFileSegments.array);
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

#endif /* BS_HH */
