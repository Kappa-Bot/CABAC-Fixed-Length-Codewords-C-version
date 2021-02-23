#ifndef ACFLW_H
#define ACFLW_H

void defaultInitACFLW();
int prob0ToFLW(float prob0, int precisionBits);
float FLWToProb0(int prob0FLW, int precisionBits);
void encodeBit(int bit);
int decodeBit();
void encodeBitContext(int bit, int context);
int decodeBitContext(int context);
void encodeBitProb(int bit, int prob0FLW);
int decodeBitProb(int prob0FLW);
void encodeInteger(int num, int numBits);
int decodeInteger(int numBits);
void transferInterval(int length);
void fillInterval();
void changeStream(FILE *stream); // ByteStream stream
void reset();
void restartEncoding();
void restartDecoding();
void terminate();
int remainingBytes();
int getReadBytes();
void setReplenishment(int replenishment);

int UPDATE_PROB0 = 7;
int WINDOW_PROB = 127;
int codewordLength = 32;
int precisionBits = 15;
long codewordMax = -1;
int codewordBytes = -1;
int precisionMid = -1;
long intervalMin = -1;
long intervalSize = -1;
long interval = -1;
int Tr = -1;
int t  = -1;
int L;
int numContexts = -1;

int* contextProb0FLW; // maybe an equivalent lol
int* context0s;       // maybe an equivalent lol
int* context0sWindow; // maybe an equivalent lol
int* contextTotal;    // maybe an equivalent lol

int replenishment = 1;
long BIT_MASKS[] = {0x0, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
int BIT_MASKS2[] = {1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6,
                    1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12,
                    1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18,
                    1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24,
                    1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30};
#endif /* ACFLW_H */

#ifndef BS_H
#define BS_H
int INITIAL_ALLOCATION = 1024;
int INITIAL_NUM_SEGMENTS = 32;
int streamMode = -1;
unsigned char* buffer;
long limit = 0;
long position = 0;

//private FileChannel readFileChannel = null; // ... AAAAAAAAAA
FILE *readFileChannel = NULL;
int readFileNumSegments = -1;
long** readFileSegments;
long temporalFilePosition = -1;
char *temporalFileName = NULL;

/**
 * Temporal ByteBuffer employed to read from the file channel when necessary (only used when
 * the stream is in readFile mode).
 * <p>
 * This object is reused every time a new byte is read from the file.
 */
unsigned char temporalBuffer = '0';

/**
 * This object is useful to lock the access to the variables shared by multiple threads
 * (i.e., the <code>temporalFileName</code>).
 * <p>
 * The object is never modified, its only purpose is to allow a lock.
 */
unsigned char lock; // Probably bein replaced by an OpenMP thread-safe locker
#endif /* BS_H */



#include <stdio.h>
#include <assert.h>
#include <math.h>

void defaultInitACFLW() {
  codewordMax = ((long) 1 << codewordLength) - 1;
  codewordBytes = (int) ceil((float) codewordLength / 8.f);
  precisionMid = 1 << (precisionBits - 1);
  restartEncoding();
}

int prob0ToFLW(float prob0, int precisionBits){
  assert((prob0 >= 0.f) && (prob0 <= 1.f));

  int prob0FLW = (int) ((float) (1 << precisionBits) * prob0);
  if(prob0FLW == 0){
    prob0FLW = 1;
  }else if(prob0FLW == (1 << precisionBits)){
    prob0FLW = (1 << precisionBits) - 1;
  }
  assert((prob0FLW > 0) && (prob0FLW < (1 << precisionBits)));
  return(prob0FLW);
}

float FLWToProb0(int prob0FLW, int precisionBits){
  assert((prob0FLW > 0) && (prob0FLW < (1 << precisionBits)));

  float prob0 = (float) prob0FLW / (float) (1 << precisionBits);
  assert((prob0 > 0.f) && (prob0 < 1.f));
  return(prob0);
}

void encodeBit(int bit){ // Integers are literally interpreted as booleans
  encodeBitProb(bit, precisionMid);
}

int decodeBit(){
  return(decodeBitProb(precisionMid));
}

void encodeBitContext(int bit, int context){
  //Updates the probability of this context, when necessary
  if((contextTotal[context] & UPDATE_PROB0) == UPDATE_PROB0){
    if(context0s[context] == 0){
      contextProb0FLW[context] = 1;
    }else if(context0s[context] == contextTotal[context]){
      contextProb0FLW[context] = (1 << precisionBits) - 1;
    }else{
      contextProb0FLW[context] = (context0s[context] << precisionBits) / contextTotal[context];
    }
    assert((contextProb0FLW[context] > 0) && (contextProb0FLW[context] < (1 << precisionBits)));
    if((contextTotal[context] & WINDOW_PROB) == WINDOW_PROB){
      if(context0sWindow[context] != -1){
        contextTotal[context] -= WINDOW_PROB;
        context0s[context] -= context0sWindow[context];
      }
      context0sWindow[context] = context0s[context];
    }
  }

  //Encodes the bit
  encodeBitProb(bit, contextProb0FLW[context]);

  //Updates the number of symbols coded for this context
  if(bit == 0){ // Integers are literally interpreted as booleans
    context0s[context]++;
  }
  contextTotal[context]++;
}

int decodeBitContext(int context){
  //Updates the probability of this context, when necessary
  if((contextTotal[context] & UPDATE_PROB0) == UPDATE_PROB0){
    if(context0s[context] == 0){
      contextProb0FLW[context] = 1;
    }else if(context0s[context] == contextTotal[context]){
      contextProb0FLW[context] = (1 << precisionBits) - 1;
    }else{
      contextProb0FLW[context] = (context0s[context] << precisionBits) / contextTotal[context];
    }
    assert((contextProb0FLW[context] > 0) && (contextProb0FLW[context] < (1 << precisionBits)));
    if((contextTotal[context] & WINDOW_PROB) == WINDOW_PROB){
      if(context0sWindow[context] != -1){
        contextTotal[context] -= WINDOW_PROB;
        context0s[context] -= context0sWindow[context];
      }
      context0sWindow[context] = context0s[context];
    }
  }

  //Decodes the bit
  int bit = decodeBitProb(contextProb0FLW[context]);

  //Updates the number of symbols coded for this context
  if(bit == 0){ // Integers are literally interpreted as booleans
    context0s[context]++;
  }
  contextTotal[context]++;
  return(bit);
}

void encodeBitProb(int bit, int prob0FLW){
  assert((prob0FLW > 0) && (prob0FLW < (1 << precisionBits)));
  assert(intervalSize >= 1);

  if(bit == 0){ // Integers are literally interpreted as booleans
    intervalSize = ((long) prob0FLW * intervalSize) >> precisionBits;
  }else{
    long tmp = (((long) prob0FLW * intervalSize) >> precisionBits) + 1;
    intervalMin += tmp;
    intervalSize -= tmp;
  }

  if(intervalSize == 0){
    interval = intervalMin;
    transferInterval(codewordLength);
    intervalMin = 0;
    intervalSize = codewordMax;
  }
}

int decodeBitProb(int prob0FLW){
  assert((prob0FLW > 0) && (prob0FLW < (1 << precisionBits)));

  if(intervalSize == 0){
    fillInterval();
    intervalMin = 0;
    intervalSize = codewordMax;
  }
  assert(intervalSize >= 1);

  long tmp = (((long) prob0FLW * intervalSize) >> precisionBits) + 1;
  long mid = intervalMin + tmp;
  int bit; // Integers are literally interpreted as booleans
  if(interval >= mid){
    bit = 1; // Integers are literally interpreted as booleans
    intervalMin = mid;
    intervalSize -= tmp;
  }else{
    bit = 0; // Integers are literally interpreted as booleans
    intervalSize = tmp - 1;
  }
  return(bit);
}

void encodeInteger(int num, int numBits){
  assert(num >= 0);
  assert(num < (1 << (numBits + 1)));

  for(int bit = numBits - 1; bit >= 0; bit--){
    int realBit = (num & BIT_MASKS2[bit]) != 0; // Integers are literally interpreted as booleans
    encodeBit(realBit);
  }
}

int decodeInteger(int numBits){
  int num = 0;
  for(int bit = numBits - 1; bit >= 0; bit--){
    if(decodeBit()){
      num += BIT_MASKS2[bit];
    }
  }
  return(num);
}

void transferInterval(int length){
  int transferredBits = 0;
  while(transferredBits < length){
    int remainingBits = codewordLength - transferredBits;
    int transfer = remainingBits <= t? remainingBits: t;

    if((remainingBits - t) >= 0){
      Tr |= (interval >> (remainingBits - t)) & BIT_MASKS[transfer];
    }else{
      Tr |= (interval & BIT_MASKS[transfer]) << (t - remainingBits);
    }

    t -= transfer;
    if(t == 0){
      //stream.putunsigned char((unsigned char) Tr); // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
      if(Tr == 0xFF){
        t = 7;
      }else{
        t = 8;
      }
      Tr = 0;
      L++;
    }
    assert(t >= 0 && t <= 8);

    transferredBits += transfer;
  }
}

void fillInterval(){
  if(!replenishment && (L /*>= stream.getLength()*/)){ // ................
    //This placed here to throw the exception only when trying to fill a new interval
    // throw new Exception("No more data in the stream to fill the interval.");

    // Replace to:
    // fprintf(stderr, "No more data in the stream to fill the interval.");
  }

  int readBits = 0;
  interval = 0;
  do{
    if(t == 0){
      if(L /*< stream.getLength()*/){ // ...
        if(Tr == 0xFF){
          t = 7;
        }else{
          t = 8;
        }
        // Tr = (0x000000FF & stream.getunsigned char(L)); // Help pls :(
        L++;
      }else{
        //Fills with 0s
        Tr = 0;
        t = 8;
      }
    }

    int remainingBits = codewordLength - readBits;
    int read = remainingBits <= t? remainingBits: t;

    if((remainingBits - t) >= 0){
      interval |= ((long) Tr & BIT_MASKS[read]) << (remainingBits - t);
    }else{
      interval |= ((long) Tr >> (t - remainingBits)) & BIT_MASKS[read];
    }
    assert(interval >= 0 && interval <= codewordMax);

    t -= read;
    assert(t >= 0 && t <= 8);

    readBits += read;
  }while(readBits < codewordLength);
}

/*
 *
 *
 *
 */
void changeStream(FILE *stream /*unsigned charStream stream*/){ // Maybe replace with a struct
  if(stream){
    //stream = new unsigned charStream();
    readFileChannel = fopen("../files/file_0.txt", "rwb");
  }
  //this.stream = stream;
  readFileChannel = stream;
}

void reset(){
  for(int c = 0; c < numContexts; c++){
    contextProb0FLW[c] = prob0ToFLW(0.66f, precisionBits); //Slightly biased towards 0
    context0s[c] = 2;
    context0sWindow[c] = -1;
    contextTotal[c] = 3;
  }
}

void restartEncoding(){
  intervalMin = 0;
  intervalSize = codewordMax;
  Tr = 0;
  t = 8;
  L = -1;
}

void restartDecoding(){
  intervalMin = 0;
  intervalSize = 0;
  Tr = 0;
  L = 0;
  t = 0;
}

void terminate(){
  long interval1 = 0;
  long interval2 = 0;
  int bits = codewordLength;
  long intervalMax = intervalMin + intervalSize;
  while(((interval1 < intervalMin) || (interval1 > intervalMax))
    && ((interval2 < intervalMin) || (interval2 > intervalMax))){
    bits--;
    interval1 |= ((long) 1 << bits) & intervalMin;
    interval2 |= ((long) 1 << bits) & intervalMax;
  }

  if((interval1 >= intervalMin) && (interval1 <= intervalMax)){
    interval = interval1;
  }else{
    assert((interval2 >= intervalMin) && (interval2 <= intervalMax));
    interval = interval2;
  }
  assert(bits >= 0);

  transferInterval(codewordLength - bits);
  if(t != 8){
    //stream.putunsigned char((unsigned char) Tr); // HEEELP AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
    Tr = 0;
    t = 8;
  }
}

int remainingBytes(){
  if(t == 8){
    return(codewordBytes);
  }else{
    return(codewordBytes + 1);
  }
}

int getReadBytes(){
  return(L);
}

void setReplenishment(int replenishment){ // Integers are literally interpreted as booleans
  //this.replenishment = replenishment; // HELP AAAAAAAAAAAAAAA :(
}
