#ifndef TOKENIZER_2_H
#define TOKENIZER_2_H
#include "token.h"
#include <stdio.h>

typedef struct {
  char* source_str;
  char* cursor;
  int source_length;
  int col, line;
  token_t current;
} tokenizer_t;

tokenizer_t* tokenizer_new(FILE*);
void tokenizer_free(tokenizer_t*);

token_type_t tokenizer_ch(tokenizer_t*, char);
token_type_t tokenizer_str(tokenizer_t*, const char*);
token_type_t tokenizer_integer(tokenizer_t*);
token_type_t tokenizer_id(tokenizer_t*);

int tokenizer_expect_t(tokenizer_t*, token_type_t);
void tokenizer_advance_t_internal(tokenizer_t*);
void tokenizer_advance_t(tokenizer_t*);
token_t tokenizer_get_t(tokenizer_t*);

void tokenizer_show_next_t(tokenizer_t*);

void token_print(token_t, tokenizer_t*);

#endif
