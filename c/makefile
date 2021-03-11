# Just don't
# gcc -Wl,--allow-multiple-definition -O3 -Wall -lm -DNDEBUG ArithmeticCoderFLW.c ByteStream.c ACTest.c -o main.o
CCFLAGS = -Wall -O3 -lm -DNDEBUG
CCRESFLAGS = -c

CC = gcc

default: compile

compile:
	$(CC) $(CCFLAGS) $(CCRESFLAGS)       ArithmeticCoderFLW.c                            -o ArithmeticCoderFLW.o
	$(CC) $(CCFLAGS) $(CCRESFLAGS)                            ByteStream.c               -o ByteStream.o
	$(CC) $(CCFLAGS) $(CCRESFLAGS)                                         FileChannel.c -o FileChannel.o
	$(CC) $(CCFLAGS) $(CCRESFLAGS)       StreamGenerator.c                               -o StreamGenerator.o

	$(CC) $(CCFLAGS) ACTest.c ArithmeticCoderFLW.o ByteStream.o FileChannel.o -o ACTest.o
	$(CC) $(CCFLAGS) BSTest.c                      ByteStream.o FileChannel.o -o BSTest.o
	$(CC) $(CCFLAGS) FCTest.c                                   FileChannel.o -o FCTest.o
	$(CC) $(CCFLAGS) SGTest.c StreamGenerator.o                               -o SGTest.o

	$(RM) ArithmeticCoderFLW.o ByteStream.o FileChannel.o StreamGenerator.o

once:
	$(CC) $(CCFLAGS) ACTest.c ArithmeticCoderFLW.c ByteStream.c FileChannel.c -o ACTest.o
	$(CC) $(CCFLAGS) BSTest.c                      ByteStream.c FileChannel.c -o BSTest.o
	$(CC) $(CCFLAGS) FCTest.c                                   FileChannel.c -o FCTest.o
	$(CC) $(CCFLAGS) SGTest.c StreamGenerator.c                               -o SGTest.o

clean:
	$(RM) *.o
