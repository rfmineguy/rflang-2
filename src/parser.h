#ifndef PARSER_HPP
#define PARSER_HPP
#include "tokenizer.h"
#include <stdbool.h>

typedef struct comp_unit_t  comp_unit_t; 
typedef struct use_t        use_t;
typedef struct if_t         if_t;
typedef struct var_t        var_t;
typedef struct var_dec_t    var_dec_t;
typedef struct assign_t     assign_t;
typedef struct condition_t  condition_t;
typedef struct block_t      block_t;
typedef struct var_list_t   var_list_t; 
typedef struct statement_t  statement_t;
typedef struct expr_t       expr_t;
typedef struct func_t       func_t;
typedef struct expression_t expression_t;

struct comp_unit_t {
  use_t* use_statements;
  int use_statement_count, use_statement_maxsize;

  func_t* functions;
  int function_count, function_maxsize;
};

struct use_t {
  char filename[10];
};

struct expression_t {
};

struct var_dec_t {
  char id[10];
  expression_t expression;
};

struct expr_t {
  
};

struct var_t {
  char id[10];
  int data_type; // this is just the token value for the data types 
  int pointer_count; // number of pointer symbols...
};

struct assign_t {
  var_t var;
  expr_t value;
};

struct condition_t {

};

struct var_list_t {
  var_t vars[10];
  int var_count;
};

struct block_t {
  struct statement_t* stmts;
  int stmt_count;
};

struct if_t {
  condition_t* cond;
  block_t* if_block;
  block_t* else_block;
};

struct statement_t {
  int type;
  union {
    assign_t assignment;
    if_t     iif;
  } data;
};

struct func_t {
  char id[10];
  int has_return_type;
  token_type_t return_type;
  var_list_t var_list;
  block_t* block;
};

int        is_type(token_t);
use_t      parse_use(tokenizer_t*);
var_dec_t  parse_var_dec(tokenizer_t*);
var_t      parse_var(tokenizer_t*);
var_list_t parse_var_list(tokenizer_t*);
func_t     parse_func(tokenizer_t*);
void       parse_cond(tokenizer_t*);
block_t*   parse_block(tokenizer_t*);
if_t       parse_if(tokenizer_t*);
expr_t*    parse_expr(tokenizer_t*);

void       free_comp_unit(comp_unit_t);
void       free_block(block_t*);

// UNSURE
void       free_use(use_t);
void       free_var_dec(var_dec_t);
void       free_var_list(var_list_t);
void       free_func(func_t*);

void       print_tabs(int);
void       show_comp_unit(comp_unit_t, int);
void       show_func(func_t, int);
void       show_var_list(var_list_t, int);
void       show_var(var_t, int);
void       show_use(use_t, int);

void       parse(tokenizer_t*);

#endif
