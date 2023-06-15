#ifndef ERRORS_H
#define ERRORS_H

typedef struct {
  int line;
  int column;
} error_t;

typedef struct {
  error_t errors[100];
} manager_t;

typedef struct {
  int valid;
  int begin, end;
} line_info_t;

void print_line_str(const char*, int);

line_info_t get_line_from_str(char*, int);

void error_print(error_t, tokenizer_t*, const char*, const char*, int, ...);


#endif
