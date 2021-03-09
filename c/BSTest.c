#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ByteStream.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"

int main() {
  printf("%s\n", H1);
  printf("Initializing ByteStream\n");
  ByteStream *BS = ByteStream_0();
  printf("%s\n", H1);
  printf("%41s\n", "putByte Test");
  printf("%s\n", H2);
  unsigned char *ptr;
  int verif;
  for (int i = 0; i < 10; i++) {
    ptr = BS->buffer.array;
    verif = 1;
    while (verif) {
      putByte(BS, 1); // [0 - 255], overflow not allowed
      verif = ptr == BS->buffer.array;
    }
    printf("old: %p\tnew: %p\tlength: %d\n",
      ptr, BS->buffer.array, BS->buffer.length);
  }
  printf("%s\n", H2);
  ptr = BS->buffer.array;
  printf("0: %d\t\t65536: %d\t\t524287: %d\t\t1000000: %d\n",
            ptr[0], ptr[65536], ptr[524287], ptr[1000000]);
  destroy(BS);
  free(BS);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "putBytes_0 Test");
  printf("%s\n", H2);
  BS = ByteStream_0();
  ByteArray arr = {
    (unsigned char []){0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    16
  };
  for (int i = 0; i < 10; i++) {
    ptr = BS->buffer.array;
    verif = 1;
    while (verif) {
      putBytes_0(BS, arr, 0, 16); // Whole array, sequences of [0-16]
      verif = ptr == BS->buffer.array;
    }
    printf("old: %p\tnew: %p\tlength: %d\n",
      ptr, BS->buffer.array, BS->buffer.length);
  }
  printf("%s\n", H2);
  ptr = BS->buffer.array;
  printf("0: %d\t\t67541: %d\t\t540639: %d\t\t1000000: %d\n",
            ptr[0], ptr[67541], ptr[540639], ptr[1000000]);
  destroy(BS);
  free(BS);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%42s\n", "putBytes_1 Test");
  printf("%s\n", H2);
  BS = ByteStream_0();
  for (int i = 0; i < 10; i++) {
    ptr = BS->buffer.array;
    verif = 1;
    while (verif) {
      putBytes_1(BS, 2147483647, 256); // Max 4B value
      verif = ptr == BS->buffer.array;
    }
    printf("old: %p\tnew: %p\tlength: %d\n",
      ptr, BS->buffer.array, BS->buffer.length);
  }
  printf("%s\n", H2);
  ptr = BS->buffer.array;
  printf("0: %d\t\t65534: %d\t\t524287: %d\t\t1000000: %d\n",
            ptr[0], ptr[65534], ptr[524287], ptr[1000000]);
  destroy(BS);
  free(BS);
  printf("%s\n", H1);
  /* **************************************************** */
  return(0);
}
