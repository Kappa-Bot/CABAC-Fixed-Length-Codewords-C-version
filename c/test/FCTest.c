#include <stdio.h>
#include <stdlib.h>

#ifndef FC_HH
#include "../src/FileChannel.h"
#endif

#define FILENAME1 "../../files/fc1.tmp"
#define FILENAME2 "../../files/fc2.tmp"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"


int main() {
  printf("%s\n", H1);
  printf("%45s\n", "Initializing FileChannels");
  printf("FileChannel 1 |read|  fc1.tmp contains\t|\t\'lorem ipsum\'\n");
  printf("FileChannel 2 |write| fc2.tmp receives\t|\t\'lorem ipsum\'\n");
  printf("%s\n", H1);

  FileChannel FC1 = FileChannel_0(FILENAME1, "r");

  /* **************************************************** */
  printf("%41s\n", "fcSize Test");
  printf("%s\n", H2);
  printf("Getting lenght of file 1\n");
  long size = fcSize(FC1);
  printf("File Size\t|\tgot: %-6ld\texpected: 13\n", size);
  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%51s\n", "fcGetPosition + fcPosition Test");
  printf("%s\n", H2);
  printf("Getting first and middle position (0) by moving to mid point (13 / 2)\n");
  long before = fcGetPosition(FC1);
  printf("Start Position\t|\tgot: %-6ld\texpected: 0\n", before);
  fcPosition(FC1, size / 2);
  long after = fcGetPosition(FC1);
  printf("Mid Position\t|\tgot: %-6ld\texpected: 6\n", after);
  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%41s\n", "fcRead Test");
  printf("%s\n", H2);

  printf("Reading 6 bytes from position 0\nGot:      | ");
  fcPosition(FC1, 0);
  ByteBuffer BB = { (unsigned char *) malloc(after), size / 2};
  fcRead(FC1, BB, 0);
  for (int i = 0; i < size / 2; i++) {
    printf("%d | ", (int) BB.array[i]);
  }
  printf("\nExpected: | 108 | 111 | 114 | 101 | 109 | 32 | = \'lorem \'\n");
  printf("%s\n", H2);

  printf("Reading 6 bytes from position 6\nGot:      | ");
  fcRead(FC1, BB, size / 2);
  for (int i = 0; i < size / 2; i++) {
    printf("%d | ", (int) BB.array[i]);
  }
  printf("\nExpected: | 105 | 112 | 115 | 117 | 109 | 13 | = \'ipsum\\n\'\n");

  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%45s\n", "fcTransferFrom Test");
  printf("%s\n", H2);
  FileChannel FC2 = FileChannel_0(FILENAME2, "w");
  long written = fcTransferFrom(FC2, FC1, 0, size * 2);
  printf("Tried to Write: %ld\nFile Size: %ld\nTotal Written: %ld\n",
      size * 2, size, written);
  printf("%s\n", H2);
  printf("%s\n", H1);

  fcClose(FC1);
  fcClose(FC2);

  free(BB.array);
  return 0;
}
