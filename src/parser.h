#ifndef PARSER_HPP
#define PARSER_HPP
#include "tokenizer.h"

typedef struct if_t        if_t;
typedef struct var_t       var_t;
typedef struct var_dec_t   var_dec_t;
typedef struct assign_t    assign_t;
typedef struct condition_t condition_t;
typedef struct block_t     block_t;
typedef struct var_list_t  var_list_t; 
typedef struct statement_t statement_t;
typedef struct func_t      func_t;

struct var_dec_t {
  char id[10];
  expression_t expression;
}

struct var_t {
  char id[10];
  int data_type; // this is just the token value for the data types 
  int pointer_count; // number of pointer symbols...
};

struct assign_t {
  var_t var;
  int value;
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
  condition_t cond;
  block_t block;
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
  var_list_t var_list;
  block_t block;
};

/*
 * 
 */
var_dec_t  parse_var_dec(tokenizer_t*);
var_t      parse_var(tokenizer_t*);
var_list_t parse_var_list(tokenizer_t*);
func_t     parse_func(tokenizer_t*);
void       parse_cond(tokenizer_t*);
block_t    parse_block(tokenizer_t*);
if_t       parse_if(tokenizer_t*);

void       print_tabs(int);
void       show_func(func_t, int);
void       show_var_list(var_list_t, int);
void       show_var(var_t, int);

void       parse(tokenizer_t*);

#endif
