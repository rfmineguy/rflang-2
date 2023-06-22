#include "args2.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void args2_help() {
  printf("Usage: rfc <options>\n\n");
  printf("  -h                : Display this help menu\n");
  printf("  -f <modules_list> : Supply list of modules to compile (NOTE: do not specify the file extension)\n");
  printf("  -I <inc_dir_list> : Supply list of directories to treat as include dirs\n");
  printf("  -S                : Whether rfc should generate assembly files for the input files\n");
  printf("  -o <file_name>    : Supply a filename for the generated executable\n");
  printf("  -p <platform>     : Choose which platform to compile for (i.e. x86_64_linux) [See -l for more]\n");
  printf("  -l                : List the available platforms available to compile for\n");
}

void args2_show(args2_t* args) {
  printf("args:\n");
  printf("  input_modules: \n");
  for (int i = 0; i < args->input_modules_count; i++) {
    printf("   - %s\n", args->input_modules[i]);
  }
  printf("  include_dirs: \n");
  for (int i = 0; i < args->include_dirs_count; i++) {
    printf("   - %s\n", args->include_dirs[i]);
  }
  printf("assembly_flag: %s\n", args->assembly_flag ? "ON" : "OFF");
  printf("assembly_file: %s\n", args->assembly_file);
  printf("comp_platform: %s\n", args->comp_platform);
  printf("list_platform: %s\n", args->list_comp_platforms ? "ON" : "OFF");
  printf("====================================\n");
}

int args2_handle(int argc, const char** argv, args2_t* p_args) {
  // Set up constant fields
  p_args->input_modules_count = 0;
  p_args->include_dirs_count = 0;

  // Parse arg list
  char c;
  while ((c = getopt(argc, (char**)argv, "ho:p:S:lf:I:")) != -1) {
    switch (c) {
      case 'h':
        args2_help();
        break;
      case 'f':
        optind--;
        p_args->input_modules_count = 0;
        while (optind < argc && *argv[optind] != '-') {
          if (p_args->input_modules_count > 99) {
            fprintf(stderr, "Too many input modules. { supplied (%d), max (%d) }\n", p_args->input_modules_count, 100);
            return 6;
          }
          if (strchr(argv[optind], '.') != NULL) {
            fprintf(stderr, "Input modules should not include extension. Error with [%s]\n", argv[optind]);
            return 7;
          }
          p_args->input_modules[p_args->input_modules_count++] = argv[optind];
          optind++;
        }
        break;
      case 'I':
        optind--;
        while (optind < argc && *argv[optind] != '-') {
          if (p_args->include_dirs_count > 99) {
            fprintf(stderr, "Too many include directories. { supplied (%d), max (%d) }\n", p_args->include_dirs_count, 100);
            return 8;
          }
          p_args->include_dirs[p_args->include_dirs_count++] = argv[optind];
          optind++;
        }
        break;
      case 'S':
        p_args->assembly_flag = 1;
        p_args->assembly_file = optarg;
        break;
      case 'o':
        p_args->out_obj_file_name = optarg;
        break;
      case 'p':
        p_args->comp_platform = optarg;
        break;
      case 'l':
        p_args->list_comp_platforms = 1;
        break ;
      case '?':
        if (optopt == 'c' || optopt == 'o' || optopt == 'p') {
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
          return 1;
        }
        else if (optopt == 'f') {
          fprintf(stderr, "Option -%c requires at least on file.\n", optopt);
          return 2;
        }
        else if (isprint (optopt)) {
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
          return 3;
        }
        else {
          fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
          return 4;
        }
      default:
        fprintf(stderr, "Default: %c\n", optopt);
        return 5;
    }
  }
  return 0; //success
}
