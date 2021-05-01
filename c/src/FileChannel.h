#ifndef FC_HH
#define FC_HH

#include <sys/stat.h>

struct SA_s {
  long long **array;
  int length;
};
typedef struct SA_s SegmentsArray;

struct BB_s {
  signed char *array;
  long long length;
};
typedef struct BB_s ByteBuffer;

struct FC_s {
  FILE *file;
  int descriptor;
  off_t position;
  struct stat stat;
};
typedef struct FC_s FileChannel;

extern FileChannel FileChannel_0(char *fileName, char *mode);  // No test needed
extern off_t fcGetPosition(FileChannel *object);                           // Tested
extern void fcPosition(FileChannel *object, long long position);               // Tested
extern long long fcSize(FileChannel *object);                                  // Tested
extern long long fcTransferFrom(FileChannel *object, FileChannel *src,          // Tested
                    off_t *position, long long count);
extern long long fcRead(FileChannel *object, ByteBuffer src, off_t position);   // Tested
extern signed char fcRead1B(FileChannel *object, ByteBuffer src, off_t position);   // Tested
extern long long fcWrite(FileChannel *object, ByteBuffer src,                  // Tested
                    off_t position, long long count);
extern void fcClose(FileChannel *object);                                 // No test needed

#endif /* FC_HH */
