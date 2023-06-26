#include "compile.h"
#include "codegen/codegen.h"
#include "analysis.h"
#include "file_util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>       //getcwd

int compile_all(args2_t args) {
  // Esure that all the files supplied actually do exist before moving on to parsing
  printf("Compiling all\n");
  const char* failed_file = NULL;
  if (!file_util_check_input_modules_exist(&args, &failed_file)) {
    fprintf(stderr, "Module ['%s'] doesn't exist in stdlib or local\n", failed_file);
    return 1;
  }

  chaining_ht_str_module_t cached_parses = chaining_ht_str_module_alloc(10);
  chaining_ht_str_symbol_t symbols = chaining_ht_str_symbol_alloc(10);

  // Parse the files needed to be parsed
  for (int i = 0; i < args.input_modules_count; i++) {
    parse_file_rec(args.input_modules[i], &args, cached_parses);
  }
  
  // Start analysis of the modules
  for (int i = 0; i < args.input_modules_count; i++) {
    analyze_module_rec(args.input_modules[i], &args, cached_parses, symbols);
  }

  // Display the hash tables
  chaining_ht_str_symbol_show(symbols);
  chaining_ht_str_module_show(cached_parses, MODULE_NAME);

  // Free the memory for the various hash tables we have
  chaining_ht_str_module_free(cached_parses);
  chaining_ht_str_symbol_free(symbols);


  fprintf(stderr, "====================================================\n");
  fprintf(stderr, "NOTE: rfc leaks a little bit of memory at the moment\n"); 
  fprintf(stderr, "====================================================\n");
  return 2; //NOTE: Neutral
}

void parse_file_rec(const char* module_name, args2_t* args, chaining_ht_str_module_t ht) {
  if (chaining_ht_str_module_contains(ht, (char*) module_name)) {
    fprintf(stderr, "Module '%s' has already been parsed\n", module_name);
    return;
  }
  char full_path_local[PATH_MAX], full_path_stdlib[PATH_MAX];
  int has_local = file_util_local_module_exists(module_name, full_path_local, PATH_MAX);
  int has_std   = file_util_stdlib_module_exists(module_name, full_path_stdlib, PATH_MAX);
  if (!has_local && !has_std) {
    fprintf(stderr, "Module [%s] doesn't exist locally or in the stdlib\n", module_name);
    return;
  }
  
  printf("Parsing [%s]\n", module_name);
  tokenizer3_t t = tokenizer3_new(has_local ? full_path_local : full_path_stdlib);
  t.module_name = module_name;
  module_t* prog = parse(&t);
  chaining_ht_str_module_put(ht, (char*) module_name, prog);

  tokenizer3_free(&t);

  for (int i = 0; i < prog->use_list_count; i++) {
    parse_file_rec(prog->use_list[i]->name, args, ht);
  }
}

void analyze_module_rec(const char* module_name, args2_t* args, chaining_ht_str_module_t mod_ht, chaining_ht_str_symbol_t sym_ht) {
  if (!chaining_ht_str_module_contains(mod_ht, (char*) module_name)) {
    fprintf(stderr, "Analyze: Module [%s] doesn't exist\n", module_name);
    return;
  }

  // Make sure the dependent parse trees have been analyzed prior to analyzing the current one
  module_t* module = chaining_ht_str_module_find(mod_ht, (char*) module_name).p;
  for (int i = 0; i < module->use_list_count; i++) {
    analyze_module_rec(module->use_list[i]->name, args, mod_ht, sym_ht);
  }

  // By the time we get here, all dependent parse trees should have been analyzed and
  //    populated their symbols into the hash table
  analyze_module_temp(module, sym_ht);
}

void analyze_module_temp(module_t* module, chaining_ht_str_symbol_t sym_ht) {
  printf("   Analyzing module '%s'\n", module->name);
  // analyze functions should traverse the module tree, and populate the symbol hash table with it
  for (int i = 0; i < module->func_list_count; i++) {
    func_t* f = module->func_list[i];
    func_decl_t* decl = f->decl;
    if (chaining_ht_str_symbol_contains(sym_ht, decl->name)) {
      fprintf(stderr, "ERROR: Func '%s' already defined\n", decl->name);
      continue; //go to next func_decl
    }
    chaining_ht_str_symbol_put(sym_ht, decl->name, (entry_symbol){});
  }
}
