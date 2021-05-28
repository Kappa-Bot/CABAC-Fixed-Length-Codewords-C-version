#ifndef FC_HH
#define FC_HH

#define FC_READ "r"
#define FC_WRITE "w"

#include <sys/stat.h>

struct SA_s {
  long long *begins;
  long long *lengths;
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
  signed char *buffer;
  int descriptor;
  off_t position;
  struct stat stat;
};
typedef struct FC_s FileChannel;

extern FileChannel FileChannel_0(char *fileName, char *mode);
extern off_t fcGetPosition(FileChannel *object);
extern void fcPosition(FileChannel *object, long long position);
extern long long fcSize(FileChannel *object);
extern long long fcTransferFrom(FileChannel *object, FileChannel *src,
                    off_t *position, long long count);
extern long long fcRead(FileChannel *object, ByteBuffer src, off_t position);
extern signed char fcRead1B(FileChannel *object, ByteBuffer src, off_t position);
extern long long fcWrite(FileChannel *object, ByteBuffer src,
                    off_t position, long long count);
extern void fcClose(FileChannel *object);

#endif /* FC_HH */
