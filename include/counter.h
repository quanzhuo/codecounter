#ifndef COUNTER_H_
#define COUNTER_H_

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>

struct statis_data {
  unsigned long total;
  unsigned long blank;
  unsigned long comment;
  unsigned long c;
  unsigned long cpp;
  unsigned long java;
  unsigned long python;
  unsigned long perl;
  unsigned long shell;
  unsigned long makefile;
  unsigned long plaintext;
  unsigned long header;
};

enum code_t {
  C,
  CPP,
  JAVA,
  PYTHON,
  PERL,
  SHELL,
  MAKEFILE,
  PLAINTEXT,
  HEADER,
  UNKNOWN
};

#define BUFSIZE 1024

void process_file(const char *);
void process_dir(const char *);
bool embeded_comment(const char*);
void c_counter(FILE *);
void sh_counter(FILE *);
bool is_dir(const char *);
bool skip_some_entries(const char *);
void check_comment_type(const char *);
#endif
