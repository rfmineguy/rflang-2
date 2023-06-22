#include "compile.h"
#include "codegen/codegen.h"
#include "analysis.h"
#include "file_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       //getcwd

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
  if (!file_util_check_input_modules_exist(&args, &failed_file)) {
    fprintf(stderr, "Module ['%s'] doesn't exist in stdlib or local\n", failed_file);
    return 1;
  }

  // Begin parsing source files
  chaining_ht_str_program_t cached_parses = chaining_ht_str_program_alloc(10);
  for (int i = 0; i < args.input_modules_count; i++) {
    parse_file_rec(args.input_modules[i], &args, cached_parses);
  }

  // Start analysis of the modules
  chaining_ht_str_program_show(cached_parses, MODULE_PARSE_TREE);
  chaining_ht_str_program_free(cached_parses);

  return 2; //NOTE: Neutral
}

void parse_file_rec(const char* module, args2_t* args, chaining_ht_str_program_t ht) {
  if (chaining_ht_str_program_contains(ht, (char*) module)) {
    fprintf(stderr, "Module '%s' has already been parsed\n", module);
    return;
  }
  char full_path_local[PATH_MAX], full_path_stdlib[PATH_MAX];
  int has_local = file_util_local_module_exists(module, full_path_local, PATH_MAX);
  int has_std   = file_util_stdlib_module_exists(module, full_path_stdlib, PATH_MAX);
  if (!has_local && !has_std) {
    fprintf(stderr, "Module [%s] doesn't exist locally or in the stdlib\n", module);
    return;
  }
  
  printf("Parsing [%s]\n", module);
  tokenizer3_t t = tokenizer3_new(has_local ? full_path_local : full_path_stdlib);
  t.module_name = module;
  program_t* prog = parse(&t);
  chaining_ht_str_program_put(ht, (char*) module, prog);

  tokenizer3_free(&t);

  for (int i = 0; i < prog->use_list_count; i++) {
    parse_file_rec(prog->use_list[i]->name, args, ht);
  }
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
