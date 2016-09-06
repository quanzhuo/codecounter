#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "counter.h"
#include "misc.h"
#include <string.h>

extern struct statis_data result;
extern FILE *debuglog;
void process_cmdline(int, char **);

int main(int argc, char *argv[]) {
  #ifdef DEBUG
    debuglog = fopen("log", "w+");
    if(debuglog == NULL) {
      perror("fopen");
      exit(EXIT_FAILURE);
    }
  #endif

  process_cmdline(argc, argv);
  print_result(&result);

  #ifdef DEBUG
    fclose(debuglog);
  #endif
  return 0;
}

void process_cmdline(int argc, char **argv) {
  if (argc == 1)
    usage_and_quit();

  // skip program name
  argv++;

  if (!strcmp(*argv, "-h") || !strcmp(*argv, "--help"))
    usage_and_quit();

  struct stat info;
  while(*argv) {
    if (stat(*argv, &info)) {
      perror(*argv);
      argv++;
      continue;
    }
    if(S_ISREG(info.st_mode))
      analysis_file(*argv);
    else if(S_ISDIR(info.st_mode)) {
      size_t len = strlen(*argv);
      // 去除 文件夹名后的 '/'
      if (*(*argv+len-1) == '/' && len > 1)
        *(*argv+len-1) = '\0';
      loop_dir(*argv);
    }
    argv++;
  }
}
