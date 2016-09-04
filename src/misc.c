/* Date: Sun Sep  4 13:48:30 CST 2016
** Desc: 该文件包含一些辅助函数
*/

#include "misc.h"


void print_result(struct statis_data *ptr_data) {
  printf("Totally: %10ld\n", ptr_data->total);
  printf("Code:    %10ld\n", ptr_data->code);
  printf("Blank:   %10ld\n", ptr_data->blank);
  printf("Comment: %10ld\n", ptr_data->comment);
}

void print_usage() {
  printf("Usage: codecounter [OPTION]... FILE\n");
  printf("\nstatistics your source code. supply a file or directory\n"
         "that contains the source files. codecounter will print\n"
         "how many lines of code, blank and comment in you project\n\n");
}
