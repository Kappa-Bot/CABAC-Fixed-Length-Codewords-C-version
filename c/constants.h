
#ifndef ACFLW
#define ACFLW
int UPDATE_PROB0 = 7;
int WINDOW_PROB = 127;
int codewordLength = 32;
int precisionBits = 15;
long codewordMax = -1;
int codewordBytes = -1;
int precisionMid = -1;
long intervalMin = -1;
long intervalSize = -1;
long interval = -1;
int Tr = -1;
int t  = -1;
int L;
int numContexts = -1;

int[] contextProb0FLW = {}; // maybe an equivalent lol
int[] context0s = {};       // maybe an equivalent lol
int[] context0sWindow = {}; // maybe an equivalent lol
int[] contextTotal = {};    // maybe an equivalent lol

int replenishment = 1;
long[] BIT_MASKS = {0x0, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF};
int[] BIT_MASKS2 = {1, 1 << 1, 1 << 2, 1 << 3, 1 << 4, 1 << 5, 1 << 6,
                    1 << 7, 1 << 8, 1 << 9, 1 << 10, 1 << 11, 1 << 12,
                    1 << 13, 1 << 14, 1 << 15, 1 << 16, 1 << 17, 1 << 18,
                    1 << 19, 1 << 20, 1 << 21, 1 << 22, 1 << 23, 1 << 24,
                    1 << 25, 1 << 26, 1 << 27, 1 << 28, 1 << 29, 1 << 30};
#endif

#ifndef BS
#define BS
int INITIAL_ALLOCATION = 1024;
int INITIAL_NUM_SEGMENTS = 32;
int streamMode = -1;
byte[] buffer = {};
long limit = 0;
long position = 0;
//private FileChannel readFileChannel = null; // ... AAAAAAAAAA

int readFileNumSegments = -1;
long[][] readFileSegments = {{}};
long temporalFilePosition = -1;

char *temporalFileName = null;

/**
 * Temporal ByteBuffer employed to read from the file channel when necessary (only used when
 * the stream is in readFile mode).
 * <p>
 * This object is reused every time a new byte is read from the file.
 */
byte temporalBuffer = '0'

/**
 * This object is useful to lock the access to the variables shared by multiple threads
 * (i.e., the <code>temporalFileName</code>).
 * <p>
 * The object is never modified, its only purpose is to allow a lock.
 */
byte lock; // Probably bein replaced by an OpenMP thread-safe locker
#endif
