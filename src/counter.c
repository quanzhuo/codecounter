#include "counter.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

#define BLOCK   80

void check_type(const char *);

#ifdef DEBUG
  extern FILE *debuglog;
#endif

struct statis_data result;
enum code_t code = UNKNOWN;

void analysis_file(char *name) {
  check_type(name);
  if (code == UNKNOWN) {
    #ifdef DEBUG
      fprintf(debuglog, "unknow file type: %s\n", name);
    #endif
    return;
  }

  FILE *file = fopen(name, "r");
  printf("analysizing file: %s\n", name);
  if (!file) {
    // 当符号链接指向的文件不存在时，简单跳过
    if (errno == ENOENT)
      return;
    perror("fopen");
    exit(EXIT_FAILURE);
  }

  switch(code) {
    case ASM_C:
    case C:
    case CPP:
    case HEADER:
    case JAVA:
    case GO:
      c_style_counter(file);
      break;
    case PYTHON:
    case PERL:
    case SHELL:
    case MAKEFILE:
      sh_style_counter(file);
      break;
    case PLAINTEXT:
      plaintext_counter(file);
    case ASM_SEMI:
      asm_semi_counter(file);
    case XML:
      xml_style_counter(file);
    default:
      break;
  }

  if(fclose(file)) {
    perror("fclose");
    exit(EXIT_FAILURE);
  }
}

void loop_dir(const char *name) {
  DIR *p_dir;
  if ((p_dir = opendir(name)) == NULL) {
    perror("opendir");
    exit(EXIT_FAILURE);
  }

  struct dirent *p_dirent;
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
      loop_dir(fullname);
    } else
      analysis_file(fullname);
  }
  closedir(p_dir);
}

void c_style_counter(FILE *file) {
  bool in_multi_line_comment = false;
  char *buffer = NULL;

  while (!mygetline(&buffer, file)){
    unsigned len = strlen(buffer);
    char *ptr = buffer;

    // 处于多行注释中
    if (in_multi_line_comment) {
      result.comment++;
      result.total++;
      // 多行注释是否结束？
      if (strstr(buffer, "*/") != NULL)
        in_multi_line_comment = false;
        free(buffer);
        continue;
    }

    // 跳过行前空白
    while (isspace(*ptr) && *ptr != '\n')
      ptr++;

    // 空白行
    if (*ptr == '\n') {
      result.blank++;
      result.total++;
      free(buffer);
      continue;
    }

    // 独占一行的单行注释
    if (*ptr == '/' && *(ptr + 1) == '/') {
      result.comment++;
      result.total++;
      free(buffer);
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
        free(buffer);
      continue;
    }

    // 代码行
    switch (code) {
      case C:
        result.c++;
        break;
      case HEADER:
        result.header++;
        break;
      case CPP:
        result.cpp++;
        break;
      case JAVA:
        result.java++;
        break;
      case ASM_C:
        result.assembly++;
        break;
      case GO:
        result.go++;
        break;
      default:
        break;
    }

    // 嵌入代码行的注释
    if (embeded_comment(ptr))
      result.comment++;

    free(buffer);
  }
}

void sh_style_counter(FILE *file) {
  char *buffer;

  while (!mygetline(&buffer, file)) {
    unsigned len = strlen(buffer);
    char *ptr = buffer;

    // 跳过行前空白
    while (isspace(*ptr) && *ptr != '\n')
      ptr++;

    // 空白行
    if (*ptr == '\n') {
      result.blank++;
      free(buffer);
      continue;
    } else if (*ptr == '#') {
      result.comment++;
      free(buffer);
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
      case MAKEFILE:
        result.makefile++;
        break;
      case PLAINTEXT:
        result.plaintext++;
      default:
        break;
    }
    free(buffer);
  }
}

void asm_semi_counter(FILE *stream) {
  char *buffer = NULL;
  while (!mygetline(&buffer, stream)) {
    char *ptr = buffer;

    // 跳过行前空白
    while (isspace(*ptr) && *ptr != '\n')
      ptr++;

    // 空白行
    if (*ptr == '\n') {
      result.blank++;
      free(buffer);
      continue;
    } else if (*ptr == ';') {
      result.comment++;
      free(buffer);
      continue;
    }

    // 代码
    result.assembly++;
    free(buffer);
  }
}

void xml_style_counter(FILE *file) {
  bool in_multi_line_comment = false;
  char *buffer = NULL;
  while (!mygetline(&buffer, file)){
    unsigned len = strlen(buffer);
    char *ptr = buffer;

    // 处于多行注释中
    if (in_multi_line_comment) {
      result.comment++;
      result.total++;
      // 多行注释是否结束？
      if (strstr(buffer, "-->") != NULL)
        in_multi_line_comment = false;
        free(buffer);
        continue;
    }

    // 跳过行前空白
    while (isspace(*ptr) && *ptr != '\n')
      ptr++;

    // 空白行
    if (*ptr == '\n') {
      result.blank++;
      result.total++;
      free(buffer);
      continue;
    }

    // comment
    if (*ptr == '<' && *(ptr + 1) == '!' && *(ptr+2) == '-' && *(ptr+3) == '-'){
      in_multi_line_comment = true;
      result.comment++;
      if(strstr(ptr, "-->"))
        in_multi_line_comment = false;
      free(buffer);
      continue;
    }

    // 代码行
    result.xml++;
    free(buffer);
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
  size_t namelen = strlen(name);
  char lowername[namelen + 1];
  strcpy(lowername, name);

  char *p_suffix = lowername;
  while (*p_suffix){
    *p_suffix = tolower(*p_suffix);
    p_suffix++;
  }

  if (ends_with(lowername, "makefile")){
    code = MAKEFILE;
    return ;
  } else if (ends_with(lowername, "readme")) {
    code = PLAINTEXT;
    return ;
  }

  // walk back to the last '.' character
  while(*p_suffix != '.' && p_suffix > lowername)
    p_suffix--;

  if (*p_suffix != '.') {
    code = UNKNOWN;
    return;
  }

  if (!strcmp(p_suffix, ".c"))
    code = C;
  else if (!strcmp(p_suffix, ".cc") || !strcmp(p_suffix, ".cpp"))
    code = CPP;
  else if (!strcmp(p_suffix, ".java"))
    code = JAVA;
  else if (!strcmp(p_suffix, ".h"))
    code = HEADER;
  else if (!strcmp(p_suffix, ".s"))
    code = ASM_C;
  else if (!strcmp(p_suffix, ".asm"))
    code = ASM_SEMI;
  else if (!strcmp(p_suffix, ".py") || !strcmp(p_suffix, ".pyw"))
    code = PYTHON;
  else if (!strcmp(p_suffix, ".sh"))
    code = SHELL;
  else if (!strcmp(p_suffix, ".md") || !strcmp(p_suffix, ".txt"))
    code = PLAINTEXT;
  else if(!strcmp(p_suffix, ".mk"))
    code = MAKEFILE;
  else if(!strcmp(p_suffix, ".xml"))
    code = XML;
  else if(!strcmp(p_suffix, ".go"))
    code = GO;
  else
    code = UNKNOWN;
}

// 读取 stream 中的一行存储在 *buffer 所指向的缓冲区中。
// 用户必须自己释放 *buffer 指向的内存
// gnu c 库中的 getline 函数可以完成同样的任务
// 本函数并没有考虑文件空洞的存在
// return : 0 on success;
//         -1 on error;
ssize_t mygetline(char **buffer, FILE *stream) {
  size_t buffer_len = BLOCK;
  size_t bytes_read = 0;
  char *ptr = NULL;

  // 分配 BLOCK 字节
  if ( !(*buffer = (char *)malloc(BLOCK)) )
    return -1;

  // 从流中读取 BLOCK - 1 字节
  if(!fgets(*buffer, BLOCK, stream))
    return -1;

  // 本次实际读取的字节数
  bytes_read = strlen(*buffer);
  // ptr 指向 '\0'
  ptr = *buffer + bytes_read;

  // 缓冲区太小，一行没读完，重新分配一个更大的缓冲区
  while (*(ptr - 1) != '\n') {
    buffer_len = buffer_len + BLOCK;
    if ( !(*buffer = (char *)realloc(*buffer, buffer_len)) )
      return -1;
    ptr = *buffer + strlen(*buffer);
    if (!fgets(ptr, BLOCK, stream)) {
      char *newbuffer = malloc(ptr - *buffer + 1);
      strncpy(newbuffer, *buffer, ptr - *buffer);
      free(*buffer);
      *buffer = newbuffer;
      return 0;
    }
    bytes_read = strlen(ptr);
    ptr += bytes_read;
  }
  return 0;
}

bool ends_with(const char *str1, const char *str2) {
  size_t len1 = strlen(str1);
  size_t len2 = strlen(str2);
  if (len1 < len2)
    return false;
  return strstr(str1, str2) + len2 == str1 + len1;
}

void plaintext_counter(FILE *stream) {
  char *buffer = NULL;
  while(!mygetline(&buffer, stream)){
    result.plaintext++;
    free(buffer);
  }
}
