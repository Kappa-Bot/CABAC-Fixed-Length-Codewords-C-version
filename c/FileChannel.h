#ifndef FC_HH
#define FC_HH

struct SA_s {
  long **array;
  int length;
};
typedef struct SA_s SegmentsArray;

struct BB_s {
  unsigned char *array;
  int length;
};
typedef struct BB_s ByteBuffer;

struct FC_s {
  FILE *file;
};
typedef struct FC_s FileChannel;

FileChannel fcConfig(char *fileName, char * mode);
long fcGetPosition(FileChannel object);
void fcPosition(FileChannel object, long position);
long fcSize(FileChannel object);
void fcTransferFrom(FileChannel object, FileChannel src, long position, long count);
void fcRead(FileChannel object, ByteBuffer src, long position);
void fcWrite(FileChannel object, ByteBuffer src);
void fcClose(FileChannel object);

#endif /* FC_HH */
