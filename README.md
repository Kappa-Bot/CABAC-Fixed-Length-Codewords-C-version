# ACFLW - Java2C
This is a transcription from an implementation of an Arithmetic encoder/decoder from Java to C. This work also aims to improve performance of the algorithm in C, mantaining and enough-equivalent compression behaviour.
## Transcription (despite system testing)
  - [X] ArithmeticCoderFLW  -> 100%
  - [ ] ByteStream          -> 78%
## Evaluation: Java vs C
### Streams generator
  - [X] On Java
  - [ ] On C
### Core Functions testing
  - [X] probToFLW
  - [X] FLWToProb
  - [ ] encodebitProb
  - [ ] decodebitProb
  - [X] putByte/putBytes
  - [ ] getByte/getBytes
  - [ ] transferInterval
  - [ ] fillInterval
  - [ ] removeByte/removeBytes
  - Coming soon ...
### Profiling: I/O + Throughput
  - Coming soon ...
## Paralelization
  - [ ] \(Optional) Process-level parallelization (ByteStream instances)
  - [ ] \(Optional) Task-level algorithm parallelization
