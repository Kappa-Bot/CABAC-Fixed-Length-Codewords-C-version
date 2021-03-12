#ifndef SC_HH
#define SC_HH

struct SC_s {
  FILE *file1;
  FILE *file2;
  long size;
};
typedef struct SC_s StreamChecker;

StreamChecker scConfig(char *fileName1, char *fileName2);
int scCheckEquals(StreamChecker object);
long scCheckError(StreamChecker object);
void scFinish(StreamChecker object);

#endif /* SC_HH */
