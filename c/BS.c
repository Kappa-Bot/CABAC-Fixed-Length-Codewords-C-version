#include <stdio.h>
#include <math.h>
#include <assert.h>

#include <constants.h>

void putByte(byte b){ // Help pls
  assert(streamMode == 0);
  if(limit == buffer/*.length*/){
    byte[] bufferTMP = new byte[buffer/*.length*/ * 2];
    // System.arraycopy(buffer, 0, bufferTMP, 0, (int) limit); // Need a memcopy
    buffer = bufferTMP;
  }
  buffer[(int) limit] = b;
  limit++;
}

void putBytes(byte[] array, int offset, int length){
  assert(streamMode == 0);
  assert((offset >= 0) && (length > 0) && (offset + length <= array/*.length*/));
  if(limit + (long) length > (long) buffer/*.length*/){
    byte[] bufferTMP = new byte[(buffer/*.length*/ + length) * 2];
    // System.arraycopy(buffer, 0, bufferTMP, 0, (int) limit); // Need a memcopy
    buffer = bufferTMP;
  }
  // System.arraycopy(array, offset, buffer, (int) limit, length); // Need a memcopy
  limit += (long) length;
}

void putBytes(int num, int numBytes){
  assert(streamMode == 0);
  assert((numBytes > 0) && (numBytes <= 4));
  for(int b = numBytes - 1; b >= 0; b--){
    byte addedByte = (byte) ((num >> (8 * b)) & 0x000000FF);
    putByte(addedByte);
  }
}

void putFileSegment(long begin, long length){
  assert(streamMode == 1);
  assert(readFileChannel != null);
  assert((begin >= 0) && (length > 0));
  assert(begin + length <= readFileChannel/*.size()*/);
  assert(readFileNumSegments <= readFileSegments/*.length*/);

  if(readFileNumSegments == readFileSegments/*.length*/){
    long[][] fcSegmentsTMP = new long[readFileSegments/*.length*/ * 2][2];
    for(int segment = 0; segment < readFileNumSegments; segment++){
      fcSegmentsTMP[segment][0] = readFileSegments[segment][0];
      fcSegmentsTMP[segment][1] = readFileSegments[segment][1];
    }
  }

  readFileSegments[readFileNumSegments][0] = begin;
  readFileSegments[readFileNumSegments][1] = length;
  readFileNumSegments++;
  limit += length;
}

void removeByte(){
	assert(streamMode == 0);
	limit = limit - 1 <= 0 ? 0: limit - 1;
	if(position > limit){
		position = limit;
	}
}

void removeBytes(int num){
  assert(streamMode == 0);
  limit = limit - num <= 0 ? 0: limit - num;
  if(position > limit){
    position = limit;
  }
}

byte getByte(){
  assert((streamMode == 0) || (streamMode == 1));
  assert(position >= 0);

  byte getByte = getByte(position);
  position++;
  return(getByte);
}

int getBytes(int numBytes){
  assert((streamMode == 0) || (streamMode == 1));
  int num = 0;
  for(int b = numBytes - 1; b >= 0; b--){
    byte getByte = getByte();
    for(int i = 7; i >= 0; i--){
      int bitMask = 1 << i;
      num += ((int) getByte & bitMask) << (b * 8);
    }
  }
  return(num);
}

int hasMoreBytes(){ // Integers are literally interpreted as booleans
  assert((streamMode == 0) || (streamMode == 1));
  return(position < limit ? 1 : 0);
}

byte getByte(long index){ // Help :(
  assert((streamMode == 0) || (streamMode == 1));

  if((index < 0) || (index >= limit)){
    //throw new Exception("Invalid position.");
  }
  byte getByte = 0;
  if(streamMode == 0){
    getByte = buffer[(int) index];
  }else if(streamMode == 1){
    //Determines the segment in which this index lies
    int segment = 0;
    long accBytes = 0;
    while(index >= accBytes + readFileSegments[segment][1]){
      accBytes += readFileSegments[segment][1];
      segment++;
    }
    assert(segment < readFileNumSegments);

    //Determines the position in the file
    long fcPosition = index - accBytes + readFileSegments[segment][0];
    assert(fcPosition < readFileChannel/*.size()*/);

    //Gets the byte
    // temporalBuffer.clear();
    // readFileChannel.read(temporalBuffer, fcPosition);
    getByte = temporalBuffer/*.array()*/[0];
  }else{
    assert(false);
  }
  return(getByte);
}

byte* getByteStream(){
  assert(streamMode == 0);
  return(buffer);
}

long getLength(){
  return(limit);
}

long getPosition(){
  return(position);
}

void clear(){
  limit = 0;
  position = 0;
}

void reset(){
  position = 0;
}

void skip(long numBytes){
  if(numBytes >= 0){
    if(position + numBytes > limit){
      position = limit;
    }else{
      position += numBytes;
    }
  }else{
    if(position + numBytes < 0){
      position = 0;
    }else{
      position += numBytes;
    }
  }
}

void endReadFileMode(){ // AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
  assert(streamMode == 1);

  buffer = new byte[(int) limit];
  int readBytes = 0;
  for(int segment = 0; segment < readFileNumSegments; segment++){
    long begin = readFileSegments[segment][0];
    long length = readFileSegments[segment][1];

    //temporalBuffer = ByteBuffer.allocate((int) length);
    //readFileChannel.read(temporalBuffer, begin);
    //System.arraycopy(temporalBuffer.array(), 0, buffer, readBytes, (int) length);
    readBytes += length;
  }
  streamMode = 0;
}

void returnReadFileMode(){
  assert(streamMode == 0);
  assert(readFileChannel != null);
  assert(readFileNumSegments != -1);
  assert(readFileSegments != null);

  buffer = null;
  limit = 0;
  for(int segment = 0; segment < readFileNumSegments; segment++){
    long length = (int) readFileSegments[segment][1];
    limit += length;
  }
  if(position >= limit){
    position = limit -1;
  }
  streamMode = 1;
}

void packetize(){
  assert(streamMode == 0);

  byte[] bufferTMP = new byte[(int) limit]; // need a malloc
  // System.arraycopy(buffer, 0, bufferTMP, 0, (int) limit); // Need a memcopy
  buffer = bufferTMP;
}

int isInReadNormalMode(){
  return(streamMode == 0 ? 1 : 0);
}

int isInReadFileMode(){
  return(streamMode == 1 ? 1 : 0);
}

int isInTemporalFile(){
  return(streamMode == 2 ? 1 : 0);
}

void write(FileOutputStream fos){ // Oof...
  assert((streamMode == 0) || (streamMode == 2));
  write(fos, 0, limit);
}

void write(FileOutputStream fos, long begin, long length){ // Yup...
  assert((streamMode == 0) || (streamMode == 2));
  assert(begin + length <= limit);
  if(!isInTemporalFile()){
    fos.write(buffer, (int) begin, (int) length);
  }else{
    FileChannel outputFC = fos.getChannel();
    FileChannel streamFC = FileChannel.open(FileSystems.getDefault().getPath(temporalFileName),
      StandardOpenOption.READ);
    long bytesWritten = 0;
    long positionOutputFC = outputFC.position();
    streamFC.position(begin + temporalFilePosition);
    while(bytesWritten < length){
      bytesWritten += outputFC.transferFrom(streamFC, positionOutputFC, length - bytesWritten);
      positionOutputFC += bytesWritten;
      streamFC.position(begin + temporalFilePosition + bytesWritten);
    }
    outputFC.position(positionOutputFC);
    streamFC.close();
  }
}

void saveToTemporalFile(char *temporalDirectory){
  assert(streamMode == 0);

  synchronized(lock){ // Oof bro, this gonna be a OpenMP critical region
    //Checks whether is the first time an object of this class is saved to the temporal file
    if(temporalFileName == null){
      do{
        temporalFileName = temporalDirectory;
        if(!temporalDirectory.endsWith("/")){
          temporalFileName += "/";
        }
        temporalFileName += "TMP-"; //prefix
        temporalFileName += Long.toString((long) (Math.random() * 999999999999d)) + ".tmp";
      }while(new File(temporalFileName).exists());
    }

    FileOutputStream fos = new FileOutputStream(temporalFileName, true);
    temporalFilePosition = fos.getChannel().position();
    FileChannel fc = fos.getChannel();
    ByteBuffer bf = ByteBuffer.wrap(buffer, 0, (int) limit);
    fc.write(bf);
    fos.close();
  }
  buffer = null;
  streamMode = 2;
}

void loadFromTemporalFile(){
  assert(streamMode == 2);

  buffer = new byte[(int) limit];
  FileChannel fc = FileChannel.open(FileSystems.getDefault().getPath(temporalFileName),
    StandardOpenOption.READ);
  fc.position(temporalFilePosition);
  ByteBuffer bf = ByteBuffer.wrap(buffer, 0, (int) limit);
  fc.read(bf);
  fc.close();
  temporalFilePosition = -1;
  streamMode = 0;
}

void destroy(){
  buffer = null;
  readFileChannel = null;
  readFileSegments = null;
}

void destroyTemporalFile(){
  if(temporalFileName != null){
    File file = new File(temporalFileName);
    int delete = file.delete();
    assert(delete);
    temporalFileName = null;
  }
}

int getMemorySegments(){
  int memory = 0;
  if(streamMode == 0){
    memory = readFileSegments.length * 2 * (Long.SIZE / 8);
  }
  return(memory);
}
