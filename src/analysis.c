#include "analysis.h"
#include <string.h>

int analyze_program(program_t* program) {
  analyze_context_t ctx = {0};
  ctx.ht = chaining_ht_alloc(10);
  
  printf("... analyze_program\n");

  chaining_ht_show(ctx.ht);
  chaining_ht_free(ctx.ht);
  return 1; 
}

int analyze_use(use_t* use, analyze_context_t ctx) { 
  return 1; 
}

int analyze_asm_block(asm_block_t* asm_block, analyze_context_t ctx) {
  return 1;
}

int analyze_block(block_t* block, analyze_context_t ctx) {
  return 1;
}

int analyze_func(func_t* func, analyze_context_t ctx) {
  return 1;
}

int analyze_func_decl(func_decl_t* func_decl, analyze_context_t ctx) {
  chaining_entry_data_t d = {0};
  strncpy(d.key, func_decl->name, 100);
  d.scope_depth = -1;
  d.scope_number = - 1;
  d.type = 1; // FUNC
  chaining_ht_put(ctx.ht, func_decl->name, d);
  return 1;
}
