#include "counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

struct statistics result;

void process_file(const char *name) {
  FILE *file = fopen(name, "r");
  if (!file) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  c_handler(file);
  if(fclose(file)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
}

void process_dir(const char *name) {

}

void c_handler(FILE *file) {
  char buffer[BUFSIZE];
  while (fgets(buffer, BUFSIZE-1, file)) {
    unsigned len = strlen(buffer);
    if (buffer[len-1] == '\n')
      buffer[len-1] = '\0';
    char *ptr = buffer;
     puts(buffer);
    while (*ptr) {
      if (isspace(*ptr))
        ptr++;
    }

    result.total++;
  }
}
