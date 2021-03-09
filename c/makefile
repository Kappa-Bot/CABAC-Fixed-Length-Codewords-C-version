# Just don't
# gcc -Wl,--allow-multiple-definition -O3 -Wall -lm -DNDEBUG ArithmeticCoderFLW.c ByteStream.c ACTest.c -o main.o
CCFLAGS = -Wall -O3 -lm -DNDEBUG

CC = gcc

default: compile

compile:
	$(CC) $(CCFLAGS) ACTest.c -o ACTest.o
	$(CC) $(CCFLAGS) BSTest.c -o BSTest.o

clean:
	$(RM) *.o
