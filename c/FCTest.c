#include <stdio.h>
#include <stdlib.h>

#ifndef FC_HH
#include "FileChannel.h"
#endif

#define FILENAME1 "../files/fc1.tmp"
#define FILENAME2 "../files/fc2.tmp"

FileChannel FC1 = {};
FileChannel FC2 = {};

int main() {
  FC1.file = fopen(FILENAME1, "r");

  long size = fcSize(&FC1);
  long before = fcGetPosition(&FC1);
  fcPosition(&FC1, size / 2);
  long after = fcGetPosition(&FC1);

  printf("Size of file: %-8ld\tStarting at: %-10ld\tMid point at: %ld\n",
          size, before, after);

  ByteBuffer BB = { (unsigned char *) malloc(after), size / 2 };
  fcRead(&FC1, BB, size - after);
  for (int i = 0; i < BB.length; i++) {
    printf("%c", BB.array[i]);
  }
  fcPosition(&FC1, 0);

  FC2.file = fopen(FILENAME2, "rb+");
  fcTransferFrom(&FC2, &FC1, 0, size);

  fcClose(&FC1);
  fcClose(&FC2);

  return 0;
}
