#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ByteStream.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"

#define pack4B(a, b, c, d) *((int*) (char[4] ) {a, b, c, d} )

unsigned char lookUpByte_0(ByteStream *BS, int targetIndex) {
  for (int i = 0; i < targetIndex; ++i) {
    getByte_0(BS);
  }
  return getByte_0(BS);
}

int main() {
  printf("%s\n", H1);
  printf("Initializing ByteStream\n");

  ByteStream *BS = ByteStream_0();
  ByteBuffer arr = {
    (unsigned char []){0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
    16
  };
  unsigned char *ptr;
  char *ptr2;
  int verif;

  printf("%s\n", H1);
  printf("%41s\n", "putByte Test");
  printf("%s\n", H2);


  for (int i = 0; i < 10; i++) {
    ptr = BS->buffer.array;
    verif = 1;
    while (verif) {
      putByte(BS, 1); // [0 - 255], overflow not allowed
      verif = ptr == BS->buffer.array;
    }
    printf("old: %p\tnew: %p\tlength: %ld\n",
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
  for (int i = 0; i < 10; i++) {
    ptr = BS->buffer.array;
    verif = 1;
    while (verif) {
      putBytes_0(BS, arr, 0, 16); // Whole array, sequences of [0-16]
      verif = ptr == BS->buffer.array;
    }
    printf("old: %p\tnew: %p\tlength: %ld\n",
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
    printf("old: %p\tnew: %p\tlength: %ld\n",
      ptr, BS->buffer.array, BS->buffer.length);
  }
  printf("%s\n", H2);
  ptr = BS->buffer.array;
  printf("0: %d\t\t65534: %d\t\t524287: %d\t\t1000000: %d\n",
            ptr[0], ptr[65534], ptr[524287], ptr[1000000]);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "getByte_1 Test (normal mode)");
  printf("%s\n", H2);
  ptr = BS->buffer.array;

  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS, 0), ptr[0]);
  printf("%s\n", H2);

  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS, 65534), ptr[65534]);
  printf("%s\n", H2);

  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS, 524287), ptr[524287]);
  printf("%s\n", H2);

  printf("Index: 1000000\t\t Got: %d\t\t\tExpected: %d\n", getByte_1(BS, 1000000), ptr[1000000]);

  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "getByte_0 Test (normal mode)");
  printf("%s\n", H2);
  ptr = BS->buffer.array;

  // Just don't want employ resets here for future tests
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 0), ptr[0]);
  printf("%s\n", H2);

  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 65534 - 1), ptr[65534]);
  printf("%s\n", H2);

  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 524287 - 65535), ptr[524287]);
  printf("%s\n", H2);

  printf("Index: 1000000\t\t Got: %d\t\t\tExpected: %d\n", lookUpByte_0(BS, 1000000 - 524288), ptr[1000000]);
  printf("%s\n", H2);
  /* **************************************************** */
  printf("%36s\n", "getBytes Test (normal mode)");
  printf("%s\n", H2);
  ByteStream_reset(BS); // lol I lied
  ptr2 = (char *)BS->buffer.array;

  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 1), pack4B(ptr2[0], 0, 0, 0));
  printf("%s\n", H2);

  printf("Index: 1-2\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 2), pack4B(ptr2[2], ptr2[1], 0, 0));
  printf("%s\n", H2);

  printf("Index: 3-5\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 3), pack4B(ptr2[5], ptr2[4], ptr2[3], 0));
  printf("%s\n", H2);

  printf("Index: 6-9\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 4), pack4B(ptr2[9], ptr2[8], ptr2[7], ptr2[6]));
  printf("%s\n", H2);

  printf("Index: 10-14\t\t Got: %d \t\tExpected: %d\n", getBytes(BS, 5),
    pack4B(ptr2[14], ptr2[13], ptr2[12], ptr2[11]) + pack4B(ptr2[10], 0, 0, 0));
  printf("%s\n", H2);

  printf("Index: 15-22\t\t Got: %d \tExpected: %d\n", getBytes(BS, 8),
    pack4B(ptr2[22], ptr2[21], ptr2[20], ptr2[19]) + pack4B(ptr2[18], ptr2[17], ptr2[16], ptr2[15]));
  printf("%s\n", H2);
  /* Code for displaying the bits of the output number step by step in function getBytes(BS, num)

      // Simplified status of the buffer
      printf("\t\t%d\t%d|%d|%d|%d\n", (char) getByte, (num >> 0) & 0xff, (num >> 8) & 0xff, (num >> 16) & 0xff, (num >> 24) & 0xff);

      // New byte added
      printf("(%d) -> %d = ", (char) getByte, b * 8);
      for (int j = 7; j >= 0; j--) {
        printf("%c", (((char) getByte) & (1 << j)) ? '1' : '0');
      }
      printf(" ");
      printf("\n");

      // Detailed status of the buffer
      printf("(%d) %*s = ", num, 8, "->");
      for (int i = 0; i <= 3; i++) {
        for (int j = 7; j >= 0; j--) {
          printf("%c", ((num >> (i * 8)) & (1 << j)) ? '1' : '0');
        }
        printf(" ");
      }
      printf("\n");
  */
  /* **************************************************** */
  printf("%36s\n", "write_1 Test");
  printf("%s\n", H2);
  ByteStream_reset(BS); // lol I lied

  int verifOut;
  ByteBuffer tmpBuff0 = {malloc(getLength(BS) / 2), getLength(BS) / 2};

  printf("Writing [0 - %ld] bytes into w1_0_c.tmp\n", getLength(BS) / 2);
  FileChannel fcOut1_0 = FileChannel_0("../../files/w1_0_c.tmp", "w");
  write_1(BS, fcOut1_0, 0, getLength(BS) / 2);
  fcClose(fcOut1_0);

  printf("Checking integrity -> w1_0_c.tmp\n");
  FileChannel fcIn1_0 = FileChannel_0("../../files/w1_0_c.tmp", "r");
  fcRead(fcIn1_0, tmpBuff0, 0);
  verifOut = 1;
  for (long i = 0; i < getLength(BS) / 2 && verifOut; ++i) {
    verifOut = ptr[i] == tmpBuff0.array[i];
  }
  printf("Result: %s\n", verifOut ? "OK" : "Error");
  fcClose(fcIn1_0);
  free(tmpBuff0.array);
  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "write_0 Test");
  printf("%s\n", H2);
  ByteStream_reset(BS); // lol I lied
  ByteBuffer tmpBuff2 = {malloc(getLength(BS)), getLength(BS)};

  printf("Writing everything into w0_c.tmp\n");
  FileChannel fcOut0 = FileChannel_0("../../files/w0_c.tmp", "w");
  write_0(BS, fcOut0);

  printf("Checking integrity -> w0_c.tmp\n");
  FileChannel fcIn0 = FileChannel_0("../../files/w0_c.tmp", "r");
  fcRead(fcIn0, tmpBuff2, 0);
  verifOut = 1;
  for (long i = 0; i < getLength(BS) && verifOut; ++i) {
    verifOut = ptr[i] == tmpBuff2.array[i];
  }
  printf("Result: %s\n", verifOut ? "OK" : "Error");
  fcClose(fcIn0);
  free(tmpBuff2.array);

  fcClose(fcOut0);
  printf("%s\n", H1);
  /* **************************************************** */

  destroy(BS);
  free(BS);
  return(0);
}
