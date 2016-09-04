#include "counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

struct statistics result;
enum file_type type;

void process_file(const char *name) {
  const char *p_suffix = name;
  while((*p_suffix != '.') && (*p_suffix != '\0'))
    p_suffix++;

  if (! *p_suffix) {
    fprintf(stderr, "%s, NO SUFFIX\n", name);
    exit(EXIT_FAILURE);
  }

  unsigned len = strlen(p_suffix);
  char suffix[len+1];
  strcpy(suffix, p_suffix);
  char *ptr_suffix = suffix;

  while (*ptr_suffix) {
    *ptr_suffix = tolower(*ptr_suffix);
    ptr_suffix++;
  }

  if(!strcmp(suffix, ".c") || !strcmp(suffix, ".cpp")
      || ! strcmp(suffix, ".java"))
    type = C_STYLE;
  else if (!strcmp(suffix, ".py") || !strcmp(suffix, ".pl")
      || !strcmp(suffix, ".sh"))
    type = SH_STYLE;

  FILE *file = fopen(name, "r");
  if (!file) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  switch(type) {
    case C_STYLE:
      c_counter(file);
      break;
    case SH_STYLE:
      sh_counter(file);
    default:
      break;
  }

  if(fclose(file)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
}

void process_dir(const char *name) {

}

void c_counter(FILE *file) {
  char buffer[BUFSIZE];
  bool in_multi_line_comment = false;

  while (fgets(buffer, BUFSIZE-1, file)) {
    unsigned len = strlen(buffer);
    char *ptr = buffer;

    // 处于多行注释中
    if (in_multi_line_comment) {
      result.comment++;
      result.total++;
      // 多行注释是否结束？
      if (strstr(buffer, "*/") != NULL)
        in_multi_line_comment = false;
      continue;
    }

    // 跳过行前空白
    while (isspace(*ptr) && *ptr != '\n')
      ptr++;

    // 空白行
    if (*ptr == '\n') {
      result.blank++;
      result.total++;
      continue;
    }

    // 独占一行的单行注释
    if (*ptr == '/' && *(ptr + 1) == '/') {
      result.comment++;
      result.total++;
      continue;
    }

    // 另起一行的多行注释
    if (*ptr == '/' && *(ptr + 1) == '*') {
      in_multi_line_comment = true;
      result.comment++;
      result.total++;
      // 多行注释是否在本行结束
      if (strstr(ptr, "*/") != NULL)
        in_multi_line_comment = false;
      continue;
    }

    // 代码行
    result.code++;

    // 嵌入代码行的注释
    if (embeded_comment(ptr))
      result.comment++;

    result.total++;
  }
}

void sh_counter(FILE *file) {

}

bool embeded_comment(const char *ptr) {
  // char *ptr_quote = NULL;
  // char *ptr_single_comment = NULL;
  // char *ptr_multi_comment_open = NULL;
  // char *ptr_multi_comment_close = NULL;
  //
  // while (*ptr != '\n') {
  //   if (*ptr == '"' )
  //     ptr_quote = ptr;
  //   else if ((*ptr == '/') && (*(ptr + 1) == '/'))
  //     ptr_comment = ptr;
  //   else if ((*ptr == '/') && (*(ptr + 1) == '*'))
  //     ptr_multi_comment_open = ptr;
  //   else if ((*ptr == '*') && (*(ptr + 1) == '/'))
  //     ptr_multi_comment_close = ptr;
  //
  //   ptr++;
  // }
  // 暂时不考虑嵌入代码行的注释
  return false;
}
