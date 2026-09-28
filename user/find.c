#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

char *
strcat(char *s1, char *s2)
{
  uint32 len1 = strlen(s1);
  uint32 len2 = strlen(s2);
  char *s3 = (char *)malloc(len1 + len2 + 2);
  char *res = s3;
  while (*s1 != '\0') {
    *s3 = *s1;
    s3++;
    s1++;
  }
  *s3 = '/';
  s3++;
  while (*s2 != '\0') {
    *s3 = *s2;
    s3++;
    s2++;
  }
  *s3 = '\0';
  return res;
}

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
  char *subpath;

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
      subpath = strcat(path, de.name);
      find(subpath, filename);
      free(subpath);
    }
  } else if (st.type == T_FILE || st.type == T_DEVICE) {
    if (strcmp(fmtname(path), filename) == 0) {
      printf("%s\n", path);
    }
  }
  close(fd);
  return;
}

int
main(int argc, char **argv)
{
  if (argc < 2) {
    fprintf(2, "find: not enough arguments");
    exit(1);
  } else if (argc == 2) {
    find(".", argv[1]);
  } else if (argc == 3) {
    find(argv[1], argv[2]);
  } else {
    fprintf(2, "find: too much arguments");
    exit(1);
  }
}