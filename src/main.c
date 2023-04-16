#include <stdio.h>
#include <stdlib.h>
#include "parser2.h"
#include "tokenizer2.h"

int main() {
  
  // tokenizer_t tokenizer = tokenizer_new(f);
  // printf("%s\n", tokenizer.source.contents); 
  // parse(&tokenizer);
  // tokenizer_free(*tokenizer);

  FILE* f = fopen("code/main.rf", "r");
  if (!f) {
    fprintf(stderr, "Failed to open file\n");
    exit(1);
  }
  tokenizer_t* t = tokenizer_new(f);
  parse(t);

  //while (tokenizer_get_t(t).type != T_EOF) {
  //  printf("%d ", tokenizer_get_t(t).type);
  //  if (tokenizer_get_t(t).type == T_NL) {
  //    printf("\n");
  //  }
  //  tokenizer_advance_t(t);
  //}
  tokenizer_free(t);
  free(t);
  t = NULL;
}
