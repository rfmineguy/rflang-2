#include "compile.h"
#include "parsers/parser3.h"
#include "analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       //getcwd

FILE* openf(const char* f, const char* perm) {
  FILE* pf = fopen(f, perm);
  if (!pf) {
    fprintf(stderr, "Could not open [%s]\n", f);
    exit(1);
  }
  return pf;
}

comp_info_t get_comp_info(args_t args) {
  comp_info_t info = {0};
  if (getcwd(info.cwd, PATH_MAX) != NULL) {}
  else {
    fprintf(stderr, "getcwd() error\n");
    exit(1);
  }
  strncat(info.cwd, "/", PATH_MAX);
  
  if (args.in_file == NULL) {
    fprintf(stderr, "No input file specified\n");
    exit(1);
  }

  if (args.out_file == NULL) {
    fprintf(stderr, "No output file specified\n");
    exit(2);
  }

  strncpy(info.input_file_path, info.cwd, PATH_MAX);
  strncat(info.input_file_path, args.in_file, PATH_MAX);

  strncpy(info.output_file_path, info.cwd, PATH_MAX);
  strncat(info.output_file_path, args.out_file, PATH_MAX);
  return info;
}

int compile(args_t args) {
  printf(" ====== Compiling ======\n");
  comp_info_t info = get_comp_info(args);
  printf("CWD: %s\n", info.cwd);
  printf("OutFile: %s\n", info.output_file_path);
  printf("InFile: %s\n", info.input_file_path);

  FILE* input_file = openf(info.input_file_path, "r");
  FILE* output_file = openf(info.output_file_path, "w");

  tokenizer3_t t = tokenizer3_new(input_file);

  program_t* prog = parse(&t);
  show_program(prog, 1);

  analyze_program(prog);
  free_program(prog);
  free(prog);
  prog = NULL;

  tokenizer3_free(&t);

  fclose(input_file);
  fclose(output_file);
  return 0;
}
