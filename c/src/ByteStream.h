#ifndef BS_HH
#define BS_HH

#include <omp.h>

#ifndef FC_HH
#include "FileChannel.h"
#endif

#define INITIAL_ALLOCATION 1024
#define INITIAL_NUM_SEGMENTS 32

extern struct BS_s {
  int streamMode;                 // 0/1/2 <-> normal/readFile/temporalFile
  ByteBuffer buffer;               // Array in which the bytes are stored
  long limit;                     // X != 0
  long position;                  // position <= limit
  FileChannel readFileChannel;   // Only for readFile mode
  int readFileNumSegments;        // 0 < X
  SegmentsArray readFileSegments; // The indices are [segment][0- first byte, 1- length]
  long temporalFilePosition;      // Only for temporalFile mode
  char *temporalFileName;         // File name in which the stream is saved temporarily
  ByteBuffer temporalBuffer;       // Only for readFile mode
  // omp_lock_t lock;                // For lock access shared by multiple threads
} BS_default;

/**
 * Defines a default instance initialization using the following instruction:
 *  memcpy(BSNewInstance, &BS_default, sizeof(ByteStream));
 */
typedef struct BS_s ByteStream;


extern ByteStream *ByteStreamInit();                               // No test needed

extern ByteStream *ByteStream_0();                                 // No test needed
extern ByteStream *ByteStream_1(int initialAllocation);            // No test needed
extern ByteStream *ByteStream_2(FileChannel fc);                   // No test needed

extern void putByte(ByteStream *object, unsigned char b);          // Tested
extern void putBytes_0(ByteStream *object, ByteBuffer array,       // Tested
                int offset, int length);
extern void putBytes_1(ByteStream *object, int num, int numBytes); // Tested

extern void putFileSegment(ByteStream *object, long begin, long length);

extern void removeByte(ByteStream *object);                        // No test needed
extern void removeBytes(ByteStream *object, int num);              // No test needed

extern unsigned char getByte_0(ByteStream *object);
extern unsigned char getByte_1(ByteStream *object, long index);
extern int getBytes(ByteStream *object, int numBytes);

extern int hasMoreBytes(ByteStream *object);                       // No test needed
extern ByteBuffer getByteStream(ByteStream *object);               // No test needed
extern long getLength(ByteStream *object);                         // No test needed
extern long getPosition(ByteStream *object);                       // No test needed

extern void clear(ByteStream *object);                             // No test needed
extern void ByteStream_reset(ByteStream *object);                  // No test needed
extern void skip(ByteStream *object, long numBytes);

// void endReadFileMode(ByteStream *object);
extern void returnReadFileMode(ByteStream *object);

extern void packetize(ByteStream *object);

extern int isInReadNormalMode(ByteStream *object);                 // No test needed
extern int isInReadFileMode(ByteStream *object);                   // No test needed
extern int isInTemporalFile(ByteStream *object);                   // No test needed

// void write_0(ByteStream *object, FileOutputStream fos); // Need to implement FileOutputStream
// void write_1(ByteStream *object, FileOutputStream fos, long begin, long length);

extern void saveToTemporalFile(ByteStream *object, char *temporalDirectory);
extern void loadFromTemporalFile(ByteStream *object);

extern void destroy(ByteStream *object);
extern void destroyTemporalFile(ByteStream *object);
extern int getMemorySegments(ByteStream *object);

#endif /* BS_HH */
