#ifndef PARSER_HPP
#define PARSER_HPP
#include "tokenizer.h"

// ID COLON TYPE
typedef struct {
  char id[10];
  int data_type; 
} var_t;

typedef struct {
  char id[10];
  var_t params[10];   //max 10 params
} func_t;

void parse_var(tokenizer_t*);
void parse_var_list(tokenizer_t*);
void parse_func(tokenizer_t*);
void parse_cond(tokenizer_t*);
void parse_block(tokenizer_t*);

void parse(tokenizer_t*);

#endif
