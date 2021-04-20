#ifndef FC_HH
#define FC_HH

struct SA_s {
  long **array;
  int length;
};
typedef struct SA_s SegmentsArray;

struct BB_s {
  unsigned char *array;
  long length;
};
typedef struct BB_s ByteBuffer;

struct FC_s {
  FILE *file;
  int descriptor;
};
typedef struct FC_s FileChannel;

extern FileChannel FileChannel_0(char *fileName, char * mode);  // No test needed
extern long fcGetPosition(FileChannel object);                           // Tested
extern void fcPosition(FileChannel object, long position);               // Tested
extern long fcSize(FileChannel object);                                  // Tested
extern long fcTransferFrom(FileChannel object, FileChannel src,          // Tested
                    long position, long count);
extern long fcRead(FileChannel object, ByteBuffer src, long position);   // Tested
extern void fcWrite(FileChannel object, ByteBuffer src,                  // Tested
                    long position, long count);
extern void fcClose(FileChannel object);                                 // No test needed

#endif /* FC_HH */
