CC = gcc
CCFLAGS = -Wall -Ofast -lm -fopenmp

AC = src/ArithmeticCoderFLW.o
BS = src/ByteStream.o
FC = src/FileChannel.o
SG = util/StreamGenerator.o
SC = util/StreamChecker.o

ACT = test/ACTest.o
BST = test/BSTest.o
FCT = test/FCTest.o
SGT = test/SGTest.o
SCT = test/SCTest.o
FULL = test/FULL.o

ACP = perf/ACPerf.o
BSP = perf/BSPerf.o
FCP = perf/FCPerf.o

default: compile
compile : src util test perf
src : $(AC) $(BS) $(FC)
util : $(SG) $(SC)
test : $(ACT) $(BST) $(FCT) $(SGT) $(SCT) $(FULL)
perf : $(ACP) $(BSP) $(FCP)


################################# SOURCE (SRC) #################################
$(AC) : src/ArithmeticCoderFLW.c
	$(CC) $(CCFLAGS) -c            src/ArithmeticCoderFLW.c                                    -o src/ArithmeticCoderFLW.o
$(BS) : src/ByteStream.c
	$(CC) $(CCFLAGS) -c                                     src/ByteStream.c                   -o src/ByteStream.o
$(FC) : src/FileChannel.c
	$(CC) $(CCFLAGS) -c                                                      src/FileChannel.c -o src/FileChannel.o
################################# SOURCE (SRC) #################################



#################################### UTIL ######################################
$(SG) : util/StreamGenerator.c
	$(CC) $(CCFLAGS) -c                     util/StreamGenerator.c                             -o util/StreamGenerator.o
$(SC) : util/StreamChecker.c
	$(CC) $(CCFLAGS) -c                                            util/StreamChecker.c        -o util/StreamChecker.o
#################################### UTIL ######################################



#################################### TEST ######################################
$(ACT) : test/ACTest.c $(AC) $(BS) $(FC)
	$(CC) $(CCFLAGS) test/ACTest.c src/ArithmeticCoderFLW.o src/ByteStream.o src/FileChannel.o -o test/ACTest.o
$(BST) : test/BSTest.c $(BS) $(FC)
	$(CC) $(CCFLAGS) test/BSTest.c                          src/ByteStream.o src/FileChannel.o -o test/BSTest.o
$(FCT) : test/FCTest.c $(FC)
	$(CC) $(CCFLAGS) test/FCTest.c                                           src/FileChannel.o -o test/FCTest.o
$(SGT) : test/SGTest.c $(SG)
	$(CC) $(CCFLAGS) test/SGTest.c          util/StreamGenerator.o                             -o test/SGTest.o
$(SCT) : test/SCTest.c $(SC)
	$(CC) $(CCFLAGS) test/SCTest.c                                  util/StreamChecker.o       -o test/SCTest.o
$(FULL) : test/FULL.c $(AC) $(BS) $(FC)
	$(CC) $(CCFLAGS) test/FULL.c   src/ArithmeticCoderFLW.o src/ByteStream.o src/FileChannel.o -o test/FULL.o
#################################### TEST ######################################



#################################### PERF ######################################
$(ACP) : perf/ACPerf.c $(AC) $(BS) $(FC)
	$(CC) $(CCFLAGS) perf/ACPerf.c src/ArithmeticCoderFLW.o src/ByteStream.o src/FileChannel.o -o perf/ACPerf.o
$(BSP) : perf/BSPerf.c $(BS) $(FC)
	$(CC) $(CCFLAGS) perf/BSPerf.c                          src/ByteStream.o src/FileChannel.o -o perf/BSPerf.o
$(FCP) : perf/FCPerf.c $(FC)
	$(CC) $(CCFLAGS) perf/FCPerf.c                                           src/FileChannel.o -o perf/FCPerf.o
#################################### PERF ######################################

.PHONY: clean
clean:
	$(RM) */*.o
