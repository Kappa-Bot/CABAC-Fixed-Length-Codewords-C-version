#ifndef ACFLW_HH
#define ACFLW_HH

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "ByteStream.h"

#define UPDATE_PROB0 7
#define WINDOW_PROB 127

const long BIT_MASKS[] = {0x0, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
const int BIT_MASKS2[] = {1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6, \
                    1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12, \
                    1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18, \
                    1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24, \
                    1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30};

struct ACFLW_s {
  int  codewordLength;  // 0 < X < 64
  int  precisionBits;   // 0 < X < 64, codewordLength + precisionBits < 64
  long codewordMax;     // 2^codewordLength - 1
  int  codewordBytes;   // ceil(codewordLength / 8)
  int  precisionMid;    // 2^(precisionBits - 1)
  long intervalMin;     // 0 < X < 2^codewordLength - 1
  long intervalSize;    // 0 < interval - 1 < 2^codewordLength - 1
  long interval;        // intervalMin <= X <= intervalMin + intervalSize
  int  Tr;              // Flushed byte to the stream
  int  t;               // Number of bits to transfer
  int  L;               // Current position in the stream
  int  numContexts;     // Number of contexts
  ByteStream *stream;   // ByteStream employed by the coder, may contain zero bytes

  int* contextProb0FLW; // Current probability of each contex
  int* context0s;       // Number of 0s coded in this context
  int* context0sWindow; // Number of 0s coded in the last WINDOW_PROB symbols coded
  int* contextTotal;    // Total number of symbols coded in this context

  int replenishment;    // True indicates that replenishment with 0s is activated
} ACFLW_default = {     // Default values for initial "instance"
  32,                   // codewordLength
  15,                   // precisionBits
  -1,                   // codewordMax
  -1,                   // codewordBytes
  -1,                   // precisionMid
  -1,                   // intervalMin
  -1,                   // intervalSize
  -1,                   // interval
  -1,                   // Tr
  -1,                   // t
  -1,                   // L
  -1,                   // numContexts
  NULL,                 // stream
  NULL,                 // contextProb0FLW
  NULL,                 // context0s
  NULL,                 // context0sWindow
  NULL,                 // contextTotal
  1                     // replenishment
};

/**
 * Defines a default instance initialization using the following instruction:
 *  memcpy(ACFLWNewInstance, &ACFLW_default, sizeof(ArithmeticCoderFLW));
 */
typedef struct ACFLW_s ArithmeticCoderFLW;

ArithmeticCoderFLW *ArithmeticCoderFLWInit();                 // No test needed

ArithmeticCoderFLW *ArithmeticCoderFLW_0();                   // No test needed
ArithmeticCoderFLW *ArithmeticCoderFLW_1(int codewordLength); // No test needed
ArithmeticCoderFLW *ArithmeticCoderFLW_2(int codewordLength,
                                        int precisionBits);
ArithmeticCoderFLW *ArithmeticCoderFLW_3(int codewordLength,  // No test needed
                                        int precisionBits,
                                        int numContexts);

int prob0ToFLW(float prob0, int precisionBits);               // Tested
float FLWToProb0(int prob0FLW, int precisionBits);            // Tested

void encodeBit(ArithmeticCoderFLW *object, int bit);
int decodeBit(ArithmeticCoderFLW *object);
void encodeBitContext(ArithmeticCoderFLW *object,
                    int bit,
                    int context);
int decodeBitContext(ArithmeticCoderFLW *object,
                    int context);
void encodeBitProb(ArithmeticCoderFLW *object,
                    int bit,
                    int prob0FLW);
int decodeBitProb(ArithmeticCoderFLW *object,
                    int prob0FLW);
void encodeInteger(ArithmeticCoderFLW *object,
                    int num,
                    int numBits);
int decodeInteger(ArithmeticCoderFLW *object,
                    int numBits);
void transferInterval(ArithmeticCoderFLW *object,
                    int length);
void fillInterval(ArithmeticCoderFLW *object);
void changeStream(ArithmeticCoderFLW *object,                 // No test needed
                    ByteStream *stream);

void ArithmeticCoderFLW_reset(ArithmeticCoderFLW *object);    // No test needed
void restartEncoding(ArithmeticCoderFLW *object);             // No test needed
void restartDecoding(ArithmeticCoderFLW *object);             // No test needed
void terminate(ArithmeticCoderFLW *object);
int remainingBytes(ArithmeticCoderFLW *object);               // No test needed
int getReadBytes(ArithmeticCoderFLW *object);                 // No test needed
void setReplenishment(ArithmeticCoderFLW *object,             // No test needed
                    int replenishment);

ArithmeticCoderFLW *ArithmeticCoderFLWInit() {
  ArithmeticCoderFLW *object = (ArithmeticCoderFLW *) malloc(sizeof(ArithmeticCoderFLW));
  memcpy(object, &ACFLW_default, sizeof(ArithmeticCoderFLW));
  return object;
}

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 */
ArithmeticCoderFLW *ArithmeticCoderFLW_0() {
  ArithmeticCoderFLW *object = ArithmeticCoderFLWInit();
  assert(object->codewordLength > 0);
  assert(object->precisionBits > 0);
  assert(object->codewordLength + object->precisionBits < 64);

  object->codewordMax = ((long) 1 << object->codewordLength) - 1;
  object->codewordBytes = (int) ceil((float) object->codewordLength / 8.f);
  object->precisionMid = 1 << (object->precisionBits - 1);
  restartEncoding(object);
  return object;
}

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 *
 * @param codewordLength number of bits of the codeword
 */
ArithmeticCoderFLW *ArithmeticCoderFLW_1(int codewordLength) {
  ArithmeticCoderFLW *object = ArithmeticCoderFLWInit();
  object->codewordLength = codewordLength;
  assert(object->codewordLength > 0);
  assert(object->precisionBits > 0);
  assert(object->codewordLength + object->precisionBits < 64);
  object->codewordMax = ((long) 1 << object->codewordLength) - 1;
  object->codewordBytes = (int) ceil((float) object->codewordLength / 8.f);
  object->precisionMid = 1 << (object->precisionBits - 1);
  restartEncoding(object);
  return object;
}

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 *
 * @param codewordLength number of bits of the codeword
 * @param precisionBits number of bits employed in the probabilities
 */
ArithmeticCoderFLW *ArithmeticCoderFLW_2(int codewordLength, int precisionBits) {
  ArithmeticCoderFLW *object = ArithmeticCoderFLWInit();
  object->codewordLength = codewordLength;
  object->precisionBits = precisionBits;
  assert(object->codewordLength > 0);
  assert(object->precisionBits > 0);
  assert(object->codewordLength + object->precisionBits < 64);
  object->codewordMax = ((long) 1 << object->codewordLength) - 1;
  object->codewordBytes = (int) ceil((float) object->codewordLength / 8.f);
  object->precisionMid = 1 << (object->precisionBits - 1);
  restartEncoding(object);
  return object;
}

/**
 * Initializes internal registers. Before using the coder, a stream has to be set
 * through <code>changeStream</code>.
 *
 * @param codewordLength number of bits of the codeword
 * @param precisionBits number of bits employed in the probabilities
 * @param numContexts number of contexts available for this object
 */
ArithmeticCoderFLW *ArithmeticCoderFLW_3(int codewordLength,
                                        int precisionBits,
                                        int numContexts) {
  ArithmeticCoderFLW *object = ArithmeticCoderFLWInit();
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
  return object;
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
 * Encodes a bit without using any probability model.
 *
 * @param bit input
 */
void encodeBit(ArithmeticCoderFLW *object, int bit){
  encodeBitProb(object, bit, object->precisionMid);
}

/**
 * Decodes a bit without using any probability model.
 *
 * @return bit output
 * @throws Exception when some problem manipulating the stream occurs
 */
int decodeBit(ArithmeticCoderFLW *object) {
  return(decodeBitProb(object, object->precisionMid));
}


/**

 * Encodes a bit using a context so that the probabilities are adaptively adjusted
 * depending on the incoming symbols.
 *
 * @param bit input
 * @param context context of the symbol
 */
void encodeBitContext(ArithmeticCoderFLW *object, int bit, int context) {
	//Updates the probability of this context, when necessary
	if((object->contextTotal[context] & UPDATE_PROB0) == UPDATE_PROB0) {
		if(object->context0s[context] == 0) {
			object->contextProb0FLW[context] = 1;
		} else if(object->context0s[context] == object->contextTotal[context]) {
			object->contextProb0FLW[context] = (1 << object->precisionBits) - 1;
		} else {
			object->contextProb0FLW[context] =
        (object->context0s[context] << object->precisionBits)
        / object->contextTotal[context];
		}
		assert((object->contextProb0FLW[context] > 0) &&
      (object->contextProb0FLW[context] < (1 << object->precisionBits)));
		if((object->contextTotal[context] & WINDOW_PROB) == WINDOW_PROB) {
			if(object->context0sWindow[context] != -1) {
				object->contextTotal[context] -= WINDOW_PROB;
				object->context0s[context] -= object->context0sWindow[context];
			}
			object->context0sWindow[context] = object->context0s[context];
		}
	}

	//Encodes the bit
	encodeBitProb(object, bit, object->contextProb0FLW[context]);

	//Updates the number of symbols coded for this context
	if(bit == 0) {
		object->context0s[context]++;
	}
	object->contextTotal[context]++;
}

/**
 * Decodes a bit using a context so that the probabilities are adaptively adjusted
 * depending on the outcoming symbols.
 *
 * @param context context of the symbols
 * @return output bit
 * @throws Exception when some problem manipulating the stream occurs
 */
int decodeBitContext(ArithmeticCoderFLW *object, int context) {
	//Updates the probability of this context, when necessary
	if((object->contextTotal[context] & UPDATE_PROB0) == UPDATE_PROB0) {
		if(object->context0s[context] == 0) {
			object->contextProb0FLW[context] = 1;
		} else if(object->context0s[context] == object->contextTotal[context]) {
			object->contextProb0FLW[context] = (1 << object->precisionBits) - 1;
		} else {
			object->contextProb0FLW[context] = (object->context0s[context] << object->precisionBits) / object->contextTotal[context];
		}
		assert((object->contextProb0FLW[context] > 0) && (object->contextProb0FLW[context] < (1 << object->precisionBits)));
		if((object->contextTotal[context] & WINDOW_PROB) == WINDOW_PROB) {
			if(object->context0sWindow[context] != -1) {
				object->contextTotal[context] -= WINDOW_PROB;
				object->context0s[context] -= object->context0sWindow[context];
			}
			object->context0sWindow[context] = object->context0s[context];
		}
	}

	//Decodes the bit
	int bit = decodeBitProb(object, object->contextProb0FLW[context]);

	//Updates the number of symbols coded for this context
	if(bit == 0) {
		object->context0s[context]++;
	}
	object->contextTotal[context]++;
	return(bit);
}

/**
 * Encodes a bit using a specified probability.
 *
 * @param bit input
 * @param prob0FLW probability of bit == false. This probability is generated via {@link #prob0ToFLW}
 */
void encodeBitProb(ArithmeticCoderFLW *object, int bit, int prob0FLW){
  assert((prob0FLW > 0) && (prob0FLW < (1 << object->precisionBits)));
  assert(object->intervalSize >= 1);

  if(bit == 0) {
    object->intervalSize = ((long) prob0FLW * object->intervalSize) >> object->precisionBits;
  } else {
    long tmp = (((long) prob0FLW * object->intervalSize) >> object->precisionBits) + 1;
    object->intervalMin += tmp;
    object->intervalSize -= tmp;
  }

  if(object->intervalSize == 0) {
    object->interval = object->intervalMin;
    transferInterval(object, object->codewordLength);
    object->intervalMin = 0;
    object->intervalSize = object->codewordMax;
  }
}

/**
 * Decodes a bit using a specified probability.
 *
 * @param prob0FLW probability of bit == false. This probability is generated via {@link #prob0ToFLW}
 * @return bit decoded bit
 * @throws Exception when some problem manipulating the stream occurs
 */
int decodeBitProb(ArithmeticCoderFLW *object, int prob0FLW) {
  assert((prob0FLW > 0) && (prob0FLW < (1 << object->precisionBits)));

  if(object->intervalSize == 0) {
    fillInterval(object);
    object->intervalMin = 0;
    object->intervalSize = object->codewordMax;
  }
  assert(object->intervalSize >= 1);

  long tmp = (((long) prob0FLW * object->intervalSize) >> object->precisionBits) + 1;
  long mid = object->intervalMin + tmp;
  int bit;
  if(object->interval >= mid) {
    bit = 1;
    object->intervalMin = mid;
    object->intervalSize -= tmp;
  } else {
    bit = 0;
    object->intervalSize = tmp - 1;
  }
  return(bit);
}

/**
 * Encodes an integer without using any probability model.
 *
 * @param num input
 * @param numBits number of bits coded for that integer
 */
void encodeInteger(ArithmeticCoderFLW *object, int num, int numBits) {
  assert(num >= 0);
  assert(num < (1 << (numBits + 1)));

  for(int bit = numBits - 1; bit >= 0; bit--) {
    int realBit = (num & BIT_MASKS2[bit]) != 0;
    encodeBit(object, realBit);
  }
}

/**
 * Decodes an integer without using any probability model.
 *
 * @param numBits number of bits decoded for that integer
 * @return output integer
 * @throws Exception when some problem manipulating the stream occurs
 */
int decodeInteger(ArithmeticCoderFLW *object, int numBits) {
  int num = 0;
  for(int bit = numBits - 1; bit >= 0; bit--){
    if(decodeBit(object)) {
      num += BIT_MASKS2[bit];
    }
  }
  return(num);
}

/**
 * Transfers the interval to the stream (for encoding only).
 *
 * @param length number of bits of the interval to be transferred
 */
void transferInterval(ArithmeticCoderFLW *object, int length) {
  int transferredBits = 0;
  while(transferredBits < length) {
    int remainingBits = object->codewordLength - transferredBits;
    int transfer = remainingBits <= object->t? remainingBits: object->t;

    if((remainingBits - object->t) >= 0) {
      object->Tr |= (object->interval >> (remainingBits - object->t)) & BIT_MASKS[transfer];
    } else {
      object->Tr |= (object->interval & BIT_MASKS[transfer]) << (object->t - remainingBits);
    }

    object->t -= transfer;
    if(object->t == 0) {
      putByte(object->stream, (unsigned char) object->Tr);
      if(object->Tr == 0xFF) {
        object->t = 7;
      } else {
        object->t = 8;
      }
      object->Tr = 0;
      object->L++;
    }
    assert(object->t >= 0 && object->t <= 8);

    transferredBits += transfer;
  }
}

/**
 * Reads the coded interval from the stream (for decoding only). If at the middle
 * of reading the interval, the stream runs out of bytes, this function fills the
 * least significant bits of the codeword with 0s.
 */
void fillInterval(ArithmeticCoderFLW *object) {
  if(!object->replenishment && (object->L >= getLength(object->stream))) {
    //This placed here to throw the exception only when trying to fill a new interval
    fprintf(stderr, "No more data in the stream to fill the interval.");
  }

  int readBits = 0;
  object->interval = 0;
  do {
    if(object->t == 0) {
      if(object->L < getLength(object->stream)) {
        if(object->Tr == 0xFF) {
          object->t = 7;
        } else {
          object->t = 8;
        }
        object->Tr = (0x000000FF & getByte_1(object->stream, object->L));
        object->L++;
      } else {
        //Fills with 0s
        object->Tr = 0;
        object->t = 8;
      }
    }

    int remainingBits = object->codewordLength - readBits;
    int read = remainingBits <= object->t? remainingBits: object->t;

    if((remainingBits - object->t) >= 0) {
      object->interval |= ((long) object->Tr & BIT_MASKS[read]) << (remainingBits - object->t);
    } else {
      object->interval |= ((long) object->Tr >> (object->t - remainingBits)) & BIT_MASKS[read];
    }
    assert(object->interval >= 0 && object->interval <= object->codewordMax);

    object->t -= read;
    assert(object->t >= 0 && object->t <= 8);

    readBits += read;
  } while(readBits < object->codewordLength);
}

/**
 * Changes the current stream. When encoding, before calling this function the stream
 * should be terminated calling the <code>terminate</code> function, and after calling
 * this function the function <code>restartEncoding</code> must be called. When decoding,
 * after calling this function the function <code>restartDecoding</code> must be called.
 *
 * @param stream the new ByteStream
 */
void changeStream(ArithmeticCoderFLW *object, ByteStream *stream) {
  object->stream = stream == NULL ? ByteStream_0(stream) : stream;
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
 * Terminates the current stream using an optimal termination (for encoding purposes).
 *
 * @throws Exception when some problem manipulating the stream occurs
 */
void terminate(ArithmeticCoderFLW *object) {
  long interval1 = 0;
  long interval2 = 0;
  int bits = object->codewordLength;
  long intervalMax = object->intervalMin + object->intervalSize;
  while(((interval1 < object->intervalMin) || (interval1 > intervalMax))
    && ((interval2 < object->intervalMin) || (interval2 > intervalMax))){
    bits--;
    interval1 |= ((long) 1 << bits) & object->intervalMin;
    interval2 |= ((long) 1 << bits) & intervalMax;
  }

  if((interval1 >= object->intervalMin) && (interval1 <= intervalMax)){
    object->interval = interval1;
  } else {
    assert((interval2 >= object->intervalMin) && (interval2 <= intervalMax));
    object->interval = interval2;
  }
  assert(bits >= 0);

  transferInterval(object, object->codewordLength - bits);
  if(object->t != 8){
    putByte(object->stream, (unsigned char) object->Tr);
    object->Tr = 0;
    object->t = 8;
  }
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
