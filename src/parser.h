#ifndef PARSER_HPP
#define PARSER_HPP
#include "tokenizer.h"

// ID COLON TYPE
typedef struct {
  char id[10];
  int data_type; // this is just the token value for the data types 
} var_t;

typedef struct {
  var_t vars[10];
  int var_count;
} var_list_t;

typedef struct {
  
} block_t;

typedef struct {
  char id[10];
  var_list_t var_list;
  block_t block;
} func_t;

var_t      parse_var(tokenizer_t*);
var_list_t parse_var_list(tokenizer_t*);
func_t     parse_func(tokenizer_t*);
void       parse_cond(tokenizer_t*);
block_t    parse_block(tokenizer_t*);

void       print_tabs(int);
void       show_func(func_t, int);
void       show_var_list(var_list_t, int);
void       show_var(var_t, int);

void parse(tokenizer_t*);

#endif
