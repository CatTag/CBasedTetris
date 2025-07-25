#include <stdio.h>
#include <time.h>
int main(void) {
  while (true) {
    int curtime = time(0);
    printf("%i\n", curtime);
  }
  return 0;
}
