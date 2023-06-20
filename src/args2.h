#ifndef ARGS2_H
#define ARGS2_H

#include <unistd.h>

typedef struct {
  const char*         input_files[100];
  int                 input_files_count;

  const char*         include_dirs[100];
  int                 include_dirs_count;

  int                 assembly_flag;
  const char*         assembly_file;
  const char*         comp_platform;
  const char*         out_obj_file_name;
  int                 list_comp_platforms;
  int                 test;
} args2_t;

void  args2_help();
void  args2_show(args2_t*);
int   args2_handle(int argc, const char** argv, args2_t* p_args);

#endif
