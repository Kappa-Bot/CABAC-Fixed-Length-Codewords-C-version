JAVAC = javac
JFLAGS = -source 11

AC = core/ArithmeticCoderFLW.class
BS = core/ByteStream.class
SG = util/StreamGenerator.class

ACT = test/ACTest.class
BST = test/BSTest.class
FCT = test/FCTest.class
FULL = test/FULL.class

ACP = perf/ACPerf.class
BSP = perf/BSPerf.class
FCP = perf/FCPerf.class

default: compile
compile : core util test perf
core : $(AC) $(BS)
util : $(SG)
test : $(ACT) $(BST) $(FCT) $(FULL)
perf : $(ACP) $(BSP) $(FCP)


################################# SOURCE (core) #################################
$(AC) : core/ArithmeticCoderFLW.java
	$(JAVAC) $(JFLAGS) core/ArithmeticCoderFLW.java
$(BS) : core/ByteStream.java
	$(JAVAC) $(JFLAGS) core/ByteStream.java
################################# SOURCE (core) #################################



#################################### UTIL ######################################
$(SG) : util/StreamGenerator.java
	$(JAVAC) $(JFLAGS) util/StreamGenerator.java
$(SC) : util/StreamChecker.java
	$(JAVAC) $(JFLAGS) util/StreamChecker.java
#################################### UTIL ######################################



#################################### TEST ######################################
$(ACT) : test/ACTest.java $(AC) $(BS)
	$(JAVAC) $(JFLAGS) test/ACTest.java
$(BST) : test/BSTest.java $(BS)
	$(JAVAC) $(JFLAGS) test/BSTest.java
$(FCT) : test/FCTest.java
	$(JAVAC) $(JFLAGS) test/FCTest.java
$(FULL) : test/FULL.java $(AC) $(BS)
	$(JAVAC) $(JFLAGS) test/FULL.java
#################################### TEST ######################################



#################################### PERF ######################################
$(ACP) : perf/ACPerf.java $(AC) $(BS)
	$(JAVAC) $(JFLAGS) perf/ACPerf.java
$(BSP) : perf/BSPerf.java $(BS)
	$(JAVAC) $(JFLAGS) perf/BSPerf.java
$(FCP) : perf/FCPerf.java
	$(JAVAC) $(JFLAGS) perf/FCPerf.java
#################################### PERF ######################################

.PHONY: clean
clean:
	$(RM) */*.class
