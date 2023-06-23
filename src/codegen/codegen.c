#include "codegen.h"
#include "codegen_x86_64.h"
#include <string.h>

#define UNSUPPORTED(target) fprintf(stderr, "Platform currently unsupported [%s]\n", #target)

// NOTE: All code generated will be in x86_64 nasm assembly... for now

void codegen_select(module_t* module, FILE* f, const char* platform) {
  if (platform == NULL) {
    fprintf(stderr, "No platform supplied\n");
  }
  else if (strcmp(platform, "x86_64-linux") == 0) {
    printf("CODEGEN: generating code for x86_64-linux\n");
    x86_64_codegen_module(module, f);
  }
  else if (strcmp(platform, "x86_32-linux") == 0) {
    UNSUPPORTED("x86_32-linux");
  }
  else if (strcmp(platform, "arm64") == 0) {
    UNSUPPORTED("arm64");
  }
  else if (strcmp(platform, "host") == 0) {
    UNSUPPORTED("host");
  }
}
