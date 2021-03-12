# ACFLW - Java2C
This is a transcription from an implementation of an Arithmetic encoder/decoder from Java to C. This work also aims to improve performance of the algorithm in C, mantaining and enough-equivalent compression behaviour.

## Transcription
  - [X] ArithmeticCoderFLW  -> 100% (Theorically)
  - [ ] ByteStream          -> 91%
  - [X] FileChannel         -> 100%
  - [X] ByteBuffer          -> 100% (just attributes and operations)
## Software Testing
  - [X] Streams checker (C)
  - [X] Streams generator (C/Java)   // Both versions, but C >>>>>>
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
  - [X] fcPosition
  - [X] fcRead/fcWrite
  - [X] fcTransferFrom
### Profiling
  - Coming soon ...
