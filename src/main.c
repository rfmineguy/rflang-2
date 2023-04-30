#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>       //getcwd
#include "parser2.h"
#include "tokenizer2.h"
#include "argparse.h"
#include "codegen.h"

#if defined(__linux__) // any linux distribution
    #define PLATFORM "linux"
#elif defined(__APPLE__)
    #define PLATFORM "apple"
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
  char cwd[PATH_MAX];  //current working directory
  char input_file_path[PATH_MAX];
  char output_file_path[PATH_MAX];
} compilation_info;

typedef struct {
  const char* in_file;
  const char* out_file;
  const char* comp_platform;
  int test, comp_platform_list;
} args;

void execute(const char* cmd, ...) {
  char command[200] = {0};
  va_list l;
  va_start(l, cmd);
  vsnprintf(command, 200, cmd, l);
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

compilation_info get_compilation_info(args* a) {
  compilation_info info = {0};
  if (getcwd(info.cwd, PATH_MAX) != NULL) {}
  else {
    fprintf(stderr, "getcwd() error\n");
    exit(1);
  }
  strncat(info.cwd, "/", PATH_MAX);
  
  if (a->in_file == NULL) {
    fprintf(stderr, "No input file specified\n");
    exit(1);
  }

  if (a->out_file == NULL) {
    fprintf(stderr, "No output file specified\n");
    exit(2);
  }

  strncpy(info.input_file_path, info.cwd, PATH_MAX);
  strncat(info.input_file_path, a->in_file, PATH_MAX);

  strncpy(info.output_file_path, info.cwd, PATH_MAX);
  strncat(info.output_file_path, a->out_file, PATH_MAX);
  return info;
}

int compile(args* a) {
  compilation_info info = get_compilation_info(a);
  printf("CWD: %s\n", info.cwd);
  printf("OutFile: %s\n", info.output_file_path);
  printf("InFile: %s\n", info.input_file_path);

  FILE* input_file = openf(info.input_file_path, "r");
  FILE* output_file = openf(info.output_file_path, "w");
  tokenizer_t* t = tokenizer_new_from_file(input_file);
  program_t* prog = parse(t);
  show_program(prog, 1);

  // Now codegen the program
  if (a->comp_platform) {
    codegen_select(prog, output_file, a->comp_platform);
  }
  else {
    fprintf(stderr, "Must select a target platform to compile for\n");
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
  return 0;
}

int test(args* a) {
  printf("============== TESTING ==============\n");
  const char* test_file = "code/test.rf";
  FILE* f = fopen(test_file, "r");
  if (!f) {
    fprintf(stderr, "Failed to open file \'%s\'\n", test_file);
    exit(0);
  }
  else {
    printf("TEST: Compiling \'%s\'\n", test_file); 
  }
  tokenizer_t* t = tokenizer_new_from_file(f);
  program_t* p = parse_program(t);
  show_program(p, 1);

  free_program(p);
  tokenizer_free(t);
  fclose(f);
  return 1;
}

// https://github.com/cofyc/argparse/blob/master/tests/basic.c
int main(int argc, const char** argv) {
  args a = {0};
  /// ================================================================================================
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_GROUP("Compilation options"),
    OPT_STRING('f', "file", &a.in_file, "File to compile", NULL , 0, 0),
    OPT_STRING('o', "output", &a.out_file, "Name of output asm file", NULL, 0, 0),
    OPT_STRING('p', "platform", &a.comp_platform, "Platform to compile for", NULL, 0, 0),
    OPT_BOOLEAN('l', "platform-list", &a.comp_platform_list, "List supported platforms", NULL, 0, 0),
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
  if (a.comp_platform_list) {
    printf("Platforms:\n");
    printf(" - x86_32-linux\n");
    printf(" - x86_64-linux\n");
    printf(" - arm64\n");
    exit(89);
  }
  /// ================================================================================================
  if (a.in_file == NULL) {
    fprintf(stderr, "You must supply a file to compile\n");
    exit(90);
  }
  else {
    return compile(&a);
  }
  /// ================================================================================================
}
