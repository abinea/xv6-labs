#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("sleep: missing operand\nUsage: sleep NUMBER\nPause for a "
           "user-specified number of ticks.A tick is a notion of time "
           "defined by the xv6 kernel, namely the time between two "
           "interrupts from the timer chip.\n");
    exit(1);
  }
  sleep(atoi(argv[1]));
  exit(0);
}
