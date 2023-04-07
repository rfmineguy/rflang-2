#include "parser.h"
#include <stdio.h>
#include <string.h>

#define TOK_SUBSTR t.loc.length, tokenizer->source.contents + t.loc.begin_index - t.loc.length
#define LOC_INFO_FMT "line: %d, col: %d"
#define LOC_INFO_ARG t.loc.line, t.loc.column

func_t parse_func(tokenizer_t* tokenizer) {
  func_t func = {0};
  if (tokenizer_peek_t(tokenizer).type != T_FN) {
    printf("Error: not function\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);

  token_t t;
  if ((t = tokenizer_peek_t(tokenizer)).type != T_ID) {
    printf("Error: function has no id\n");
    exit(1);
  }
  strncpy(func.id, tokenizer->source.contents + t.loc.begin_index, t.loc.length);
  
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  
  // includes '(' and ')'
  func.var_list = parse_var_list(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  
  // '{' ... '}'
  func.block = parse_block(tokenizer);
  return func;
}

block_t parse_block(tokenizer_t* tokenizer) {
  block_t block;
  if (tokenizer_peek_t(tokenizer).type != T_LB) {
    printf("Error: block missing '{'\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);

  // parse everything inside the block
  var_t v = parse_var(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  if (tokenizer_peek_t(tokenizer).type == T_EQ) {
    //asignment
    var_dec_t vd = parse_var_dec(tokenizer);
  }
  
  if (tokenizer_peek_t(tokenizer).type != T_RB) {
    printf("Error: block missing '}'\n"); 
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  return block;
}

var_list_t parse_var_list(tokenizer_t* tokenizer) {
  var_list_t var_list = {0};
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
    var_t v = parse_var(tokenizer);
    var_list.vars[var_list.var_count++] = v;
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
  return var_list;
}

// [var, EQ, expr]
var_dec_t parse_var_dec(tokenizer* tokenizer) {
  var_dec_t var_dec = {0};
  token_t t;
  if ((t = tokenizer_peek_t(tokenizer)).type != T_ID) {
    printf(LOC_INFO_FMT, LOC_INFO_ARG);
    printf("Error: var declaration has no id\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  if ((t = tokenizer_peek_t(tokenizer)).type != T_EQ) {
    printf(LOC_INFO_FMT, LOC_INFO_ARG);
    printf("Error: var declaration has no assignment operator\n");
    exit(1);
  }
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  // NOTE: Next should be an expression construct
  return var_dec;
}

var_t parse_var(tokenizer_t* tokenizer) {
  var_t var = {0};
  token_t t;
  if ((t = tokenizer_peek_t(tokenizer)).type != T_ID) {
    printf(LOC_INFO_FMT, LOC_INFO_ARG);
    printf("Error: var has no id\n");
    exit(1);
  }
  strncpy(var.id, tokenizer->source.contents + t.loc.begin_index, t.loc.length);
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
  var.data_type = t.type;
  tokenizer_next_t(tokenizer);
  t = tokenizer_peek_t(tokenizer);
  while (t.type == T_MUL) {
    var.pointer_count++;
    tokenizer_next_t(tokenizer);
    t = tokenizer_peek_t(tokenizer);
  }
  return var;
}

void parse_cond(tokenizer_t* tokenizer) {

}

void print_tabs(int count) {
  if (count < 0) return;
  for (int i = 0; i < count; i++)
    printf("\t");
}

void show_func(func_t func, int level) {
  print_tabs(level - 1);
  printf("Func: \n");
  print_tabs(level);
  printf("id: %s\n", func.id);
  print_tabs(level);
  show_var_list(func.var_list, level + 1);
}

void show_var(var_t var, int level) {
  print_tabs(level - 1);
  printf("var: %s, %d, {%s, count=%d}\n", var.id, var.data_type, var.pointer_count > 0 ? "pointer":"value", var.pointer_count);
}

void show_var_list(var_list_t list, int level) {
  print_tabs(level - 1);
  printf("var_list:\n");
  for (int i = 0; i < list.var_count; i++) {
    show_var(list.vars[i], level + 1);
  }
}

void parse(tokenizer_t* tokenizer) {
  token_t t;
  while ((t = tokenizer_peek_t(tokenizer)).type != T_EOF) {
    // token_print(t, tokenizer);
    if (t.type == T_FN) {
      func_t f = parse_func(tokenizer);
      printf("parsed function\n");
      show_func(f, 0);
    }
    tokenizer_next_t(tokenizer);
  }
}

