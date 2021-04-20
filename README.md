# ACFLW - Java2C
This is a transcription from an implementation of an Arithmetic Coder from Java to C [[1]](#1).
This work aims to get a version of the resources in C, keeping an enough-equivalent compression behaviour, attempting to obtain a significantly-improved performance for this system.

## Transcription
  - [X] ArithmeticCoderFLW    -> 100% developed and 100% tested
  - [X] ByteStream            -> 100% developed and  62% tested
  - [X] FileChannel           -> 100% developed and 100% tested
  - [X] ByteBuffer            -> (Structure)
## Software Testing
  - [X] Streams checker (C)
  - [X] Streams generator (Both versions, but C > Java)
### Core Functions Progress
#### ArithmeticCoderFLW
  - [X] probToFLW
  - [X] FLWToProb
  - [x] transferInterval      // Implicit
  - [x] fillInterval          // Implicit
  - [X] encodeInteger
  - [X] encodeBit
  - [X] encodebitProb
  - [X] encodebitContext
  - [X] decodeInteger
  - [X] decodeBit
  - [X] decodebitProb
  - [X] decodebitContext
  - [X] terminate
#### ByteStream
  - [X] putByte
  - [X] putBytes
  - [X] getByte               // Missing readFile mode test
  - [X] getBytes              // Missing readFile mode test
  - [ ] putFileSegment
  - [ ] saveToTemporalFile
  - [ ] loadFromTemporalFile
  - [X] write                 // Missing temporalFile mode test
#### FileChannel
  - [X] size
  - [X] position
  - [X] read
  - [X] write
  - [X] transferFrom
### Profiling
  - Coming soon...

## References
<a id="1">[1]</a>
F. Aulí-Llinàs, “Context-Adaptive Binary Arithmetic Coding With Fixed-Length Codewords,” in IEEE Transactions on Multimedia, vol. 17, no. 8, pp. 1385-1390, Aug. 2015, doi: 10.1109/TMM.2015.2444797.

<a id="2">[2]</a>
J. Gosling, B. Joy and G. Steele, “The Java Language Specification,” Menlo Park, Calif.,, 1996.
