#ifndef PARSER2_H
#define PARSER2_H
#include "tokenizer2.h"
#include "parser_util.h"    //contains all the basic parser constructs (i.e. struct program_t)

int           is_operator(token_type_t);
int           get_precedence(token_type_t);
int           is_type_token(token_type_t);

program_t*    parse(tokenizer_t*);

program_t*    parse_program(tokenizer_t*);
use_t*        parse_use(tokenizer_t*);
block_t*      parse_block(tokenizer_t*);
asm_block_t*  parse_asm_block(tokenizer_t*);
string_lit_t* parse_string_lit(tokenizer_t*);
var_t*        parse_var(tokenizer_t*);
func_decl_t*  parse_func_decl(tokenizer_t*);
func_t*       parse_func(tokenizer_t*);
func_call_t*  parse_func_call(tokenizer_t*);
assign_t*     parse_assign(tokenizer_t*);
return_t*     parse_return(tokenizer_t*);
param_list_t* parse_param_list(tokenizer_t*);
arg_list_t*   parse_arg_list(tokenizer_t*);

expression_t* parse_expression_ex(tokenizer_t*, expression_t*, token_t*, int, int);
expression_t* parse_expression(tokenizer_t*);
expression_t* parse_expression_v2(token_t*, int);
void          get_postfix_rep(tokenizer_t*, token_t*, int*);
void          get_postfix_rep_v2(tokenizer_t*, token_t*, int*);

statement_t*  parse_statement(tokenizer_t*);
if_t*         parse_if(tokenizer_t*);

void          free_program(program_t*);
void          free_use(use_t*);
void          free_block(block_t*);
void          free_asm_block(asm_block_t*);
void          free_string_lit(string_lit_t*);
void          free_var(var_t*);
void          free_func_decl(func_decl_t*);
void          free_func(func_t*);
void          free_func_call(func_call_t*);
void          free_assign(assign_t*);
void          free_return(return_t*);
void          free_param_list(param_list_t*);
void          free_arg_list(arg_list_t*);
void          free_expression(expression_t*);
void          free_iff(if_t*);
void          free_while(while_t*);
void          free_statement(statement_t*);

void          tabs(int);

void          show_program(program_t*, int);
void          show_use(use_t*, int);
void          show_block(block_t*, int);
void          show_asm_block(asm_block_t*, int);
void          show_string_lit(string_lit_t*, int);
void          show_var(var_t*, int);
void          show_func_decl(func_decl_t*, int);
void          show_func(func_t*, int);
void          show_func_call(func_call_t*, int);
void          show_assign(assign_t*, int);
void          show_return(return_t*, int);
void          show_param_list(param_list_t*, int);
void          show_arg_list(arg_list_t*, int);
void          show_expression(expression_t*, int);
void          show_iff(if_t*, int);
void          show_while(while_t*, int);
void          show_statement(statement_t*, int);

#endif
