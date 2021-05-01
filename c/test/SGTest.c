#include "../util/StreamGenerator.h"

#define PATH "../../files/"
#define EXT ".tmp"
#define FILESIZE 4096
#define FILECOUNT 2

StreamGenerator SG;

int main() {
  SG = sgConfig(PATH, EXT);
  sgGenerate(SG, FILESIZE, FILECOUNT);
  return 0;
}
