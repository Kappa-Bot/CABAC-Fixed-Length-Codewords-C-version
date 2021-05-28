#ifndef BS_HH
#define BS_HH

// Won't include the library if not compiled with "-fopenmp"
#ifdef _OPENMP
    #include <omp.h>
#endif

#ifndef FC_HH
#include "FileChannel.h"
#endif

#define INITIAL_ALLOCATION 1024
#define INITIAL_NUM_SEGMENTS 32

extern struct BS_s {
  int streamMode;                     // 0/1/2 <-> normal/readFile/temporalFile
  ByteBuffer buffer;                  // Array in which the bytes are stored
  long long limit;                    // X != 0
  long long position;                 // position <= limit
  FileChannel readFileChannel;        // Only for readFile mode
  int readFileNumSegments;            // 0 < X
  SegmentsArray readFileSegments;     // The indices are [segment][0- first byte, 1- length]
  long long temporalFilePosition;     // Only for temporalFile mode
  char *temporalFileName;             // File name in which the stream is saved temporarily
  ByteBuffer temporalBuffer;          // Only for readFile mode
  #ifdef _OPENMP
      omp_lock_t lock;                // For lock access shared by multiple threads
  #endif
} BS_default;

/**
 * Defines a default instance initialization using the following instruction:
 *  memcpy(BSNewInstance, &BS_default, sizeof(ByteStream));
 */
typedef struct BS_s ByteStream;

extern ByteStream *ByteStreamInit();

extern ByteStream *ByteStream_0();
extern ByteStream *ByteStream_1(int initialAllocation);
extern ByteStream *ByteStream_2(FileChannel fc);

extern void putByte(ByteStream *object, signed char b);
extern void putBytes_0(ByteStream *object, ByteBuffer array,
                int offset, int length);
extern void putBytes_1(ByteStream *object, int num, int numBytes);

extern void putFileSegment(ByteStream *object, long long begin, long long length);

extern void removeByte(ByteStream *object);
extern void removeBytes(ByteStream *object, int num);

extern signed char getByte_0(ByteStream *object);
extern signed char getByte_1(ByteStream *object, long long index);

extern signed char getByte_2(ByteStream *object, long long index);

extern int getBytes(ByteStream *object, int numBytes);

extern int hasMoreBytes(ByteStream *object);
extern ByteBuffer getByteStream(ByteStream *object);
extern long long getLength(ByteStream *object);
extern long long getPosition(ByteStream *object);

extern void clear(ByteStream *object);
extern void ByteStream_reset(ByteStream *object);
extern void skip(ByteStream *object, long long numBytes);

extern void endReadFileMode(ByteStream *object);
extern void returnReadFileMode(ByteStream *object);

extern void packetize(ByteStream *object);

extern int isInReadNormalMode(ByteStream *object);
extern int isInReadFileMode(ByteStream *object);
extern int isInTemporalFile(ByteStream *object);

extern void write_0(ByteStream *object, FileChannel fc);
extern void write_1(ByteStream *object, FileChannel fc, long long begin, long long length);

extern void saveToTemporalFile(ByteStream *object, char *temporalDirectory);
extern void loadFromTemporalFile(ByteStream *object);

extern void destroy(ByteStream *object);
extern void destroyTemporalFile(ByteStream *object);
extern int getMemorySegments(ByteStream *object);
#endif /* BS_HH */
