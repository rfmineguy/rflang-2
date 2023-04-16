#include <stdio.h>
#include <stdlib.h>
#include "parser2.h"
#include "tokenizer2.h"
#include "argparse.h"

static const char *const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

typedef struct {
  const char* file;
} args;

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

  FILE* f = fopen(a.file, "r");
  if (!f) {
    fprintf(stderr, "Failed to open file\n");
    exit(1);
  }
  tokenizer_t* t = tokenizer_new(f);
  parse(t);

  tokenizer_free(t);
  free(t);
  t = NULL;
}
