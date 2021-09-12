# Context-Adaptive Binary Arithmetic Coding With Fixed-Length CodeWords - Transcription, Testing and Evaluation
This is a transcription from an implementation of the Group of Interactive Coding of Images's Context-based Adaptive Binary Arithmetic Coding with Fixed-length Codewords, from it's original version in Java, to C [[1]](#1). This work aims to get a version of the resources in C, keeping an enough-equivalent compression behaviour, attempting to obtain a significantly-improved performance for this system and to achieve capability to operate with high throughput.

Every Java builtin dependency has been implemented, in such an specific way so the coder remains the fastest as possible. The performance evaluation results are documented in the following reference [[2]](#2).

The compilation and execution methods specified in the script benchmarks.sh must be followed in order to get a more-accurate performance evaluation, due to the Java JIT, the bytecode must be optimized just once at the beginning of the execution to get an always-constant program performance, so is necessary to discard the compilation time from the total execution Java time (showed in the program output).

## Optimizations
### Optimal design of ByteStream.putByte() & ByteStream.putFileSegment()
The need to copy the entire buffer in order to extend the memory space is no longer needed.
C provides the syscall realloc() to perform such action. Java's actual structure doesn't allow such an operation without adding complexity to it's system. This optimization has been implemented in the code.

Result: more capability to operate with large volumes of data, aka high throughput scalability, by avoiding perform a copy for the more times possible, saving cache performance and kernel overhead, like managing the syscall or memory paging.

### Optimization proposal for ByteStream.getByte() (readFile mode)
An accumulative sum and compare loop is performed to find the position of the desired byte when this function is called in this mode. Perhaps, the sum is not actually necessary if we assume that all the file segments conform the entire file with no ambiguities, in fact, this function's bottleneck is in the loop when numFileSegments gets escalated. This is a potentially dangerous assumption to achieve higher performance, mainly because this can generate distortion in the original behaviour of the system, so keeping the original behaviour is a higher priority. This optimization has not been implemented in the code due to this reason.

### Optimal design of FileChannel and optimization in FileChannel.read1B()
ByteStream's usage of the function FileChannel.read() in it's performance hotspot is to read one single byte each time the function ByteStream.getByte() is called, meaning a significantly-high overhead by the syscall malloc() implied in the function. A new function has been designed to perform this specific operation, FileChannel.read1B() copies 1 byte from the memory-mapped file buffer to the target array, employing an user-like usage.

## References
<a id="1">[1]</a>
F. Aulí-Llinàs, “Context-Adaptive Binary Arithmetic Coding With Fixed-Length Codewords,” in IEEE Transactions on Multimedia, vol. 17, no. 8, pp. 1385-1390, Aug. 2015, doi: 10.1109/TMM.2015.2444797.

<a id="1">[1]</a>
E. Felipe, J. Bartrina, dir. Programación en C y evaluación de un codificador aritmético. 2021. (958 Enginyeria Informàtica) <https://ddd.uab.cat/record/248486> [Checked: 12-Sep-2021].

<a id="3">[3]</a>
J. Gosling, B. Joy and G. Steele, “The Java Language Specification,” Menlo Park, Calif.,, 1996.

<a id="4">[4]</a>
“FileChannel (Java Platform SE 7)”, Docs.oracle, 2021. [Online]. Available: https://docs.oracle.com/javase/7/docs/api/java/nio /channels/FileChannel.html. [Accessed: 20-Apr-2021].

<a id="5">[5]</a>
“sendfile(2) [Redhat man page]”, Unix.com, 2021. [Online]. Available: https://www.unix.com/man-page/redhat/2/sendfile/. [Accessed: 21-Apr-2021].
