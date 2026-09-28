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
find(char *path, char *filename, char **cmd)
{
  int fd, pid, i;
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
        continue;
      }
      strcpy(buf, path);
      p = buf + strlen(buf);
      *p++ = '/';
      strcpy(p, de.name);
      find(buf, filename, cmd);
    }
  }
  if (strcmp(fmtname(path), filename) == 0) {
    if (cmd == (char **)0)
      printf("%s\n", path);
    else {
      pid = fork();
      if (pid < 0) {
        printf("find: fork failed\n");
        exit(1);
      }
      if (pid == 0) {
        char *argv[16];
        for (i = 0; cmd[i] != 0; i++) {
          argv[i] = cmd[i];
        }
        argv[i++] = path;
        argv[i] = 0;
        exec(argv[0], argv);
        printf("find: exec failed\n");
        exit(1);
      } else {
        wait(0);
      }
    }
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
    find(".", argv[1], (char **)0);
  } else if (argc == 3) {
    find(argv[1], argv[2], (char **)0);
  } else if (argc > 4 && strcmp(argv[3], "-exec") == 0) {
    find(argv[1], argv[2], argv + 4);
  }
  exit(0);
}