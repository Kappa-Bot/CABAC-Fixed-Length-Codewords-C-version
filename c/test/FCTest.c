#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

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

  FileChannel FC1 = FileChannel_0(FILENAME1, FC_READ);

  /* **************************************************** */
  printf("%41s\n", "fcSize Test");
  printf("%s\n", H2);
  printf("Getting lenght of file 1\n");
  long long size = fcSize(&FC1);
  printf("File Size\t|\tgot: %-6lld\texpected: 13\n", size);
  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%51s\n", "fcGetPosition + fcPosition Test");
  printf("%s\n", H2);
  printf("Getting first and middle position (0) by moving to mid point (13 / 2)\n");
  long long before = fcGetPosition(&FC1);
  printf("Start Position\t|\tgot: %-6lld\texpected: 0\n", before);
  fcPosition(&FC1, size / 2);
  long long after = fcGetPosition(&FC1);
  printf("Mid Position\t|\tgot: %-6lld\texpected: 6\n", after);
  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%41s\n", "fcRead Test");
  printf("%s\n", H2);

  printf("Reading 6 bytes from position 0\nGot:      | ");
  fcPosition(&FC1, 0);
  ByteBuffer BB = { (signed char *) malloc(after), size / 2};
  fcRead(&FC1, BB, 0);
  for (int i = 0; i < size / 2; i++) {
    printf("%d | ", (int) BB.array[i]);
  }
  printf("\nExpected: | 108 | 111 | 114 | 101 | 109 | 32 | = \'lorem \'\n");
  printf("%s\n", H2);

  printf("Reading 6 bytes from position 6\nGot:      | ");
  fcRead(&FC1, BB, size / 2);
  for (int i = 0; i < size / 2; i++) {
    printf("%d | ", (int) BB.array[i]);
  }
  printf("\nExpected: | 105 | 112 | 115 | 117 | 109 | 13 | = \'ipsum\\n\'\n");

  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%41s\n", "fcRead1B Test");
  printf("%s\n", H2);

  printf("Reading 6 bytes from position 0\nGot:      | ");
  fcPosition(&FC1, 0);
  ByteBuffer BB1B = { (signed char *) malloc(1), 1};
  for (int i = 0; i < size / 2; i++) {
    fcRead1B(&FC1, BB1B, i);
    printf("%d | ", (int) BB1B.array[0]);
  }
  printf("\nExpected: | 108 | 111 | 114 | 101 | 109 | 32 | = \'lorem \'\n");
  printf("%s\n", H2);

  printf("Reading 6 bytes from position 6\nGot:      | ");
  for (int i = size / 2; i < size - 1; i++) {
    fcRead1B(&FC1, BB1B, i);
    printf("%d | ", (int) BB1B.array[0]);
  }
  printf("\nExpected: | 105 | 112 | 115 | 117 | 109 | 13 | = \'ipsum\\n\'\n");

  printf("%s\n", H2);
  printf("%s\n", H1);

  /* **************************************************** */
  printf("%45s\n", "fcTransferFrom Test");
  printf("%s\n", H2);
  FileChannel FC2 = FileChannel_0(FILENAME2, FC_WRITE);
  printf("File size before: %lld\n", fcSize(&FC2));
  off_t transferPosition = (off_t) 0;
  long long written = fcTransferFrom(&FC2, &FC1, &transferPosition, size * 2);
  printf("Tried to Write: %lld\nFile Size: %lld\nTotal Written: %lld\nFile size after: %lld\n",
      size * 2, size, written, fcSize(&FC2));
  printf("%s\n", H2);
  printf("%s\n", H1);

  fcClose(&FC1);
  fcClose(&FC2);

  free(BB.array);
  return 0;
}
