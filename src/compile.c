#include "compile.h"
#include "codegen/codegen.h"
#include "parsers/parser3.h"
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

int compile_all(args2_t args) {
  /* MOCK CODE
  // make a hashtable for storing already parsed files
  hashmap<string, program> cached_parses = ht_new(...);

  // parse every file supplied to the compiler
  for (int i = 0; i < args.input_files_count; i++) {
    tokenizer3_t t = tokenizer3_new(args.input_files[i]);   // make tokenizer for the input file
    program_t* p = parse(&t);                               // generate ast for the tokenizer
    ht_put(cached_parses, args.input_files[i], p);          // cache the ast for later analysis
  }

  for (int i = 0; i < cached_parses.size; i++) {
    analyze(p, cached_parses);                              // analyze the cached parse, with awareness
  }
  */

  /* ISSUES WITH THIS ^^
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
   */

  return 1;
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
