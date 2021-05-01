#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../src/ArithmeticCoderFLW.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"

// Message contains "ARITHMETIC"
const signed char code_table[256] = {
  // [0 ... 255] = 255,
  ['A'] = 4,
  ['R'] = 5,
  ['I'] = 6,
  ['T'] = 7,
  ['H'] = 8,
  ['M'] = 9,
  ['E'] = 10,
  ['C'] = 11,
};

signed char lookupTable(int flw) {
  for (signed char i = 0; i < 256; ++i) {
    // The index(i) is the symbol
    if (code_table[i] == flw) return i;
  }
  return -1;
}

int main(int argc, char *argv[]) {
  printf("Initializing ArithmeticCoderFLW\n");

  /*
   * Initialize with:
   *  - codewordLength ->  3
   *  - precisionBits  ->  8
   *  - numContexts    ->  1
   */
  ArithmeticCoderFLW *ACN = ArithmeticCoderFLW_3(4, 8, 1);

  printf("Initializing ByteStream (normal mode)\n");
  /*
   * Initialize with:
   *  - initialAllocation = INITIAL_ALLOCATION
   *  - streamMode        = 0
   */
  ByteStream *BSN = ByteStream_1(INITIAL_ALLOCATION);

  // Links normal-mode ByteStream to the ArithmeticCoderFLW
  changeStream(ACN, BSN);
  /* **************************************************** */

  printf("Encoding the message: \"%s\"\n", "ARITHMETIC");

  /*
   * Encoding the Message
   *  A - R - I - T - H - M - E - T - I - C
   */
  for (int i = 0; i < 10; ++i) {
    encodeInteger(ACN, code_table['A'], 4);
    encodeInteger(ACN, code_table['R'], 4);
    encodeInteger(ACN, code_table['I'], 4);
    encodeInteger(ACN, code_table['T'], 4);
    encodeInteger(ACN, code_table['H'], 4);
    encodeInteger(ACN, code_table['M'], 4);
    encodeInteger(ACN, code_table['E'], 4);
    encodeInteger(ACN, code_table['T'], 4);
    encodeInteger(ACN, code_table['I'], 4);
    encodeInteger(ACN, code_table['C'], 4);
  }

  /* **************************************************** */
  terminate(ACN);
  restartDecoding(ACN);
  /* **************************************************** */

  printf("Decoding %lld bytes\n", BSN->limit);
  signed char *aux = (signed char *) malloc(BSN->limit * sizeof(signed char));
  for (int i = 0; i < BSN->limit; ++i) {
    // Looks for a match in code_table[] for the 3-bits flw
    aux[i] = lookupTable(decodeInteger(ACN, 4));
  }

  // Should print ARITHMETIC
  long long pad = 0;
  printf("Showing %lld decoded bytes:\n\t", BSN->limit);
  for(int i = 0; i < BSN->limit; ++i) {
    printf("%c", aux[i]);
    // Line break every 10 signed characters (length of "ARITHMETIC")
    pad++;
    if (pad % 10 == 0) printf("\n\t");
  }
  printf("\n");
  free(aux);

  // Writes the whole BSN->limit bytes into a file for next test
  FileChannel FCN = FileChannel_0("../../files/full_c.tmp", "w");
  write_0(BSN, FCN);
  fcClose(&FCN);

  /* **************************************************** */

  /*
   * Initialize with:
   *  - codewordLength ->  3
   *  - precisionBits  ->  8
   *  - numContexts    ->  1
   */
  ArithmeticCoderFLW *ACR = ArithmeticCoderFLW_3(4, 8, 1);

  // Using the previously coded stream written into the file
  FileChannel FCR = FileChannel_0("../../files/full_c.tmp", "r");

  printf("Initializing ByteStream (readFile mode)\n");
  /*
   * Initialize with:
   *  - readFileChannel = FCR
   *  - streamMode      = 1
   */
  ByteStream *BSR = ByteStream_2(FCR);

  // Use only one segment
  putFileSegment(BSR, 0L, BSN->limit);

  // Links normal-mode ByteStream to the ArithmeticCoderFLW
  changeStream(ACR, BSR);
  restartDecoding(ACR);
  /* **************************************************** */

  printf("Decoding %lld bytes\n", BSR->limit);
  signed char *aux2 = (signed char *) malloc(BSR->limit * sizeof(signed char));
  for (int i = 0; i < BSR->limit; ++i) {
    // Looks for a match in code_table[] for the 3-bits flw
    aux2[i] = lookupTable(decodeInteger(ACR, 4));
  }

  // Should print ARITHMETIC
  long long pad2 = 0;
  printf("Showing %lld decoded bytes:\n\t", BSR->limit);
  for(int i = 0; i < BSR->limit; ++i) {
    printf("%c", aux2[i]);
    // Line break every 10 signed characters (length of "ARITHMETIC")
    pad2++;
    if (pad2 % 10 == 0) printf("\n\t");
  }
  printf("\n");
  free(aux2);

  /* **************************************************** */
  // Won't need these ArithmeticCoderFLW + ByteStream anymore
  destroy(BSN);
  free(BSN);
  free(ACN);

  destroy(BSR);
  free(BSR);
  free(ACR);
  /* **************************************************** */
  return 0;
}
