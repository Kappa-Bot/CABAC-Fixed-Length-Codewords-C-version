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

FileChannel fcConfig(char *fileName, char * mode);                // No test needed
long fcGetPosition(FileChannel object);                           // Tested
void fcPosition(FileChannel object, long position);               // Tested
long fcSize(FileChannel object);                                  // Tested
void fcTransferFrom(FileChannel object, FileChannel src,          // Tested
                    long position, long count);
void fcRead(FileChannel object, ByteBuffer src, long position);   // Tested
void fcWrite(FileChannel object, ByteBuffer src);                 // Tested
void fcClose(FileChannel object);                                 // No test needed

#endif /* FC_HH */
