# ACFLW - Java2C
This is a transcription from an implementation of an Arithmetic encoder/decoder from Java to C. This work also aims to improve performance of the algorithm in C, mantaining its compression behaviour.
## Transcription
  - [ ] ArithmeticCoderFLW
  - [ ] ByteStream
  - [ ] \(Optional) Manage to externally use Java's ByteStream instances
## Evaluation: Java vs C
### Streams generator
  - [ ] On Java
  - [ ] On C
### Functions testing
  - [ ] transferInterval
  - [ ] fillInterval
#### Encode
  - [ ] encodeBit
  - [ ] encodeBitContext
  - [ ] encodeBitProb
  - [ ] encodeInteger
#### Decode
  - [ ] decodeBit
  - [ ] decodeBitContext
  - [ ] decodeBitProb
  - [ ] decodeInteger
### Profiling: I/O + Throughput
  - [ ] on Java
  - [ ] on C
## Paralelization
  - [ ] \(Optional) Process-level parallelization (ByteStream instances)
  - [ ] \(Optional) Task-level algorithm parallelization
