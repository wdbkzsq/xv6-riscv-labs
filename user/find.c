#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for (p = path + strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  return p;
}

void
find(char *path, char *filename)
{
  int fd;
  struct stat st;
  struct dirent de;
  char *p;
  char buf[512];

  if ((fd = open(path, O_RDONLY)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_DIR) {
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 || strcmp(de.name, ".") == 0 ||
          strcmp(de.name, "..") == 0)
        continue;
      if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
        fprintf(2, "find: path too long\n");
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      strcpy(p, de.name);
      find(buf, filename);
    }
  }
  if (strcmp(fmtname(path), filename) == 0) {
    printf("%s\n", path);
  }
  close(fd);
  return;
}

int
main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(2, "find: not enough arguments\n");
    exit(1);
  } else if (argc == 2) {
    find(".", argv[1]);
  } else if (argc == 3) {
    find(argv[1], argv[2]);
  } else {
    fprintf(2, "find: too much arguments\n");
    exit(1);
  }
}