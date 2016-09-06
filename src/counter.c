#include "counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

void check_type(const char *);

#ifdef DEBUG
  extern FILE *debuglog;
#endif

struct statis_data result;
enum code_t code = UNKNOWN;

// extern int errno;

void process_file(const char *name) {
  check_type(name);
  if (code == UNKNOWN) {
    #ifdef DEBUG
      fprintf(debuglog, "unknow file type: %s\n", name);
    #endif
    return;
  }

  FILE *file = fopen(name, "r");
  printf("counting file: %s\n", name);
  if (!file) {
    if (errno == ENOENT)
      return;
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  switch(code) {
    case C:
    case CPP:
    case HEADER:
    case JAVA:
      c_counter(file);
      break;
    case PYTHON:
    case PERL:
    case SHELL:
      sh_counter(file);
      break;
    default:
      break;
  }

  if(fclose(file)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
}

void process_dir(const char *name) {
  DIR *p_dir;
  if ((p_dir = opendir(name)) == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  struct dirent *p_dirent;

  // set errno = 0 to distinguish between an
  //end-of-directory condition or an error
  // errno = 0;

  while ((p_dirent = readdir(p_dir)) != NULL) {
    if(skip_some_entries(p_dirent->d_name))
      continue;
    // basename + '/' + filename + '\0'
    size_t fullnamelen = strlen(name) + 1 + strlen(p_dirent->d_name) + 1;
    char fullname[fullnamelen];
    strcpy(fullname, name);
    strcat(strcat(fullname, "/"), p_dirent->d_name);

    if (is_dir(fullname)) {
      // printf("%s is a directory\n", fullname);
      process_dir(fullname);
    } else
      process_file(fullname);
      // printf("%s is a regular file\n", fullname);

    // printf("%s\n", fullname);
  }
  closedir(p_dir);
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
    switch (code) {
      case C:
        result.c++;
        break;
      case CPP:
        result.cpp++;
        break;
      case HEADER:
        result.header++;
        break;
      case JAVA:
        result.java++;
        break;
      default:
        break;
    }

    // 嵌入代码行的注释
    if (embeded_comment(ptr))
      result.comment++;

  }
}

void sh_counter(FILE *file) {
  char buffer[BUFSIZE];

  while (fgets(buffer, BUFSIZE-1, file)) {
    unsigned len = strlen(buffer);
    char *ptr = buffer;

    // 跳过行前空白
    while (isspace(*ptr) && *ptr != '\n')
      ptr++;

    // 空白行
    if (*ptr == '\n') {
      result.blank++;
      continue;
    } else if (*ptr == '#') {
      result.comment++;
      continue;
    }

    // 代码
    switch (code) {
      case PYTHON:
        result.python++;
        break;
      case PERL:
        result.perl++;
        break;
      case SHELL:
        result.shell++;
        break;
      default:
        break;
    }
  }
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

bool is_dir(const char *pathname) {
  struct stat file_mode;

  if (lstat(pathname, &file_mode) == -1)
    return false;

  return S_ISDIR(file_mode.st_mode);
}

bool skip_some_entries(const char *entry_name) {
  // skip . and ..
  if (!strcmp(entry_name, ".") ||
      !strcmp(entry_name, "..") ||
      // skip repos
      !strcmp(entry_name, ".git") ||
      !strcmp(entry_name, ".svn") ||
      !strcmp(entry_name, ".hg") ||
      !strcmp(entry_name, ".repo")
    )
    return true;
  return false;
}

void check_type(const char *name) {
  if(!strncmp(name, "./", 2))
    name += 2;
  else if (!strncmp(name, "../", 3))
    name += 3;

  const char *p_suffix = name;
  // walk to the end
  while(*p_suffix != '\0')
    p_suffix++;
  // walk back to the last '.' character
  while(*p_suffix != '.' && p_suffix > name)
    p_suffix--;

  if (*p_suffix != '.') {
    code = UNKNOWN;
    return;
  }

  size_t len = strlen(p_suffix);
  char suffix[len+1];
  strcpy(suffix, p_suffix);
  char *ptr_suffix = suffix;
  //change suffix to lower case
  while (*ptr_suffix) {
    *ptr_suffix = tolower(*ptr_suffix);
    ptr_suffix++;
  }

  if (!strcmp(suffix, ".c"))
    code = C;
  else if (!strcmp(suffix, ".cc") || !strcmp(suffix, ".cpp"))
    code = CPP;
  else if (!strcmp(suffix, ".java"))
    code = JAVA;
  else if (!strcmp(suffix, ".h"))
    code = HEADER;
  else if (!strcmp(suffix, ".py") || !strcmp(suffix, ".pyw"))
    code = PYTHON;
  else if (!strcmp(suffix, ".sh"))
    code = SHELL;
  else
    code = UNKNOWN;
}
