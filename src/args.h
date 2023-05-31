#ifndef ARGS_H
#define ARGS_H

typedef enum {
  ERR_NO_IN_FILE    = 0b1,
  ERR_NO_OUT_FILE   = 0b10,
  ERR_NO_PLATFORM   = 0b100
} handle_error_t;

typedef struct {
  const char* in_file;
  const char* out_file;
  const char* comp_platform;
  int list_comp_platforms;
  int test;
} args_t;

typedef struct {

} compile_info_t;

typedef struct {
  args_t args;
  compile_info_t comp_info;
  int errors;
} arg_handle_result_t;

arg_handle_result_t args_handle(int, const char**);

#endif
