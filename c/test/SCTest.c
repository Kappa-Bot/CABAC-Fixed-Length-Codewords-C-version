#include <stdio.h>
#include "../util/StreamChecker.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"

// Both contains "lorem ipsum"
#define FILENAME1 "../../files/fc1.tmp"
#define FILENAME2 "../../files/fc2.tmp"

// Different content (random)
#define FILENAME3 "../../files/sg_0.tmp"
#define FILENAME4 "../../files/sg_1.tmp"

// Same content: [0-(length/2)] bytes from BSTest
#define FILENAME5 "../../files/w1_0_c.tmp"
#define FILENAME6 "../../files/w1_0_j.tmp"

// Same content: write(normal) from BSTest
#define FILENAME7 "../../files/w0_c.tmp"
#define FILENAME8 "../../files/w0_j.tmp"

// Same content: write(temporalFile) from BSTest
#define FILENAME9 "../../files/wt_c.tmp"
#define FILENAME10 "../../files/wt_j.tmp"

void runTest(char *file1, char *file2) {
  printf("%s\n", H1);
  printf("%s\n", H2);

  printf("Initializing StreamChecker\n");

  StreamChecker SC = scConfig(file1, file2);

  printf("File name 1: %s\n", file1);
  printf("File name 2: %s\n", file2);
  printf("Content size: %lld\n", SC.size);

  printf("%s\n", H1);

  printf("%41s\n", "scCheckEquals Test");
  printf("%s\n", H2);

  int equal = scCheckEquals(SC);

  printf("Test %s content\n", equal ? "passed: same" : "failed, different");

  printf("%s\n", H1);

  /* **************************************************** */

  printf("%42s\n", "scCheckError Test");
  printf("%s\n", H2);

  long long error = scCheckError(SC);

  printf("Test %s %lld\n", error == SC.size ? "failed, error at size:"
                      : (error == -1 ? "passed with return:"
                                    : "failed, error at position:"), error);
  printf("%s\n", H2);
  printf("%s\n", H1);

  scFinish(SC);
}

int main() {
  runTest(FILENAME1, FILENAME2);
  runTest(FILENAME1, FILENAME3);
  runTest(FILENAME3, FILENAME4);
  runTest(FILENAME5, FILENAME6);
  runTest(FILENAME7, FILENAME8);
  runTest(FILENAME9, FILENAME10);

  return 0;
}
