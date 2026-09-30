#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char **argv)
{
  if (argc < 3) {
    fprintf(2, "sandbox: need more arguments\n");
    exit(0);
  }
  int mask = atoi(argv[1]);
  if (fork() == 0) {
    if (interpose(mask) < 0) {
      fprintf(2, "sandbox: interpose failed\n");
      exit(0);
    }
    exec(argv[2], argv + 2);
    fprintf(2, "sandbox: exec failed\n");
    exit(1);
  }
  wait(0);
  exit(0);
}