/* Date: Sun Sep  4 13:48:30 CST 2016
** Desc: 该文件包含一些辅助函数
*/

#include "misc.h"
#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG
  FILE *debuglog = NULL;
#endif

void print_result(struct statis_data *ptr_data) {
  double  total = ptr_data->c + ptr_data->cpp + ptr_data->java
                + ptr_data->header + ptr_data->python + ptr_data->perl
                + ptr_data->shell + ptr_data->makefile +ptr_data->plaintext
                + ptr_data->comment + ptr_data->blank;
  printf("\nITEM:       %10s%10s\n", "COUNT", "PERCENT");
  if (ptr_data->c)
    printf("c:        %10ld%10.2f\n", ptr_data->c, ptr_data->c / total);
  if (ptr_data->cpp)
    printf("cpp:      %10ld%10.2f\n", ptr_data->cpp, ptr_data->cpp / total);
  if (ptr_data->header)
    printf("header:   %10ld%10.2f\n", ptr_data->header, ptr_data->header / total);
  if (ptr_data->java)
    printf("java:     %10ld%10.2f\n", ptr_data->java, ptr_data->java / total);
  if (ptr_data->python)
    printf("python:   %10ld%10.2f\n", ptr_data->python, ptr_data->python / total);
  if (ptr_data->perl)
    printf("perl:     %10ld%10.2f\n", ptr_data->perl, ptr_data->perl / total);
  if(ptr_data->shell)
    printf("shell:    %10ld%10.2f\n", ptr_data->shell, ptr_data->shell / total);
  if(ptr_data->makefile)
    printf("makefile: %10ld%10.2f\n", ptr_data->makefile, ptr_data->makefile / total);
  if (ptr_data->comment)
    printf("comment:  %10ld%10.2f\n", ptr_data->comment, ptr_data->comment / total);
  if(ptr_data->plaintext)
    printf("plaintext:%10ld%10.2f\n", ptr_data->plaintext, ptr_data->plaintext / total);
  if(ptr_data->blank)
    printf("blank:    %10ld%10.2f\n", ptr_data->blank, ptr_data->blank / total);
  printf("Totally:  %10.0f\n", total);
}

void usage_and_quit() {
  printf("Usage: codecounter [-h|--help]\n");
  printf("       codecounter PATH ...\n");
  printf("\nstatistics your source code. supply some files or dirs\n"
         "codecounter will print how many lines of code, blank and\n"
         "comment in you project\n\n");
  exit(EXIT_SUCCESS);
}
