# ACFLW - Java2C
This is a transcription from an implementation of an Arithmetic Coder from Java to C [[1]](#1).
This work aims to get a version of the resources in C, keeping an enough-equivalent compression behaviour, attempting to obtain a significantly-improved performance for this system.

## Transcription
  - [X] ArithmeticCoderFLW
  - [X] ByteStream
  - [X] FileChannel
  - [X] Structures (ByteBuffer, FileSegments)
## Software Testing
  - [X] Streams checker (C)
  - [X] Streams generator (Both versions, but C > Java)
  - [X] ArithmeticCoderFLW
  - [X] ByteStream - normal
  - [X] ByteStream - readFile
  - [X] ByteStream - temporalFile
  - [X] FileChannel
  - [ ] Full System coding/decoding
## Profiling (Raspberry Pi 4 8GB - Raspbian)
### ArithmeticCoderFLW + ByteStream (normal)
  - [ ] encodeBit
  - [ ] putByte
  - [ ] putBytes
  - [ ] decodeBit
  - [ ] getByte
  - [ ] ...
### ArithmeticCoderFLW + ByteStream (readFile)
  - [ ] putFileSegment
  - [ ] decodeBit
  - [ ] getByte
  - [ ] ...

## References
<a id="1">[1]</a>
F. Aulí-Llinàs, “Context-Adaptive Binary Arithmetic Coding With Fixed-Length Codewords,” in IEEE Transactions on Multimedia, vol. 17, no. 8, pp. 1385-1390, Aug. 2015, doi: 10.1109/TMM.2015.2444797.

<a id="2">[2]</a>
J. Gosling, B. Joy and G. Steele, “The Java Language Specification,” Menlo Park, Calif.,, 1996.

<a id="3">[3]</a>
“FileChannel (Java Platform SE 7)”, Docs.oracle, 2021. [Online]. Available: https://docs.oracle.com/javase/7/docs/api/java/nio /channels/FileChannel.html. [Accessed: 20-Apr-2021].

<a id="4">[4]</a>
“FileOutputStream (Java Platform SE 7)”, Oracle Docs, 2021. [Online]. Available: https://docs.oracle.com/javase/7/docs/api/java/io /FileOutputStream.html. [Accessed: 20-Apr-2021].

<a id="5">[5]</a>
“sendfile(2) [Redhat man page]”, Unix.com, 2021. [Online]. Available: https://www.unix.com/man-page/redhat/2/sendfile/. [Accessed: 21-Apr-2021].
