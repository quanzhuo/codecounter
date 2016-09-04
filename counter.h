#ifndef COUNTER_H_
#define COUNTER_H_

#include <stdio.h>
#include <stdbool.h>

struct statistics {
  unsigned long total;
  unsigned long code;
  unsigned long blank;
  unsigned long comment;
};

enum file_type {
  C_STYLE,
  SH_STYLE
};

#define BUFSIZE 1024

void process_file(const char *);
void process_dir(const char *);
bool embeded_comment(const char*);
void c_counter(FILE *);
void sh_counter(FILE *);
#endif
