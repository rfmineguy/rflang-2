#ifndef PARSER3_H
#define PARSER3_H
#include "parse_tree.h"
#include "../tokenizer3.h"

int           is_asm_type_specifier(token_type_t);
int           is_operator(token_type_t);
int           get_precedence(token_type_t);
int           is_type_token(token_type_t);

program_t*    parse(tokenizer3_t*);

program_t*    parse_program(tokenizer3_t*);
use_t*        parse_use(tokenizer3_t*);
block_t*      parse_block(tokenizer3_t*);
asm_block_t*  parse_asm_block(tokenizer3_t*);
string_lit_t* parse_string_lit(tokenizer3_t*);
var_t*        parse_var(tokenizer3_t*);
func_decl_t*  parse_func_decl(tokenizer3_t*);
func_t*       parse_func(tokenizer3_t*);
func_call_t*  parse_func_call(tokenizer3_t*);
assign_t*     parse_assign(tokenizer3_t*);
return_t*     parse_return(tokenizer3_t*);
param_list_t* parse_param_list(tokenizer3_t*);
arg_list_t*   parse_arg_list(tokenizer3_t*);
statement_t*  parse_statement(tokenizer3_t*);
if_t*         parse_if(tokenizer3_t*);

expression_t* parse_expression_postfix(token_t* postfix, int postfix_len); 
expression_t* parse_expression(tokenizer3_t*);
void          get_postfix_rep(tokenizer3_t*, token_t*, int*);

#endif
