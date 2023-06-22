#include "file_util.h"
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int file_util_check_input_modules_exist(args2_t* args, const char** failed_module) {
  int i;
  for (i = 0; i < args->input_modules_count; i++) {
    if (strchr(args->input_modules[i], '.') != NULL) {
      fprintf(stderr, "Supplying modules with extension is not supported. Try removing it.\n");
      break;
    }
    char with_ext[PATH_MAX];
    strncpy(with_ext, args->input_modules[i], PATH_MAX);
    strncat(with_ext, ".rf", PATH_MAX);
    char* rp = realpath(with_ext, NULL);
    printf("%s: path=%s, rp=%s\n", __func__, with_ext, rp);
    if (!rp) {
      free(rp);
      break;
    }
    FILE* f = fopen(args->input_modules[i], "r");
    if (!f) {
      fclose(f);
      free(rp);
      break;
    }
    fclose(f);
    free(rp);
  }
  if (i == args->input_modules_count) {
    *failed_module = args->input_modules[i];
    return 0;
  }
  return 1;
}

int file_util_stdlib_module_exists(const char* module, char* out_module_full_path, int max_length) {
  const char* stdlib_path = "/usr/local/include/rfcstd/";

  out_module_full_path[0] = 0;
  strncat(out_module_full_path, stdlib_path, max_length < PATH_MAX ? max_length : PATH_MAX);
  strncat(out_module_full_path, module,      max_length < PATH_MAX ? max_length : PATH_MAX);
  if (strchr(out_module_full_path, '.') == NULL) {
    strncat(out_module_full_path, ".rf",       max_length < PATH_MAX ? max_length : PATH_MAX);
  }

  printf("out_module_full_path: %s\n", out_module_full_path);
  FILE* f = fopen(out_module_full_path, "r");
  if (!f) {
    fclose(f);
    return 0;
  }
  fclose(f);
  return 1;
}

int file_util_local_module_exists(const char* module, char* out_module_full_path, int max_length) {
  char cwd[PATH_MAX] = {0};
  if (getcwd(cwd, PATH_MAX) == NULL) {
    fprintf(stderr, "ERROR. getcwd\n");
    return 0;
  }

  out_module_full_path[0] = 0;
  strncat(out_module_full_path, cwd   , max_length < PATH_MAX ? max_length : PATH_MAX);
  strncat(out_module_full_path, "/"   , max_length < PATH_MAX ? max_length : PATH_MAX);
  strncat(out_module_full_path, module, max_length < PATH_MAX ? max_length : PATH_MAX);
  if (strchr(out_module_full_path, '.') == NULL) {
    strncat(out_module_full_path, ".rf" , max_length < PATH_MAX ? max_length : PATH_MAX);
  }

  printf("out_module_full_path: %s\n", out_module_full_path);
  FILE* f = fopen(out_module_full_path, "r");
  if (!f) {
    fclose(f);
    return 0;
  }
  fclose(f);
  return 1;
}
