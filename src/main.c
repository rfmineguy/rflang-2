#include <stdio.h>
#include "parser.h"

int main() {
  tokenizer_t tokenizer = tokenizer_new("code/main.rf");
  printf("%s\n", tokenizer.source.contents); 
  parse(&tokenizer);
}
