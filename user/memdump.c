#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

void
memdump(char *fmt, char *data, int len)
{
  int offset = 0;
  while (*fmt != '\0') {
    if (*fmt == 'i') {
      if (len - offset < 4) {
        fprintf(2, "memdump: not enough data for '%c'\n", *fmt);
        exit(0);
      }
      printf("%d\n", *(uint32 *)data);
      offset += 4;
      data += 4;
      fmt++;
    } else if (*fmt == 'p') {
      if (len - offset < 8) {
        fprintf(2, "memdump: not enough data for '%c'\n", *fmt);
        exit(0);
      }
      printf("%lx\n", *(uint64 *)data);
      offset += 8;
      data += 8;
      fmt++;
    } else if (*fmt == 'h') {
      if (len - offset < 2) {
        fprintf(2, "memdump: not enough data for '%c'\n", *fmt);
        exit(0);
      }
      printf("%d\n", *(short *)data);
      offset += 2;
      data += 2;
      fmt++;
    } else if (*fmt == 'c') {
      if (len - offset < 1) {
        fprintf(2, "memdump: not enough data for '%c'\n", *fmt);
        exit(0);
      }
      printf("%c\n", *(char *)data);
      offset += 1;
      data += 1;
      fmt++;
    } else if (*fmt == 's') {
      if (len - offset < 8) {
        fprintf(2, "memdump: not enough data for '%c'\n", *fmt);
        exit(0);
      }
      printf("%s\n", *(char **)data);
      offset += 8;
      data += 8;
      fmt++;
    } else if (*fmt == 'S') {
      if (len - offset < 1) {
        fprintf(2, "memdump: not enough data for '%c'\n", *fmt);
        exit(0);
      }
      while (offset < len && *data != '\0') {
        printf("%c", *data);
        offset++;
      }
      printf("\n");
      break;
    } else {
      break;
    }
  }
}

int
main(int argc, char **argv)
{
  int n;
  char buf[128];
  if (argc < 2) {
    exit(0);
  }
  n = read(0, buf, sizeof(buf));
  memdump(argv[1], buf, n);
}