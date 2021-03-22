# ACFLW - Java2C
This is a transcription from an implementation of an Arithmetic Coder from Java to C [[1]](#1).
This work aims to get a version of the resources in C, keeping an enough-equivalent compression behaviour, attempting to obtain a significantly-improved performance for this system.

## Transcription
  - [X] ArithmeticCoderFLW  -> 100% (Theorically)
  - [ ] ByteStream          -> 91%
  - [X] FileChannel         -> 100%
  - [X] ByteBuffer          -> Structure for operations
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
  - [X] getByte/getBytes
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

## References
<a id="1">[1]</a>
F. Aulí-Llinàs, “Context-Adaptive Binary Arithmetic Coding With Fixed-Length Codewords,” in IEEE Transactions on Multimedia, vol. 17, no. 8, pp. 1385-1390, Aug. 2015, doi: 10.1109/TMM.2015.2444797.
<a id="2">[1]</a>
J. Gosling, B. Joy and G. Steele, “The Java Language Specification,” Menlo Park, Calif.,, 1996.
