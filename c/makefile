CCFLAGS = -Wall -O3 -lm -DNDEBUG -fopenmp

CC = gcc

default: compile

# SRC src/%.o : src/%.c
#	$(CC) $(CCFLAGS) -o src/$< -o src/$@

# UTIL util/%.o : util/%.c
#	$(CC) $(CCFLAGS) -o util/$< -o util/$@

.PHONY: ACFLW BS FC SG SC ACT BST FCT SGT SCT compile clean

ACFLW src/ArithmeticCoderFLW.o : src/ArithmeticCoderFLW.c src/ArithmeticCoderFLW.h
	$(CC) $(CCFLAGS) -c src/ArithmeticCoderFLW.c -o src/ArithmeticCoderFLW.o
BS src/ByteStream.o : src/ByteStream.c src/ByteStream.h
	$(CC) $(CCFLAGS) -c src/ByteStream.c -o src/ByteStream.o
FC src/FileChannel.o : src/FileChannel.c src/FileChannel.h
	$(CC) $(CCFLAGS) -c src/FileChannel.c -o src/FileChannel.o

SG util/StreamGenerator.o : util/StreamGenerator.c util/StreamGenerator.h
	$(CC) $(CCFLAGS) -c util/StreamGenerator.c -o util/StreamGenerator.o
SC util/StreamChecker.o : util/StreamChecker.c util/StreamChecker.h
	$(CC) $(CCFLAGS) -c util/StreamChecker.c -o util/StreamChecker.o

ACT test/ACTest.o : test/ACTest.c src/ArithmeticCoderFLW.o src/ByteStream.o src/FileChannel.o
	$(CC) $(CCFLAGS) test/ACTest.c src/ArithmeticCoderFLW.o src/ByteStream.o src/FileChannel.o -o test/ACTest.o
BST test/BSTest.o : test/BSTest.c src/ByteStream.o src/FileChannel.o
	$(CC) $(CCFLAGS) test/BSTest.c src/ByteStream.o src/FileChannel.o -o test/ACTest.o
FCT test/FCTest.o : test/FCTest.c src/FileChannel.o
	$(CC) $(CCFLAGS) test/FCTest.c src/FileChannel.o -o test/ACTest.o
SGT test/SGTest.o : test/SGTest.c util/StreamGenerator.o
	$(CC) $(CCFLAGS) test/SGTest.c util/StreamGenerator.o -o test/SGTest.o
SCT test/SCTest.o : test/SCTest.c util/StreamChecker.o
	$(CC) $(CCFLAGS) test/SCTest.c util/StreamChecker.o -o test/SCTest.o

compile:
	cd src/;  $(CC) $(CCFLAGS) -c *.c; cd ../;
	cd util/; $(CC) $(CCFLAGS) -c *.c; cd ../;
	$(CC) $(CCFLAGS) test/ACTest.c src/ArithmeticCoderFLW.o src/ByteStream.o src/FileChannel.o -o test/ACTest.o
	$(CC) $(CCFLAGS) test/BSTest.c                          src/ByteStream.o src/FileChannel.o -o test/BSTest.o
	$(CC) $(CCFLAGS) test/FCTest.c                                           src/FileChannel.o -o test/FCTest.o
	$(CC) $(CCFLAGS) test/SGTest.c util/StreamGenerator.o                                      -o test/SGTest.o
	$(CC) $(CCFLAGS) test/SCTest.c util/StreamChecker.o                                        -o test/SCTest.o
	$(CC) $(CCFLAGS) test/FULL.c   src/ArithmeticCoderFLW.o src/ByteStream.o src/FileChannel.o -o test/FULL.o
	$(CC) $(CCFLAGS) perf/BSPerf.c                          src/ByteStream.o src/FileChannel.o -o perf/BSPerf.o

clean:
	$(RM) */*.o
