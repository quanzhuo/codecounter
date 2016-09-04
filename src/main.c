#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "counter.h"
#include "misc.h"

extern struct statis_data result;

int main(int argc, char const *argv[]) {
  struct stat file_mode;

  if(argc != 2) {
    print_usage();
    exit(EXIT_FAILURE);
  }

  if(stat(argv[1], &file_mode)) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  if(S_ISDIR(file_mode.st_mode))
    process_dir(argv[1]);

  if(S_ISREG(file_mode.st_mode))
    process_file(argv[1]);

  print_result(&result);

  return 0;
}
