#include "codegen.h"
#include "codegen_x86_64.h"
#include <string.h>

#define UNSUPPORTED(target) fprintf(stderr, "Platform unsupported [%s]\n", #target)

// NOTE: All code generated will be in x86_64 nasm assembly

void codegen_select(program_t* prog, FILE* f, const char* platform) {
  printf("Platform: %s\n", platform);
  if (strcmp(platform, "x86_64-linux") == 0) {
    printf("CODEGEN: generating code for x86_64-linux\n");
    x86_64_codegen_program(prog, f);
  }
  else if (strcmp(platform, "x86_32-linux") == 0) {
    UNSUPPORTED("x86_32-linux");
  }
  else if (strcmp(platform, "arm64") == 0) {
    UNSUPPORTED("arm64");
  }
}
