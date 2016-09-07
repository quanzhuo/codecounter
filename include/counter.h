#ifndef COUNTER_H_
#define COUNTER_H_

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <unistd.h>

struct statis_data {
  unsigned long total;
  unsigned long blank;
  unsigned long comment;
  unsigned long assembly;
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
  ASM,
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

void analysis_file(char *);
void loop_dir(const char *);
bool embeded_comment(const char*);
void c_style_counter(FILE *);
void sh_style_counter(FILE *);
bool is_dir(const char *);
bool skip_some_entries(const char *);
void check_comment_type(const char *);
ssize_t mygetline(char **, FILE *);
#endif
