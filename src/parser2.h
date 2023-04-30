#ifndef PARSER2_H
#define PARSER2_H
#include "tokenizer2.h"

// typedef enum   expr_type_t  expr_type_t; 

typedef struct program_t    program_t;
typedef struct use_t        use_t;
typedef struct asm_block_t  asm_block_t;
typedef struct block_t      block_t;
typedef struct func_t       func_t;
typedef struct func_decl_t  func_decl_t;
typedef struct func_call_t  func_call_t;
typedef struct var_t        var_t;
typedef struct assign_t     assign_t;
typedef struct return_t     return_t;
typedef struct param_list_t param_list_t;
typedef struct arg_list_t   arg_list_t;
typedef struct expression_t expression_t;
typedef struct statement_t  statement_t;
typedef struct if_t         if_t;
typedef struct condition_t  condition_t;
typedef struct arg_list_t   arg_list_t;

typedef enum {
  X86_32_LINUX_ASM = 142, X86_64_LINUX_ASM, ARM64_ASM  // the order here needs to match the order of the normal token_type_t enum
} asm_type_t;
typedef enum {
  EXPR_NUM = 0, EXPR_STRING, EXPR_COMPOUND
} expr_type_t;

struct program_t {
  use_t**     use_list;
  int         use_list_count, use_list_max;
  func_t**    func_list;
  int         func_list_count, func_list_max;
};
struct use_t {
  char name[30];
};
struct block_t {
  statement_t** statements;
  int statement_count;
};
struct asm_block_t {
  char* asm_source_code_begin;
  char* asm_source_code_end;
  asm_type_t asm_type;
};
struct func_t {
  int          has_return_type;
  token_type_t return_type;
  const char*  return_type_str;
  func_decl_t* decl;
  block_t*     block;
};
struct func_decl_t {
  char name[30];
  param_list_t* params;
};
struct func_call_t {
  char name[30];
  arg_list_t* args;
};
struct var_t {
  char name[30];
  const char* type_name;
  int type;
  int indirection;
};
struct assign_t {
  var_t* var;
  expression_t* expr;
};
struct return_t {
  expression_t* expr;
};
struct param_list_t {
  var_t** params;
  int params_count;
};
struct expression_t {
  expr_type_t type;             // single value (number or id), or composed expression (expr '+' expr)
  union {
    token_type_t operation;
    int i;
    char s[30];
  } value;
  expression_t *left, *right; // ?
};
struct arg_list_t {
  expression_t** args;
  int arg_count;
};
struct statement_t {
  if_t* iff;
  return_t* ret;
  func_call_t* func_call;
  assign_t* assign;
  asm_block_t* asm_block;
};
struct if_t {
  condition_t* condition;
  block_t* block;
};
struct condition_t {
  
};

int           is_operator(token_type_t);
int           get_precedence(token_type_t);
int           is_type_token(token_type_t);

program_t*    parse_program(tokenizer_t*);
use_t*        parse_use(tokenizer_t*);
block_t*      parse_block(tokenizer_t*);
asm_block_t*  parse_asm_block(tokenizer_t*);
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
statement_t*  parse_statement(tokenizer_t*);
if_t*         parse_if(tokenizer_t*);

void          free_program(program_t*);
void          free_use(use_t*);
void          free_block(block_t*);
void          free_asm_block(asm_block_t*);
void          free_var(var_t*);
void          free_func_decl(func_decl_t*);
void          free_func(func_t*);
void          free_func_call(func_call_t*);
void          free_assign(assign_t*);
void          free_return(return_t*);
void          free_param_list(param_list_t*);
void          free_arg_list(arg_list_t*);
void          free_expression(expression_t*);
void          free_statement(statement_t*);

void          tabs(int);

void          show_program(program_t*, int);
void          show_use(use_t*, int);
void          show_block(block_t*, int);
void          show_asm_block(asm_block_t*, int);
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
void          show_statement(statement_t*, int);

program_t*    parse(tokenizer_t*);

#endif
