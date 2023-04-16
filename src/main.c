#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "parser2.h"
#include "tokenizer2.h"
#include "argparse.h"
#include "codegen.h"

#if defined(__linux__) // any linux distribution
    #define PLATFORM "linux"
#elif defined(_WIN32) // any windows system
    #define PLATFORM "windows"
#else
    #define PLATFORM "Is not linux or windows"
#endif

static const char *const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

typedef struct {
  const char* file;
  int test;
} args;

void execute(const char* cmd, ...) {
  char command[200] = {0};
  va_list l;
  va_start(l, cmd);
  vsprintf(command, cmd, l);
  system(command);
  va_end(l);
}

FILE* openf(const char* f, const char* perm) {
  FILE* pf = fopen(f, perm);
  if (!pf) {
    fprintf(stderr, "Could not open [%s]\n", f);
    exit(1);
  }
  return pf;
}

int compile(args* a) {
  FILE* input_file  = openf(a->file, "r");
  char outfile[100] = {0};
  strncpy(outfile, a->file, 100);
  strncat(outfile, ".out", 100);
  FILE* output_file = openf(outfile, "w");
  tokenizer_t* t = tokenizer_new(input_file);
  program_t* prog = parse(t);
  show_program(prog, 1);

  // Now codegen the program
  codegen_program(prog, output_file);

  // Run nasm and ld
  if (strcmp(PLATFORM, "linux") == 0) {
    printf("Linux\n");
    execute("nasm -g -f elf64 -F dwarf %s -o %s", outfile, "test_out");
  }

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
  return 1;
}

int test(args* a) {
  printf("============== TESTING ==============\n");
  FILE* f = fopen("code/expressions.io", "r");
  if (!f) {
    fprintf(stderr, "Failed to open file\n");
    exit(0);
  }
  tokenizer_t* t = tokenizer_new(f);
  expression_t* e = parse_expression(t);
  show_expression(e, 1);
  free_expression(e);
  tokenizer_free(t);
  fclose(f);
  return 1;
}

// https://github.com/cofyc/argparse/blob/master/tests/basic.c
int main(int argc, const char** argv) {
  args a = {};
  /// ================================================================================================
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_GROUP("Compilation options"),
    OPT_STRING('f', "file", &a.file, "File to compile", NULL , 0, 0),
    OPT_GROUP("Testing options"),
    OPT_BOOLEAN('t', "test", &a.test, "Run tests", NULL, 0, 0), 
    OPT_END()
  };
  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argc = argparse_parse(&argparse, argc, argv);
  /// ================================================================================================
  if (a.test) {
    return test(&a);
  }
  if (a.file == NULL) {
    fprintf(stderr, "You must supply a file to compile\n");
    exit(90);
  }
  else {
    return compile(&a);
  }
}
