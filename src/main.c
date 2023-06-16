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
    printf("No test registered\n");
    return 0;
  }
  if (r.args.list_comp_platforms) {
    printf("Platforms:\n");
    printf(" - x86_32-linux\n");
    printf(" - x86_64-linux\n");
    printf(" - arm64\n");
    printf(" - host\n");
    return 420;
  }
  return compile(r.args);
}
