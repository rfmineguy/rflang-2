#include "parser.h"
#include <stdio.h>

#define TOK_SUBSTR t.loc.length, tokenizer->source.contents + t.loc.begin_index - t.loc.length
#define LOC_INFO_FMT "line: %d, col: %d"
#define LOC_INFO_ARG t.loc.line, t.loc.column

// Parse func like [fn, id, var_list] 
void parse_func(tokenizer_t* tokenizer) {
  if (tokenizer_peek_t(tokenizer).type != T_FN) {
    printf("Error: not function\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);

  if (tokenizer_peek_t(tokenizer).type != T_ID) {
    printf("Error: function has no id\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  
  // includes '(' and ')'
  parse_var_list(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  
  // '{' ... '}'
  parse_block(tokenizer);
}

// Parse block like ['{', ..., '}']
void parse_block(tokenizer_t* tokenizer) {
  if (tokenizer_peek_t(tokenizer).type != T_LB) {
    printf("Error: block missing '{'\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);

  if (tokenizer_peek_t(tokenizer).type != T_RB) {
    printf("Error: block missing '}'\n"); 
    exit(1);
  }
  tokenizer_next_t(tokenizer);
}

// Parse var_list like ['(', var, ',', ..., ')']
void parse_var_list(tokenizer_t* tokenizer) {
  // Expect a '('
  if (tokenizer_peek_t(tokenizer).type != T_LP) {
    printf("Error: param list has no (\n");
    exit(1);    
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  
  // Expect [var, ',']
  while (1) {
    tokenizer_consume_spaces(tokenizer);
    parse_var(tokenizer);
    token_t t = tokenizer_peek_t(tokenizer);
    if (t.type == T_RP) {
      break;
    }
    if (t.type != T_COMMA) {
      printf("Error: missing comma\n"); 
      exit(1);
    }
    else {
      tokenizer_next_t(tokenizer);
    }
  }

  // Expect [')']
  if (tokenizer_peek_t(tokenizer).type != T_RP) {
    printf("Error: param list has no )\n"); 
    exit(1);
  }
  tokenizer_next_t(tokenizer);
}

// Parse var like [id, ':', type]
void parse_var(tokenizer_t* tokenizer) {
  token_t t;
  if ((t = tokenizer_peek_t(tokenizer)).type != T_ID) {
    printf(LOC_INFO_FMT, LOC_INFO_ARG);
    printf("Error: var has no id\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  if (tokenizer_peek_t(tokenizer).type != T_COLON) {
    printf("Error: var has no colon\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  t = tokenizer_peek_t(tokenizer);
  int is_type = t.type == T_INT || t.type == T_SHT || t.type == T_CHR || t.type == T_DBL || t.type == T_FLT || t.type == T_BOOL;
  if (!is_type) {
    printf("Error: var has no type\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
}

void parse_cond(tokenizer_t* tokenizer) {

}

void parse(tokenizer_t* tokenizer) {
  token_t t;
  while ((t = tokenizer_peek_t(tokenizer)).type != T_EOF) {
    // token_print(t, tokenizer);
    if (t.type == T_FN) {
      parse_func(tokenizer);
      printf("parsed function\n");
    }
    tokenizer_next_t(tokenizer);
  }
}
