#include "parser.h"
#include <stdio.h>
#include <string.h>

#define TOK_SUBSTR t.loc.length, tokenizer->source.contents + t.loc.begin_index - t.loc.length
#define LOC_INFO_FMT "line: %d, col: %d\n"
#define LOC_INFO_ARG t.loc.line, t.loc.column

int is_type(token_t t) {
  int is_type = t.type == T_INT || t.type == T_SHT || t.type == T_CHR || t.type == T_DBL || t.type == T_FLT || t.type == T_BOOL;
  return is_type;
}

use_t parse_use(tokenizer_t* tokenizer) {
  use_t use = {0};
  token_t t;
  tokenizer_next_t(tokenizer);
  tokenizer_consume_spaces(tokenizer);
  if ((t = tokenizer_peek_t(tokenizer)).type != T_ID) {
    printf("Error: No file for use\n");
    exit(1); 
  }
  strncpy(use.filename, tokenizer->source.contents + t.loc.begin_index, t.loc.length); 
  return use;
}

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
  t = tokenizer_peek_t(tokenizer);
  // return type
  if ((t = tokenizer_peek_t(tokenizer)).type == T_ARROW) {
    func.has_return_type = 1;
  }
  if (func.has_return_type) {
    tokenizer_next_t(tokenizer);
    tokenizer_consume_spaces(tokenizer);

    t = tokenizer_peek_t(tokenizer);
    if (!is_type(t)) {
      printf("Error: function missing return type\n");
      exit(1);
    }
    func.return_type = t.type;
    tokenizer_next_t(tokenizer);
    tokenizer_consume_spaces(tokenizer);
  } 

  // '{' ... '}'
  func.block = parse_block(tokenizer);
  return func;
}

block_t* parse_block(tokenizer_t* tokenizer) {
  block_t* block = malloc(sizeof(block_t));
  token_t t;
  if ((t = tokenizer_peek_t(tokenizer)).type != T_LB) {
    printf("Error: block missing '{'\n");
    printf(LOC_INFO_FMT, LOC_INFO_ARG);
    exit(1);
  }
  tokenizer_consume_spaces(tokenizer);
  tokenizer_next_t(tokenizer);

  // parse everything inside the block
  // assign
  token_print(tokenizer_peek_t(tokenizer), tokenizer);
  if (tokenizer_peek_t(tokenizer).type == T_ID) {
    var_t v = parse_var(tokenizer);
    tokenizer_consume_spaces(tokenizer);
    tokenizer_next_t(tokenizer);
    show_var(v, 0);
    if (tokenizer_peek_t(tokenizer).type == T_EQ) {
      assign_t a = {0};
      tokenizer_consume_spaces(tokenizer);
      tokenizer_next_t(tokenizer);
      // expr_t exp = parse_expr(tokenizer);
    }
  }

  if (tokenizer_peek_t(tokenizer).type == T_RETURN) {
    tokenizer_consume_spaces(tokenizer);
    tokenizer_next_t(tokenizer);
    if (tokenizer_peek_t(tokenizer).type == T_ID) {
      tokenizer_consume_spaces(tokenizer);
      tokenizer_next_t(tokenizer);
      if (tokenizer_peek_t(tokenizer).type != T_SEMICOLON) {
        fprintf(stderr, "No semi colon at end of return\n");
        exit(1);
      }
    }
    else {
      fprintf(stderr, "Return has no id\n");
    }
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
var_dec_t parse_var_dec(tokenizer_t* tokenizer) {
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
  if (!is_type(t)) {
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

void free_comp_unit(comp_unit_t cu) {
  free(cu.functions);
  cu.functions = NULL;
  free(cu.use_statements);
  cu.use_statements = NULL;
}

void print_tabs(int count) {
  if (count < 0) return;
  for (int i = 0; i < count; i++)
    printf("\t");
}

void show_comp_unit(comp_unit_t comp, int level) {
  print_tabs(level - 1);
  printf("comp_unit:\n");
  for (int i = 0; i < comp.use_statement_count; i++) {
    show_use(comp.use_statements[i], level+ 1);
  }
  for (int i = 0; i < comp.function_count; i++) {
    show_func(comp.functions[i], level+ 1);
  }
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

void show_use(use_t use, int level) {
  print_tabs(level - 1);
  printf("use:\n");
  print_tabs(level);
  printf("filename: %s\n", use.filename);
}

void parse(tokenizer_t* tokenizer) {
  token_t t;
  comp_unit_t cu = {0};
  while ((t = tokenizer_peek_t(tokenizer)).type != T_EOF) {
    // Parse function
    if (t.type == T_FN) {
      if (cu.function_count + 1 >= cu.function_maxsize) {
        cu.function_maxsize *= 2;
        cu.functions = realloc(cu.functions, sizeof(func_t) * cu.function_maxsize);
      }
      func_t f = parse_func(tokenizer);
      cu.functions[cu.function_count++] = f;
      printf("parsed function\n");
    }
    // Parse use statement
    if (t.type == T_USE) {
      if (cu.use_statement_count + 1 >= cu.use_statement_maxsize) {
        cu.use_statement_maxsize *= 2;
        cu.use_statements = realloc(cu.use_statements, sizeof(use_t) * cu.use_statement_maxsize);
      }
      use_t u = parse_use(tokenizer);
      cu.use_statements[cu.use_statement_count++] = u;
      printf("parsed use\n");
    }
    tokenizer_next_t(tokenizer);
  }
  show_comp_unit(cu, 1);
  free_comp_unit(cu);
}
