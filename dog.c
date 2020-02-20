#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
  int fd, rd;
  char *buf[BUFSIZ];
  // When no arguments are provided.
  if (argc == 1) {
    while (1) {
      // To reset
      memset(buf, 0, sizeof(buf));
      rd = read(0, buf, sizeof(buf));
      // Detection for Ctrl-D
      if (rd == 0) {
        break;
      }
      write(1, buf, rd);
    }
    // When arguments are provided
  } else {
    for (int i = 1; i < argc; i++) {
      // When - is entered as an argument
      if (strcmp(argv[i], "-") == 0) {
        while (1) {
          memset(buf, 0, sizeof(buf));
          rd = read(0, buf, sizeof(buf));
          // Detection for Ctrl-D
          if (rd == 0) {
            break;
          }
          write(1, buf, rd);
        }
      } else {
        fd = open(argv[i], O_RDONLY);
        // To generate error msg for directory
        if (open(argv[i], __O_DIRECTORY) != -1) {
          fprintf(stderr, "dog: %s: Is a directory\n", argv[i]);
        } else {
          // To generate the correct error msg when failed to open a file
          if (fd == -1) {
            if (access(argv[i], 0) != -1) {
              fprintf(stderr, "dog: %s: Permission denied\n", argv[i]);
            } else {
              fprintf(stderr, "dog: %s: No such file or directory\n", argv[i]);
            }
          } else {
            memset(buf, 0, sizeof(buf));
            rd = read(fd, buf, sizeof(buf));
            write(1, buf, rd);
          }
        }
        if (i + 1 == argc) {
          close(fd);
        }
      }
    }
  }
  return 0;
}
