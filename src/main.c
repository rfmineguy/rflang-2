#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <limits.h>
#include <unistd.h>       //getcwd
#include "parsers/parser3.h"
#include "tokenizer3.h"
#include "analysis.h"
#include "args2.h"
#include "compile.h"
#include "logging/logger_w_channels.h"

#if defined(__linux__) // any linux distribution
    #define PLATFORM "linux"
#elif defined(__APPLE__)
    #define PLATFORM "apple"
#elif defined(_WIN32) // any windows system
    #define PLATFORM "windows"
#else
    #define PLATFORM "Is not linux, apple, or windows"
#endif

void setup_logger_channels() {
  logger_channel_set_stream(logger_get_global(), WARNING_CHANNEL, stdout);
  logger_channel_set_stream(logger_get_global(), ERROR_CHANNEL, stdout);
  logger_channel_set_stream(logger_get_global(), PARSE_STATUS_CHANNEL, stdout);
  logger_channel_enable(logger_get_global(), WARNING_CHANNEL,      "Warn",        ANSI_RESET);
  logger_channel_enable(logger_get_global(), ERROR_CHANNEL,        "Error",       ANSI_RED);
  logger_channel_enable(logger_get_global(), PARSE_STATUS_CHANNEL, "ParseStatus", ANSI_RESET);
}

int main(int argc, const char** argv) {
  // Parse and retrieve arguments
  args2_t args = {0};
  int result = args2_handle(argc, argv, &args); 
  if (result != 0) {
    fprintf(stderr, "Argument parsing error(code: %d)\n", result);
    args2_help();
    return result;
  }
  args2_show(&args);

  // LIST COMP PLATFORMS
  if (args.list_comp_platforms) {
    printf("Platforms:\n");
    printf(" - x86_32-linux\n");
    printf(" - x86_64-linux\n");
    printf(" - arm64\n");
    printf(" - host\n");
    return 420;
  }
  if (args.input_modules_count == 0) {
    fprintf(stderr, "ERROR: Must supply at least one file to compile\n");
    return 421;
  }

  //setup_logger_channels();
  return compile_all(args);
}
