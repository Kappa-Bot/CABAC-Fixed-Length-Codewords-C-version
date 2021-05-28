#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ByteStream.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"

#define pack4B(a, b, c, d) *((int*) (signed char[4] ) {a, b, c, d} )

signed char lookUpByte_0(ByteStream *BS, int targetIndex) {
  for (int i = 0; i < targetIndex; ++i) {
    getByte_0(BS);
  }
  return getByte_0(BS);
}

int main() {
  printf("%s\n", H1);
  printf("Initializing ByteStream (normal mode)\n");

  ByteStream *BS = ByteStream_0();
  ByteBuffer arr = {
    (signed char []){0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15},
    16
  };
  signed char *ptr = BS->buffer.array;
  long long auxLen = BS->buffer.length;
  int verif;

  printf("%s\n", H1);
  printf("%41s\n", "putByte Test");
  printf("%s\n", H2);

  for (int i = 0; i < 10; i++) {
    ptr = BS->buffer.array;
    auxLen = BS->buffer.length;
    verif = 1;
    while (verif) {
      putByte(BS, 1); // [0 - 255], overflow not allowed
      verif = auxLen == BS->buffer.length;
    }
    printf("old: %p\tnew: %p\tlength: %lld\n",
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
    auxLen = BS->buffer.length;
    verif = 1;
    while (verif) {
      putBytes_0(BS, arr, 0, 16); // Whole array, sequences of [0-16]
      verif = auxLen == BS->buffer.length;
    }
    printf("old: %p\tnew: %p\tlength: %lld\n",
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
    auxLen = BS->buffer.length;
    verif = 1;
    while (verif) {
      putBytes_1(BS, pack4B(1, 63, 127, 255), 4); // Max 4B value
      verif = auxLen == BS->buffer.length;
    }
    printf("old: %p\tnew: %p\tlength: %lld\n",
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
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS, 0), ptr[0]);
  printf("%s\n", H2);
  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS, 65534), ptr[65534]);
  printf("%s\n", H2);
  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS, 524287), ptr[524287]);
  printf("%s\n", H2);
  printf("Index: limit-1\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS, BS->limit - 1), ptr[BS->limit - 1]);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "getByte_0 Test (normal mode)");
  printf("%s\n", H2);

  // Just don't want employ resets here for future tests
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 0), ptr[0]);
  printf("%s\n", H2);
  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 65534 - 1), ptr[65534]);
  printf("%s\n", H2);
  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, 524287 - 65535), ptr[524287]);
  printf("%s\n", H2);
  printf("Index: limit-1\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS, BS->limit - 1 - 524288), ptr[BS->limit - 1]);
  printf("%s\n", H2);
  /* **************************************************** */
  printf("%36s\n", "getBytes Test (normal mode)");
  printf("%s\n", H2);
  ByteStream_reset(BS); // lol I lied

  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 1), pack4B(ptr[0], 0, 0, 0));
  printf("%s\n", H2);
  printf("Index: 1-2\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 2), pack4B(ptr[2], ptr[1], 0, 0));
  printf("%s\n", H2);
  printf("Index: 3-5\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 3), pack4B(ptr[5], ptr[4], ptr[3], 0));
  printf("%s\n", H2);
  printf("Index: 6-9\t\t Got: %d\t\tExpected: %d\n", getBytes(BS, 4), pack4B(ptr[9], ptr[8], ptr[7], ptr[6]));
  printf("%s\n", H2);
  printf("Index: 10-14\t\t Got: %d \t\tExpected: %d\n", getBytes(BS, 5),
    pack4B(ptr[14], ptr[13], ptr[12], ptr[11]) + pack4B(ptr[10], 0, 0, 0));
  printf("%s\n", H2);
  printf("Index: 15-22\t\t Got: %d \tExpected: %d\n", getBytes(BS, 8),
    pack4B(ptr[22], ptr[21], ptr[20], ptr[19]) + pack4B(ptr[18], ptr[17], ptr[16], ptr[15]));
  printf("%s\n", H2);
  /* Code for displaying the bits of the output number step by step in function getBytes(BS, num)

      // Simplified status of the buffer
      printf("\t\t%d\t%d|%d|%d|%d\n", (signed char) getByte, (num >> 0) & 0xff, (num >> 8) & 0xff, (num >> 16) & 0xff, (num >> 24) & 0xff);

      // New byte added
      printf("(%d) -> %d = ", (signed char) getByte, b * 8);
      for (int j = 7; j >= 0; j--) {
        printf("%c", (((signed char) getByte) & (1 << j)) ? '1' : '0');
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
  ByteBuffer tmpBuff0 = {(signed char *) malloc(getLength(BS) / 2), getLength(BS) / 2};

  printf("Writing [0 - %lld] bytes into w1_0_c.tmp\n", getLength(BS) / 2);
  FileChannel fcOut1_0 = FileChannel_0("../../files/w1_0_c.tmp", FC_WRITE);
  write_1(BS, fcOut1_0, 0, getLength(BS) / 2);
  fcClose(&fcOut1_0);

  printf("Checking integrity -> w1_0_c.tmp\n");
  FileChannel fcIn1_0 = FileChannel_0("../../files/w1_0_c.tmp", FC_READ);
  fcRead(&fcIn1_0, tmpBuff0, 0);
  verifOut = 1;
  for (long long i = 0; i < getLength(BS) / 2 && verifOut; ++i) {
    verifOut = ptr[i] == tmpBuff0.array[i];
  }
  printf("Result: %s\n", verifOut ? "OK" : "Error");
  fcClose(&fcIn1_0);
  free(tmpBuff0.array);
  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "write_0 Test");
  printf("%s\n", H2);
  ByteStream_reset(BS); // lol I lied
  ByteBuffer tmpBuff2 = {(signed char *) malloc(getLength(BS)), getLength(BS)};

  printf("Writing everything into w0_c.tmp\n");
  FileChannel fcOut0 = FileChannel_0("../../files/w0_c.tmp", FC_WRITE);
  write_0(BS, fcOut0);

  printf("Checking integrity -> w0_c.tmp\n");
  FileChannel fcIn0 = FileChannel_0("../../files/w0_c.tmp", FC_READ);
  fcRead(&fcIn0, tmpBuff2, 0);
  verifOut = 1;
  for (long long i = 0; i < getLength(BS) && verifOut; ++i) {
    verifOut = ptr[i] == tmpBuff2.array[i];
  }
  printf("Result: %s\n", verifOut ? "OK" : "Error");
  fcClose(&fcIn0);
  free(tmpBuff2.array);

  fcClose(&fcOut0);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%s\n", H1);
  printf("Initializing ByteStream (readFile mode)\n");

  FileChannel FC = FileChannel_0("../../files/w0_c.tmp", FC_READ);
  ByteStream *BS2 = ByteStream_2(FC);

  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "putFileSegment Test");
  printf("%s\n", H2);

  printf("Putting two segments of size (limit / 2)\n");
  putFileSegment(BS2,                0L, BS->limit / 4);
  putFileSegment(BS2,     BS->limit / 4, BS->limit / 4);
  putFileSegment(BS2, 2 * BS->limit / 4, BS->limit / 4);
  putFileSegment(BS2, 3 * BS->limit / 4, BS->limit / 4);
  printf("Limit\t\tGot: %lld\t\t\tExpected: %lld\n", BS2->limit, BS->limit);
  printf("NumSegments\tGot: %d\t\t\t\tExpected: %d\n", BS2->readFileNumSegments, 4);

  printf("%s\n", H2);
  for (int i = 0; i < 4; ++i) {
    printf("Segment %d\tGot: %lld, %-16lld\tExpected: %lld, %lld\n", i,
        BS2->readFileSegments.begins[i], BS2->readFileSegments.lengths[i],
        i * BS->limit / 4, BS->limit / 4);
  }
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "getByte_1 Test (readFile mode)");
  printf("%s\n", H2);
  printf("Checking integrity of buffer with normalMode ByteStream\n");
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, 0), getByte_1(BS, 0));
  printf("%s\n", H2);
  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, 65534), getByte_1(BS, 65534));
  printf("%s\n", H2);
  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, 524287), getByte_1(BS, 524287));
  printf("%s\n", H2);
  printf("Index: limit-1\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, BS2->limit - 1), getByte_1(BS, BS->limit - 1));
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "getByte_0 Test (readFile mode)");
  printf("%s\n", H2);

  // Just don't want employ resets here for future tests
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS2, 0), ptr[0]);
  printf("%s\n", H2);
  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS2, 2047 - 1), ptr[2047]);
  printf("%s\n", H2);
  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS2, 65535 - 2048), ptr[65535]);
  printf("%s\n", H2);
  printf("Index: limit-1\t\t Got: %d\t\tExpected: %d\n", lookUpByte_0(BS2, BS2->limit - 1 - 65536), ptr[BS->limit - 1]);
  printf("%s\n", H2);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "endReadFileMode Test");
  printf("%s\n", H2);

  endReadFileMode(BS2);
  // From here, BS2 has streamMode = 0
  printf("Verifying both normalMode buffers\n");
  printf("%s\n", H2);
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[0], BS->buffer.array[0]);
  printf("%s\n", H2);
  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[65534], BS->buffer.array[65534]);
  printf("%s\n", H2);
  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[524287], BS->buffer.array[524287]);
  printf("%s\n", H2);
  printf("Index: limit-1\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[BS2->limit - 1], BS->buffer.array[BS->limit - 1]);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "saveToTemporalFile Test");
  printf("%s\n", H2);

  printf("Writing to the temporalFile\n");
  saveToTemporalFile(BS2, "../../files/");
  printf("%s\n", H2);
  printf("Checking integrity -> w0_c.tmp vs %s\n", BS2->temporalFileName);
  FileChannel fcInTmp0 = FileChannel_0("../../files/w0_c.tmp", FC_READ);
  FileChannel fcInTmp1 = FileChannel_0(BS2->temporalFileName, FC_READ);
  ByteBuffer tmpBuffTmp0 = {(signed char *) malloc(fcSize(&fcInTmp0)), fcSize(&fcInTmp0)};
  ByteBuffer tmpBuffTmp1 = {(signed char *) malloc(fcSize(&fcInTmp1)), fcSize(&fcInTmp1)};
  fcRead(&fcInTmp0, tmpBuffTmp0, 0);
  fcRead(&fcInTmp1, tmpBuffTmp1, 0);
  verifOut = 1;
  for (int i = 0; i < getLength(BS) / 2 && verifOut; ++i) {
    verifOut = tmpBuffTmp0.array[i] == tmpBuffTmp1.array[i];
  }
  printf("Result: %s\n", verifOut ? "OK" : "Error");
  fcClose(&fcInTmp0);
  fcClose(&fcInTmp1);
  free(tmpBuffTmp0.array);
  free(tmpBuffTmp1.array);

  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "write_0 (temporalFile mode) Test");
  printf("%s\n", H2);
  FileChannel FCTMP = FileChannel_0("../../files/wt_c.tmp", FC_WRITE);
  write_0(BS2, FCTMP);
  fcClose(&FCTMP);
  printf("%s\n", H2);
  printf("Checking integrity -> wt_c.tmp vs %s\n", BS2->temporalFileName);
  FileChannel fcInTmp2 = FileChannel_0("../../files/wt_c.tmp", FC_READ);
  FileChannel fcInTmp3 = FileChannel_0(BS2->temporalFileName, FC_READ);
  ByteBuffer tmpBuffTmp2 = {(signed char *) malloc(fcSize(&fcInTmp3)), fcSize(&fcInTmp2)};
  ByteBuffer tmpBuffTmp3 = {(signed char *) malloc(fcSize(&fcInTmp2)), fcSize(&fcInTmp3)};
  fcRead(&fcInTmp2, tmpBuffTmp2, 0);
  fcRead(&fcInTmp3, tmpBuffTmp3, 0);
  verifOut = 1;
  for (int i = 0; i < getLength(BS2) / 2 && verifOut; ++i) {
    verifOut = tmpBuffTmp2.array[i] == tmpBuffTmp3.array[i];
  }
  printf("Result: %s\n", verifOut ? "OK" : "Error");
  fcClose(&fcInTmp2);
  fcClose(&fcInTmp3);
  free(tmpBuffTmp2.array);
  free(tmpBuffTmp3.array);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "loadFromTemporalFile Test");
  printf("%s\n", H2);

  loadFromTemporalFile(BS2);
  printf("Verifying both normalMode buffers\n");
  printf("%s\n", H2);
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[0], BS->buffer.array[0]);
  printf("%s\n", H2);
  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[65534], BS->buffer.array[65534]);
  printf("%s\n", H2);
  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[524287], BS->buffer.array[524287]);
  printf("%s\n", H2);
  printf("Index: limit-1\t\t Got: %d\t\tExpected: %d\n", BS2->buffer.array[BS2->limit - 1], BS->buffer.array[BS->limit - 1]);
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "destroyTemporalFile Test");
  printf("%s\n", H2);
  printf("Destroying temporalFile %s\n", BS2->temporalFileName);
  destroyTemporalFile(BS2);
  printf("%s\n", BS2->temporalFileName == NULL ? "OK" : "Error");
  printf("%s\n", H1);
  /* **************************************************** */
  printf("%36s\n", "returnReadFileMode Test");
  printf("%s\n", H2);
  returnReadFileMode(BS2);
  printf("Index: 0\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, 0), getByte_1(BS, 0));
  printf("%s\n", H2);
  printf("Index: 65534\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, 65534), getByte_1(BS, 65534));
  printf("%s\n", H2);
  printf("Index: 524287\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, 524287), getByte_1(BS, 524287));
  printf("%s\n", H2);
  printf("Index: limit-1\t\t Got: %d\t\tExpected: %d\n", getByte_1(BS2, BS2->limit - 1), getByte_1(BS, BS->limit - 1));
  printf("%s\n", H1);
  /* **************************************************** */
  destroy(BS);
  destroy(BS2);
  free(BS);
  free(BS2);
  /* **************************************************** */
  return(0);
}
