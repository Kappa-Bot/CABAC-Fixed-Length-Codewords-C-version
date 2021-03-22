#ifndef ACFLW_HH
#define ACFLW_HH


#ifndef BS_HH
#include "ByteStream.h"
#endif

#define UPDATE_PROB0 7
#define WINDOW_PROB 127

extern const long BIT_MASKS[];
extern const int BIT_MASKS2[];

extern struct ACFLW_s {
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
} ACFLW_default;

/**
 * Defines a default instance initialization using the following instruction:
 *  memcpy(ACFLWNewInstance, &ACFLW_default, sizeof(ArithmeticCoderFLW));
 */
typedef struct ACFLW_s ArithmeticCoderFLW;

extern ArithmeticCoderFLW *ArithmeticCoderFLWInit();                 // No test needed

extern ArithmeticCoderFLW *ArithmeticCoderFLW_0();                   // No test needed
extern ArithmeticCoderFLW *ArithmeticCoderFLW_1(int codewordLength); // No test needed
extern ArithmeticCoderFLW *ArithmeticCoderFLW_2(int codewordLength,
                                                int precisionBits);
extern ArithmeticCoderFLW *ArithmeticCoderFLW_3(int codewordLength,  // No test needed
                                                int precisionBits,
                                                int numContexts);

extern int prob0ToFLW(float prob0, int precisionBits);               // Tested
extern float FLWToProb0(int prob0FLW, int precisionBits);            // Tested

extern void encodeBit(ArithmeticCoderFLW *object, int bit);
extern int decodeBit(ArithmeticCoderFLW *object);

extern void encodeBitContext(ArithmeticCoderFLW *object,
                            int bit, int context);
extern int decodeBitContext(ArithmeticCoderFLW *object,
                            int context);

extern void encodeBitProb(ArithmeticCoderFLW *object,
                            int bit, int prob0FLW);
extern int decodeBitProb(ArithmeticCoderFLW *object,
                            int prob0FLW);

extern void encodeInteger(ArithmeticCoderFLW *object,
                            int num, int numBits);
extern int decodeInteger(ArithmeticCoderFLW *object,
                            int numBits);

extern void transferInterval(ArithmeticCoderFLW *object,
                            int length);
extern void fillInterval(ArithmeticCoderFLW *object);

extern void changeStream(ArithmeticCoderFLW *object,                 // No test needed
                            ByteStream *stream);

extern void ArithmeticCoderFLW_reset(ArithmeticCoderFLW *object);    // No test needed
extern void restartEncoding(ArithmeticCoderFLW *object);             // No test needed
extern void restartDecoding(ArithmeticCoderFLW *object);             // No test needed

extern void terminate(ArithmeticCoderFLW *object);

extern int remainingBytes(ArithmeticCoderFLW *object);               // No test needed
extern int getReadBytes(ArithmeticCoderFLW *object);                 // No test needed
extern void setReplenishment(ArithmeticCoderFLW *object,             // No test needed
                    int replenishment);


#endif /* ACFLW_HH */
