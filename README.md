# ACFLW - Java2C
This is a transcription from an implementation of an Arithmetic encoder/decoder from Java to C. This work also aims to improve performance of the algorithm in C, mantaining and enough-equivalent compression behaviour.

## Transcription (despite system testing)
  - [X] ArithmeticCoderFLW  -> 100%
  - [ ] ByteStream          -> 91%
  - [ ] FileChannel         -> 99% (not totally guaranteed)
## Function Testing (both versions)
  - [ ] File/Streams checker (C)
  - [X] File/Streams generator (C/Java)   // Both versions, but C >>>>>>
### Core Functions Progress
#### ArithmeticCoderFLW
  - [X] probToFLW/FLWToProb
  - [ ] transferInterval
  - [ ] fillInterval
  - [ ] encodebitProb/decodebitProb
  - [ ] encodebitContext/decodebitContext
#### ByteStream
  - [X] putByte/putBytes
  - [ ] getByte/getBytes
  - [ ] putFileSegment
  - [ ] saveToTemporalFile
  - [ ] loadFromTemporalFile
  - [ ] write
#### FileChannel
  - [ ] fcPosition      // Java test missing
  - [ ] fcRead/fcWrite  // Java test missing
  - [ ] fcTransferFrom  // Java test missing
### Profiling
  - Coming soon ...
