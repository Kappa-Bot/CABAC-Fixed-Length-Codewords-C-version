#ifndef SG_HH
#define SG_HH

struct SG_s {
  char *path;
  char *extension;
};
typedef struct SG_s StreamGenerator;

void sgGenerate(StreamGenerator object, int fileSize, int count);

#endif /* SG_HH */
