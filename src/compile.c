#include "compile.h"
#include "codegen/codegen.h"
#include "analysis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       //getcwd

FILE* openf(const char* f, const char* perm) {
  FILE* pf = fopen(f, perm);
  if (!pf) {
    fprintf(stderr, "Could not open [%s]\n", f);
    exit(1);
  }
  return pf;
}

comp_info_t get_comp_info(args2_t args) {
  comp_info_t info = {0};
  // if (getcwd(info.cwd, PATH_MAX) != NULL) {}
  // else {
  //   fprintf(stderr, "getcwd() error\n");
  //   exit(1);
  // }
  // strncat(info.cwd, "/", PATH_MAX);
  // 
  // if (args.in_file == NULL) {
  //   fprintf(stderr, "No input file specified\n");
  //   exit(1);
  // }

  // if (args.out_file == NULL) {
  //   fprintf(stderr, "No output file specified\n");
  //   exit(2);
  // }

  // strncpy(info.input_file_path, info.cwd, PATH_MAX);
  // strncat(info.input_file_path, args.in_file, PATH_MAX);

  // strncpy(info.output_file_path, info.cwd, PATH_MAX);
  // strncat(info.output_file_path, args.out_file, PATH_MAX);
  return info;
}

int check_all_files_exist(args2_t args, const char** failed_file) {
  int i;
  for (i = 0; i < args.input_files_count; i++) {
    // make sure the listed files all exist
    char* rp = realpath(args.input_files[i], NULL);
    if (!rp) {
      free(rp);
      break;
    }
    FILE* f = fopen(args.input_files[i], "r");
    if (!f) {
      fclose(f);
      free(rp);
      break;
    }
    fclose(f);
    free(rp);
  }
  if (i != args.input_files_count) {
    *failed_file = args.input_files[i];
    return 0;
  }
  return 1;
}

int compile_all(args2_t args) {
  /* POTENTIAL ISSUES WITH THIS
   *  - For standard library functions, how would this work?
   *    + It would be have to be "on demand to some extent", sort of like 
   */

  /* SOLUTION
   *  - Have some standardized path folder for where the standard lib modules go
   *    + e.g. libc is in /usr/include
   *    + we could do something like /usr/rfc/std/
   *      * /usr/rfc/std/io.rf
   *      * /usr/rfc/std/math.rf
   *      * /usr/rfc/std/string.rf
   *      * /usr/rfc/std/stddef.rf
   *    + If the used module doesn't exist locally use the args.include_dirs to look for the requested module
   *      If it doesn't exist, obviously there was a problem
   */

  // Esure that all the files supplied actually do exist before moving on to parsing
  printf("Compiling all\n");
  const char* failed_file = NULL;
  if (!check_all_files_exist(args, &failed_file)) {
    fprintf(stderr, "'%s' doesn't exist\n", failed_file);
    return 1;
  }

  // Begin parsing source files
  chaining_ht_str_program_t cached_parses = chaining_ht_str_program_alloc(10);
  for (int i = 0; i < args.input_files_count; i++) {
    if (!chaining_ht_str_program_contains(cached_parses, (char*) args.input_files[i])) {
      program_t* prog = parse_file(args.input_files[i], &args);
      chaining_ht_str_program_put(cached_parses, (char*) args.input_files[i], prog);
      printf("Parsed, '%s'\n", args.input_files[i]);
    }
  }

  // show_program(chaining_ht_str_program_find(cached_parses, "entry.rf").p, 0);
  // chaining_ht_str_program_show(cached_parses, 0);

  // Start analysis of the modules
  
  
  chaining_ht_str_program_show(cached_parses, 0);
  chaining_ht_str_program_free(cached_parses);
}

program_t* parse_file(const char* file_path, args2_t* args) {
  program_t* prog = NULL;
  char* rp = realpath(file_path, NULL); //NOTE: I think some better error checking could happen here; see `man 3 realpath`

  tokenizer3_t t = tokenizer3_new(rp);
  prog = parse(&t);
  tokenizer3_free(&t);

  free(rp);
  return prog;
}

int compile(args2_t args) {
  // printf(" ====== Compiling ======\n");
  // comp_info_t info = get_comp_info(args);
  // printf("CWD: %s\n", info.cwd);
  // printf("OutFile: %s\n", info.output_file_path);
  // printf("InFile: %s\n", info.input_file_path);

  // // FILE* input_file = openf(info.input_file_path, "r");
  // FILE* output_file = openf(info.output_file_path, "w");

  // tokenizer3_t t = tokenizer3_new(info.input_file_path);

  // program_t* prog = parse(&t);
  // printf("Show program: DISABLED\n");
  // show_program(prog, 1);

  // printf("Compile program: DISABLED\n");
  // //analyze_program(prog);
  // 
  // printf("Codegen program: DISABLED\n");
  // // codegen_select(prog, output_file, args.comp_platform);

  // free_program(prog);
  // free(prog);
  // prog = NULL;

  // tokenizer3_free(&t);

  // fclose(output_file);
  return 0;
}
