#include "args.h"
#include <argparse.h>
#include <stdlib.h>

static const char *const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

arg_handle_result_t args_handle(int argc, const char** argv) {
  arg_handle_result_t r = {0};

  // argparse options
  struct argparse_option options[] = {
    OPT_HELP(),
    OPT_GROUP("Compilation options"),
    OPT_STRING('f', "file", &r.args.in_file, "File to compile", NULL , 0, 0),
    OPT_STRING('o', "output", &r.args.out_file, "Name of output asm file", NULL, 0, 0),
    OPT_STRING('p', "platform", &r.args.comp_platform, "Platform to compile for", NULL, 0, 0),
    OPT_BOOLEAN('l', "platform-list", &r.args.list_comp_platforms, "List supported platforms", NULL, 0, 0),
    OPT_GROUP("Testing options"),
    OPT_BOOLEAN('t', "test", &r.args.test, "Run active test", NULL, 0, 0),
    OPT_END()
  };

  // argparse initialization/execution
  struct argparse argparse;
  argparse_init(&argparse, options, usages, 0);
  argc = argparse_parse(&argparse, argc, argv);
  
  if (r.args.in_file == NULL) {
    r.errors |= ERR_NO_IN_FILE;
  }
  if (r.args.out_file == NULL) {
    r.errors |= ERR_NO_OUT_FILE;
  }
  if (r.args.comp_platform == NULL) {
    r.errors |= ERR_NO_PLATFORM;
  }
  else {
    // TODO: Maybe check if the platform is supported
  }

  return r;
}
