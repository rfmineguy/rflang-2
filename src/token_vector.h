#ifndef TOKEN_VECTOR_H
#define TOKEN_VECTOR_H
#include "token.h"
#include <stdlib.h>

typedef struct {
  int max_size, length;
  int current;
  token_t* tokens;
} token_vector_t;

void token_vector_init(token_vector_t*);
void token_vector_init_sz(token_vector_t*, size_t);
void token_vector_free(token_vector_t*);
void token_vector_push(token_vector_t*, token_t);
void token_vector_pop(token_vector_t*);
token_t token_vector_peek(token_vector_t*);
token_t token_vector_at(token_vector_t*, int);

void token_vector_print(token_vector_t*);

#endif 