#include "StreamGenerator.h"

#define PATH "../files/"
#define EXT ".tmp"
#define FILESIZE 32
#define FILECOUNT 8

StreamGenerator SG = { PATH, EXT };

int main() {
  sgGenerate(SG, FILESIZE, FILECOUNT);
  return 0;
}
