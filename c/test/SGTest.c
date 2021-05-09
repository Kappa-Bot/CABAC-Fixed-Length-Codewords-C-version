#include "../util/StreamGenerator.h"

#define PATH "../../files/"
#define EXT ".tmp"
#define FILESIZE (1024 * 1024 * 16) // 16MB
#define FILECOUNT 2

StreamGenerator SG;

int main() {
  SG = sgConfig(PATH, EXT);
  sgGenerate(SG, FILESIZE, FILECOUNT);
  return 0;
}
