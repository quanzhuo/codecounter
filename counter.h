#ifndef COUNTER_H_
#define COUNTER_H_

#include <stdio.h>

struct statistics {
  unsigned long total;
  unsigned long code_line;
  unsigned long blank;
  unsigned long comment;
  unsigned long c;
  unsigned long cpp;
  unsigned long java;
  unsigned long python;
  unsigned long perl;
  unsigned long sh;
};

#define BUFSIZE 1024

void process_file(const char *name);
void process_dir(const char *name);
void c_handler(FILE *);
#endif
