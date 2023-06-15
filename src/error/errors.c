#include "tokenizer2.h"
#include "errors.h"
#include <string.h>
#include <stdarg.h>

void error_print(error_t e, tokenizer_t* t,  const char* msg, const char* func_name, int line, ...) {
  printf("[%s, %d] ERROR: %d:%d\n", func_name, line, e.line, e.column);
  line_info_t line_info = get_line_from_str(t->source_str, e.line + 1);
  printf("%.*s\n", line_info.end - line_info.begin, t->source_str + line_info.begin);

  for (int i = 0; i < e.column; i++)
    printf(" ");

  char buf[300] = {0};
  va_list args;
  va_start(args, msg);
  strncat(buf, "^ ", 300);
  strncat(buf, msg, 300);
  vprintf(buf, args);
  va_end(args);
}

line_info_t get_line_from_str(char* str, int target_line) {
  line_info_t t = {0};
  int line = 0;
  char* result;
  char* rest = str;

  while ((result = strtok_r(rest, "\n", &rest)) != NULL) {
    if (line == target_line - 1) {
      printf("%s", result);
      break;
    }
    line++;
  }
  return t;
}
