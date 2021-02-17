#include <stdio.h>
#include <assert.h>
#include <math.h>

#include <constants.h>

int prob0ToFLW(float prob0, int precisionBits){
  assert((prob0 >= 0f) && (prob0 <= 1f));

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
  assert((prob0 > 0f) && (prob0 < 1f));
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
      //stream.putByte((byte) Tr); // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
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
    throw new Exception("No more data in the stream to fill the interval.");
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
        // Tr = (0x000000FF & stream.getByte(L)); // Help pls :(
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
void changeStream(/*ByteStream stream*/){
  if(stream == null){
    //stream = new ByteStream();
  }
  //this.stream = stream;
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
    //stream.putByte((byte) Tr); // HEEELP AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
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
