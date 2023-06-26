#ifndef ANALYSIS2_H
#define ANALYSIS2_H
#include "parsers/parse_tree.h"
#include "ht/chain_ht_str_symbol.h"

void analyze2_module    (module_t*,     chaining_ht_str_symbol_t);
void analyze2_use       (use_t*,        chaining_ht_str_symbol_t);
void analyze2_block     (block_t*,      chaining_ht_str_symbol_t);
void analyze2_asm_block (asm_block_t*,  chaining_ht_str_symbol_t);
void analyze2_func      (func_t*,       chaining_ht_str_symbol_t);
void analyze2_func_decl (func_decl_t*,  chaining_ht_str_symbol_t);
void analyze2_func_call (func_call_t*,  chaining_ht_str_symbol_t);
void analyze2_var       (var_t*,        chaining_ht_str_symbol_t);
void analyze2_string_lit(string_lit_t*, chaining_ht_str_symbol_t);
void analyze2_assign    (assign_t*,     chaining_ht_str_symbol_t);
void analyze2_return    (return_t*,     chaining_ht_str_symbol_t);
void analyze2_param_list(param_list_t*, chaining_ht_str_symbol_t);

#endif
