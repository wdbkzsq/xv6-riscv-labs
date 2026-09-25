#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
sixfive(int fd)
{
  char buf[128];
  char *div = " -\r\t\n.,/";
  int flag = 1, i, sum = 0, n;
  while ((n = read(fd, buf, sizeof(buf))) > 0) {
    for (i = 0; i < n; i++) {
      if (buf[i] >= '0' && buf[i] <= '9' && flag == 1) {
        sum = sum * 10 + buf[i] - '0';
        if (i == n - 1) {
          printf("%d\n", sum);
        }
      } else if (strchr(div, buf[i])) {
        if ((sum % 6 == 0 || sum % 5 == 0) && sum != 0 && flag == 1) {
          printf("%d\n", sum);
        }
        sum = 0;
        flag = 1;
      } else {
        flag = 0;
      }
    }
  }
}

int
main(int argc, char **argv)
{
  int i, fd;
  if (argc < 2) {
    sixfive(0);
    exit(0);
  }

  for (i = 1; i < argc; i++) {
    if ((fd = open(argv[i], O_RDONLY)) < 0) {
      fprintf(2, "sixfive: cannot open %s\n", argv[i]);
      continue;
    }
    sixfive(fd);
    close(fd);
  }
  exit(0);
}