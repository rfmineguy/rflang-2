#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>       //getcwd
#include "parsers/parser3.h"
#include "tokenizer3.h"
#include "analysis.h"
#include "argparse.h"
#include "args.h"
#include "compile.h"
// #include "codegen.h"

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

int test_new_tokenizer() {
  const char* file = "code/test/new_expr.rf";
  FILE* f = fopen(file, "r");
  if (!f) {
    fprintf(stderr, "'%s' not present\n", file);
    return -1;
  }
  printf("Testing tokenizer3\n");
  tokenizer3_t t = tokenizer3_new(f);
  
  while (tokenizer3_get(&t, 2).type != T_EOF) {
    tokenizer3_advance(&t);
    printf("========================\n");
    tokenizer3_token_print(t.history[0], &t);
    tokenizer3_token_print(t.history[1], &t);
    tokenizer3_token_print(t.history[2], &t);
    tokenizer3_token_print(t.history[3], &t);
    tokenizer3_token_print(t.history[4], &t);
  }

  printf("Tested tokenizer3\n");
  tokenizer3_free(&t);
  fclose(f);
}

// https://github.com/cofyc/argparse/blob/master/tests/basic.c
int main(int argc, const char** argv) {
  arg_handle_result_t r = args_handle(argc, argv);
  if (!(r.errors |= ERR_NO_IN_FILE)) {
    fprintf(stderr, "ERROR: No input source file supplied. Try -f\n");
  }
  if (!(r.errors |= ERR_NO_OUT_FILE)) {
    fprintf(stderr, "ERROR: No output filepath supplied. Try -o\n");
  }
  if (!(r.errors |= ERR_NO_PLATFORM)) {
    fprintf(stderr, "ERROR: No platform supplied. Try -p. See -l for list\n");
  }
  if (r.args.test) {
    return test_new_tokenizer();
  }
  if (r.args.list_comp_platforms) {
    printf("Platforms:\n");
    printf(" - x86_32-linux\n");
    printf(" - x86_64-linux\n");
    printf(" - arm64\n");
    return 420;
  }
  return compile(r.args);
}
