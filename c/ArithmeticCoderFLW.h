#ifndef ACFLW_HH
#define ACFLW_HH

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ByteStream.h"

#define UPDATE_PROB0 7;
#define WINDOW_PROB 127;

const long BIT_MASKS[] = {0x0, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
const int BIT_MASKS2[] = {1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, \
                    1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, \
                    1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, \
                    1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24, \
                    1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30};

struct ACFLW_s {
  int  codewordLength;   // 0 < X < 64
  int  precisionBits;    // 0 < X < 64, codewordLength + precisionBits < 64
  long codewordMax;     // 2^codewordLength - 1
  int  codewordBytes;    // ceil(codewordLength / 8)
  int  precisionMid;     // 2^(precisionBits - 1)
  long intervalMin;     // 0 < X < 2^codewordLength - 1
  long intervalSize;    // 0 < interval - 1 < 2^codewordLength - 1
  long interval;        // intervalMin <= X <= intervalMin + intervalSize
  int  Tr;               // Flushed byte to the stream
  int  t;                // Number of bits to transfer
  int  L;                // Current position in the stream
  int  numContexts;      // Number of contexts
  ByteStream *stream;    // ByteStream employed by the coder, may contain zero bytes

  int* contextProb0FLW; // Current probability of each contex
  int* context0s;       // Number of 0s coded in this context
  int* context0sWindow; // Number of 0s coded in the last WINDOW_PROB symbols coded
  int* contextTotal;    // Total number of symbols coded in this context

  int replenishment;    // True indicates that replenishment with 0s is activated
} ACFLW_default = {     // Default values for initial "instance"
  32,     // codewordLength
  15,     // precisionBits
  -1,     // codewordMax
  -1,     // codewordBytes
  -1,     // precisionMid
  -1,     // intervalMin
  -1,     // intervalSize
  -1,     // interval
  -1,     // Tr
  -1,     // t
  -1,     // L
  -1,     // numContexts
  NULL,   // stream
  NULL,   // contextProb0FLW
  NULL,   // context0s
  NULL,   // context0sWindow
  NULL,   // contextTotal
  1       // replenishment
};

/**
 * Defines a default instance initialization using the following instruction:
 * ArithmeticCoderFLW ACFLW = ACFLW_default;
 */
typedef struct ACFLW_s ArithmeticCoderFLW;

void ArithmeticCoderFLW_0(ArithmeticCoderFLW *object);
void ArithmeticCoderFLW_1(ArithmeticCoderFLW *object, int codewordLength);
void ArithmeticCoderFLW_2(ArithmeticCoderFLW *object, int codewordLength, int precisionBits);
void ArithmeticCoderFLW_3(ArithmeticCoderFLW *object, int codewordLength, int precisionBits, int numContexts);

int prob0ToFLW(float prob0, int precisionBits);     // Tested
float FLWToProb0(int prob0FLW, int precisionBits);  // Tested

// void encodeBit(ArithmeticCoderFLW *object, int bit);
// int decodebit(ArithmeticCoderFLW *object);
// void encodeBitContext(ArithmeticCoderFLW *object, int bit, int context);
// int decodeBitContext(ArithmeticCoderFLW *object, int context);
// void encodeBitProb(ArithmeticCoderFLW *object, int bit, int prob0FLW);
// int decodeBitProb(ArithmeticCoderFLW *object, int prob0FLW);
// void encodeInteger(ArithmeticCoderFLW *object, int num, int numBits);
// int decodeInteger(ArithmeticCoderFLW *object, int numBits);
// void transferInterval(ArithmeticCoderFLW *object, int length);
// void fillInterval(ArithmeticCoderFLW *object);
// void changeStream(ByteStream *stream);

void ArithmeticCoderFLW_reset(ArithmeticCoderFLW *object);
void restartEncoding(ArithmeticCoderFLW *object);
void restartDecoding(ArithmeticCoderFLW *object);
// void terminate(ArithmeticCoderFLW *object);
int remainingBytes(ArithmeticCoderFLW *object);
int getReadBytes(ArithmeticCoderFLW *object);
void setReplenishment(ArithmeticCoderFLW *object, int replenishment);

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 */
void ArithmeticCoderFLW_0(ArithmeticCoderFLW *object) {
  assert(object->codewordLength > 0);
  assert(object->precisionBits > 0);
  assert(object->codewordLength + object->precisionBits < 64);

  object->codewordMax = ((long) 1 << object->codewordLength) - 1;
  object->codewordBytes = (int) ceil((float) object->codewordLength / 8.f);
  object->precisionMid = 1 << (object->precisionBits - 1);
  restartEncoding(object);
}

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 *
 * @param codewordLength number of bits of the codeword
 */
void ArithmeticCoderFLW_1(ArithmeticCoderFLW *object, int codewordLength) {
  object->codewordLength = codewordLength;

  assert(object->codewordLength > 0);
  assert(object->precisionBits > 0);
  assert(object->codewordLength + object->precisionBits < 64);

  object->codewordMax = ((long) 1 << object->codewordLength) - 1;
  object->codewordBytes = (int) ceil((float) object->codewordLength / 8.f);
  object->precisionMid = 1 << (object->precisionBits - 1);
  restartEncoding(object);
}

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 *
 * @param codewordLength number of bits of the codeword
 * @param precisionBits number of bits employed in the probabilities
 */
void ArithmeticCoderFLW_2(ArithmeticCoderFLW *object, int codewordLength, int precisionBits) {
  object->codewordLength = codewordLength;
  object->precisionBits = precisionBits;

  assert(object->codewordLength > 0);
  assert(object->precisionBits > 0);
  assert(object->codewordLength + object->precisionBits < 64);

  object->codewordMax = ((long) 1 << object->codewordLength) - 1;
  object->codewordBytes = (int) ceil((float) object->codewordLength / 8.f);
  object->precisionMid = 1 << (object->precisionBits - 1);
  restartEncoding(object);
}

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 *
 * @param codewordLength number of bits of the codeword
 * @param precisionBits number of bits employed in the probabilities
 * @param numContexts number of contexts available for this object
 */
void ArithmeticCoderFLW_3(ArithmeticCoderFLW *object, int codewordLength, int precisionBits, int numContexts) {
  object->codewordLength = codewordLength;
  object->precisionBits = precisionBits;
  object->numContexts = numContexts;

  assert(object->codewordLength > 0);
  assert(object->precisionBits > 0);
  assert(object->numContexts > 0);
  assert(object->codewordLength + object->precisionBits < 64);

  object->codewordMax = ((long) 1 << object->codewordLength) - 1;
  object->codewordBytes = (int) ceil((float) object->codewordLength / 8.f);
  object->precisionMid = 1 << (object->precisionBits - 1);
  object->contextProb0FLW = (int *) malloc(object->numContexts * sizeof(int));
  object->context0s = (int *) malloc(object->numContexts * sizeof(int));
  object->context0sWindow = (int *) malloc(object->numContexts * sizeof(int));
  object->contextTotal = (int *) malloc(object->numContexts * sizeof(int));
  ArithmeticCoderFLW_reset(object);
  restartEncoding(object);
}

/**
 * Transforms the probability of the symbol 0 (or false) in the range (0,1) into the integer
 * required in the FLW coder to represent that probability.
 *
 * @param prob0 in the range [0:1]. If 0 or 1, the closest probability to 0/1 given the
 * precision bits employed will be used --but it will not be 0 or 1.
 * @param precisionBits number of bits employed to represent the probability in the
 * returned integer
 * @return integer that can be fed to the FLW coder. It is computed as (2^precisionBits) * prob0.
 */
int prob0ToFLW(float prob0, int precisionBits) {
  assert((prob0 >= 0.f) && (prob0 <= 1.f));

  int prob0FLW = (int) ((float) (1 << precisionBits) * prob0);
  if(prob0FLW == 0) {
    prob0FLW = 1;
  } else if(prob0FLW == (1 << precisionBits)) {
    prob0FLW = (1 << precisionBits) - 1;
  }
  assert((prob0FLW > 0) && (prob0FLW < (1 << precisionBits)));
  return(prob0FLW);
}

/**
 * Transforms the FLW integer to the probability of the symbol 0 (or false) in the range (0,1).
 *
 * @param prob0FLW integer fed to the FLW coder. It is (2^precisionBits) * prob0.
 * @param precisionBits number of bits employed to represent the probability in prob0FLW. It
 * is recommended to be, at least, equal to the codewordLength. To use fewer bits than the
 * codewordLength may decrease compression efficiency for very high/low probabilities,
 * and to use more is inconsequential. Also, be aware that codewordLength + precisionBits < 64.
 * @return prob0 in the range (0:1)
 */
float FLWToProb0(int prob0FLW, int precisionBits) {
  assert((prob0FLW > 0) && (prob0FLW < (1 << precisionBits)));

  float prob0 = (float) prob0FLW / (float) (1 << precisionBits);
    assert((prob0 > 0.f) && (prob0 < 1.f));
  return(prob0);
}

/**
 * Resets the state of all contexts.
 */
void ArithmeticCoderFLW_reset(ArithmeticCoderFLW *object) {
  for(int c = 0; c < object->numContexts; c++) {
    object->contextProb0FLW[c] = prob0ToFLW(0.66f, object->precisionBits); //Slightly biased towards 0
    object->context0s[c] = 2;
    object->context0sWindow[c] = -1;
    object->contextTotal[c] = 3;
  }
}

/**
 * Restarts the internal registers of the coder for encoding.
 */
void restartEncoding(ArithmeticCoderFLW *object) {
  object->intervalMin = 0;
  object->intervalSize = object->codewordMax;
  object->Tr = 0;
  object->t = 8;
  object->L = -1;
}

/**
 * Restarts the internal registers of the coder for decoding.
 *
 */
void restartDecoding(ArithmeticCoderFLW *object) {
  object->intervalMin = 0;
  object->intervalSize = 0;
  object->Tr = 0;
  object->L = 0;
  object->t = 0;
}

/**
 * Computes the number of bytes belonging to the currently encoded data needed to flush
 * the internal registers (for encoding purposes). This function is useful to determine
 * potential truncation points of the stream.
 *
 * @return number of bytes
 */
int remainingBytes(ArithmeticCoderFLW *object) {
  if(object->t == 8) {
    return(object->codewordBytes);
  } else {
    return(object->codewordBytes + 1);
  }
}

/**
 * Gets the number of bytes read or written to the stream associated to the coder.
 *
 * @return number of bytes
 */
int getReadBytes(ArithmeticCoderFLW *object) {
  return(object->L);
}

/**
 * Sets the {@link #replenishment} parameter.
 *
 * @param replenishment the new value for the parameter
 */
void setReplenishment(ArithmeticCoderFLW *object, int replenishment) {
  object->replenishment = replenishment;
}

#endif /* ACFLW_HH */
