#include "token_vector.h"

void token_vector_init(token_vector_t* vec) {
  token_vector_init_sz(vec, 10);
}

void token_vector_init_sz(token_vector_t* vec, size_t size) {
  vec->max_size = size;
  vec->current = 0;
  vec->length = 0;
  vec->tokens = malloc(sizeof(token_t) * size);
}

void token_vector_free(token_vector_t* vec) {
  free(vec->tokens);
  vec->tokens = NULL;
}

void token_vector_push(token_vector_t* vec, token_t tok) {
  if (vec->current < 0) {
    return;
  }
  if (vec->current + 1 >= vec->max_size) {
    vec->tokens = realloc(vec->tokens, vec->max_size * 2);
    vec->max_size *= 2;
  }
  vec->tokens[vec->current++] = tok;
  vec->length ++;
}

void token_vector_pop(token_vector_t* vec) {
  vec->current--;
  vec->length--;
}

token_t token_vector_peek(token_vector_t* vec) {
  return vec->tokens[vec->current - 1];
}

token_t token_vector_at(token_vector_t* vec, int index) {
  if (index < 0 || index >= vec->length) {
    return (token_t) {.valid = 0};
  }
  return vec->tokens[index];
}

void token_vector_print(token_vector_t* vec) {
  for (int i = 0; i < vec->length; i++) {
    rfc_token_print(&vec->tokens[i]);
  }
}
