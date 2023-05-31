#ifndef COMPILE_H
#define COMPILE_H
#include "args.h"
#include <limits.h>

typedef struct {
  char cwd[PATH_MAX];  //current working directory
  char input_file_path[PATH_MAX];
  char output_file_path[PATH_MAX];
} comp_info_t;

comp_info_t get_comp_info(args_t);
int         compile(args_t);

#endif
