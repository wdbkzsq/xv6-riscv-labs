#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  if (argc != 2) {
    fprintf(2, "usage: sleep tick...\n");
    exit(1);
  }
  int n = atoi(argv[1]) * 10;
  pause(n);
  return 0;
}