#ifndef PARSER3_H
#define PARSER3_H
#include "parse_tree.h"
#include "../error/errors2.h"
#include "../tokenizer3.h"

int           is_asm_type_specifier(token_type_t);
int           is_operator(token_type_t);
int           get_precedence(token_type_t);
int           is_type_token(token_type_t);

module_t*     parse(tokenizer3_t*);

module_t*     parse_module(tokenizer3_t*);
use_t*        parse_use(tokenizer3_t*, error_context_t*);
block_t*      parse_block(tokenizer3_t*, error_context_t*);
asm_block_t*  parse_asm_block(tokenizer3_t*, error_context_t*);
string_lit_t* parse_string_lit(tokenizer3_t*, error_context_t*);
var_t*        parse_var(tokenizer3_t*, error_context_t*);
func_decl_t*  parse_func_decl(tokenizer3_t*, error_context_t*);
func_t*       parse_func(tokenizer3_t*, error_context_t*);
func_call_t*  parse_func_call(tokenizer3_t*, error_context_t*);
assign_t*     parse_assign(tokenizer3_t*, error_context_t*);
return_t*     parse_return(tokenizer3_t*, error_context_t*);
param_list_t* parse_param_list(tokenizer3_t*, error_context_t*);
arg_list_t*   parse_arg_list(tokenizer3_t*, error_context_t*);
statement_t*  parse_statement(tokenizer3_t*, error_context_t*);
if_t*         parse_if(tokenizer3_t*, error_context_t*);
while_t*      parse_while(tokenizer3_t*, error_context_t*);

expression_t* parse_expression_postfix(token_t* postfix, int postfix_len, error_context_t*); 
expression_t* parse_expression(tokenizer3_t*, error_context_t*);
void          get_postfix_rep(tokenizer3_t*, token_t*, int*, error_context_t*);

#endif
