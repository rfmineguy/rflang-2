#ifndef ANALYSIS_H
#define ANALYSIS_H
#include "parsers/parser3.h"
#include "ht/chain_ht_str_vardat.h"
#include "ht/chain_ht_str_module.h"

/* NOTES:
 *  - analysis will include the following features
 *       1. type checking
 *       2. symbol resolution
 */

typedef struct {
  const char* used_modules[10];
  int         used_modules_count;

  chaining_ht_str_var_t var_ht;
  chaining_ht_str_module_t* parsed_module_ht;
  int scope_number;
  int scope_depth;
} analyze_context_t;

typedef struct {
  int is_const_expr;
} expression_info_t;

int analyze_module(module_t*, chaining_ht_str_module_t*);
int analyze_use(use_t*, analyze_context_t*);
int analyze_block(block_t*, analyze_context_t*);
int analyze_func(func_t*, analyze_context_t*);
int analyze_func_decl(func_decl_t*, analyze_context_t*);
int analyze_param_list(param_list_t*, analyze_context_t*);
int analyze_var(var_t*, analyze_context_t*);
int analyze_string_lit(string_lit_t*, analyze_context_t*);
int analyze_statement(statement_t*, analyze_context_t*);
int analyze_iff(if_t*, analyze_context_t*);
int analyze_return(return_t*, analyze_context_t*);
int analyze_func_call(expression_t*, analyze_context_t*, int);
int analyze_assign(assign_t*, analyze_context_t*);
int analyze_asm_block(asm_block_t*, analyze_context_t*);
int analyze_while(while_t*, analyze_context_t*);
int analyze_expression(expression_t*, analyze_context_t*, int);

#endif
