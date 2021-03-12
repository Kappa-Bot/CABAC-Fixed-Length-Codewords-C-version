#include <stdio.h>
#include "StreamChecker.h"

#define H1 "**************************************************************************"
#define H2 "__________________________________________________________________________"

#define FILENAME1 "../files/fc1.tmp"
#define FILENAME2 "../files/fc2.tmp"
#define FILENAME3 "../files/0.tmp"
#define FILENAME4 "../files/1.tmp"

void runTest(char *file1, char *file2) {
  printf("%s\n", H1);

  printf("Initializing StreamChecker\n");

  StreamChecker SC = scConfig(file1, file2);

  printf("File name 1: %s\n", file1);
  printf("File name 2: %s\n", file2);
  printf("Content size: %ld\n", SC.size);

  printf("%s\n", H1);

  printf("%41s\n", "scCheckEquals Test");
  printf("%s\n", H2);

  int equal = scCheckEquals(SC);

  printf("%s\n", H2);
  printf("Test %s content\n", equal ? "passed: same" : "failed, different");

  printf("%s\n", H1);

  /* **************************************************** */

  printf("%42s\n", "scCheckError Test");
  printf("%s\n", H2);

  long error = scCheckError(SC);

  printf("%s\n", H2);
  printf("Test %s %ld\n", error == SC.size ? "failed, error at size:"
                      : (error == -1 ? "passed with return:"
                                    : "failed, error at position:"), error);
  printf("%s\n", H1);

  scFinish(SC);

}

int main() {

  runTest(FILENAME1, FILENAME2);
  runTest(FILENAME1, FILENAME3);
  runTest(FILENAME3, FILENAME4);

  return 0;
}
