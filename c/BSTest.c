#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ByteStream.h"

int main() {
  printf("**************************************************************************\n");
  printf("Initializing ByteStream\n");
  ByteStream *BS = (ByteStream *) malloc(sizeof(ByteStream));
  memcpy(BS, &BS_default, sizeof(ByteStream));
  ByteStream_0(BS);
  printf("**************************************************************************\n");
  printf("putByte Test\n");
  printf("__________________________________________________________________________\n");
  unsigned char *ptr;
  int verif;
  for (int i = 0; i < 10; i++) {
    ptr = BS->buffer.array;
    verif = 1;
    while (verif) {
      putByte(BS, 1); // [0 - 255], overflow not allowed
      verif = ptr == BS->buffer.array;
    }
    printf("old: %p\tnew: %p\tlength: %d\n", ptr, BS->buffer.array, BS->buffer.length);
  }
  printf("__________________________________________________________________________\n");
  ptr = BS->buffer.array;
  printf("0: %d\t\t65536: %d\t\t524287: %d\t\t1000000: %d\n", ptr[0], ptr[65536], ptr[524287], ptr[1000000]);
  printf("**************************************************************************\n");

  destroy(BS);
  free(BS);
  return(0);
}
