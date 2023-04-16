#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser2.h"
#include "tokenizer2.h"
#include "argparse.h"
#include "codegen.h"

static const char *const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

typedef struct {
  const char* file;
} args;

FILE* openf(const char* f, const char* perm) {
  FILE* pf = fopen(f, perm);
  if (!pf) {
    fprintf(stderr, "Could not open [%s]\n", f);
    exit(1);
  }
  return pf;
}

// https://github.com/cofyc/argparse/blob/master/tests/basic.c
int main(int argc, const char** argv) {
  args a = {};
  /// ================================================================================================
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_GROUP("Basic options"),
    OPT_STRING('f', "file", &a.file, "File to compile", NULL , 0, 0),
    OPT_END()
  };
  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argc = argparse_parse(&argparse, argc, argv);
  /// ================================================================================================

  if (a.file == NULL) {
    fprintf(stderr, "You must supply a file to compile\n");
    exit(90);
  }

  FILE* input_file  = openf(a.file, "r");
  char outfile[100] = {0};
  strncpy(outfile, a.file, 100);
  strncat(outfile, ".out", 100);
  FILE* output_file = openf(outfile, "w");
  tokenizer_t* t = tokenizer_new(input_file);
  program_t* prog = parse(t);
  show_program(prog, 1);

  // Now codegen the program
  codegen_program(prog, output_file);

  // Free the program
  free_program(prog);
  free(prog);
  prog = NULL;

  // Free the tokenizer
  tokenizer_free(t);
  free(t);
  t = NULL;

  // Close files
  fclose(input_file);
  fclose(output_file);
}
