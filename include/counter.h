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
  unsigned long go;
  unsigned long makefile;
  unsigned long plaintext;
  unsigned long header;
  unsigned long xml;
};

enum code_t {
  // 使用 C 风格注释的汇编文件
  ASM_C,
  // 以分号为注释开头的汇编文件
  ASM_SEMI,
  C,
  CPP,
  JAVA,
  PYTHON,
  PERL,
  GO,
  SHELL,
  MAKEFILE,
  PLAINTEXT,
  HEADER,
  XML,
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
bool ends_with(const char*, const char*);
void plaintext_counter(FILE *);
void asm_semi_counter(FILE *);
void xml_style_counter(FILE *);
#endif
