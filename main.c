#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "counter.h"

extern struct statistics result;

int main(int argc, char const *argv[]) {
  struct stat file_mode;

  if(stat(argv[1], &file_mode)) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  if(S_ISDIR(file_mode.st_mode))
    process_dir(argv[1]);

  if(S_ISREG(file_mode.st_mode))
    process_file(argv[1]);

  printf("totally: %ld\n", result.total);
  printf("blank: %ld\n", result.blank);
  printf("comment: %ld\n", result.comment);
  printf("code: %ld\n", result.code);

  return 0;
}
