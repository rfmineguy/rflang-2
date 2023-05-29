#include "parser3.h"
#include "../tokenizer3.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int is_asm_type_specifier(token_type_t type) {
  return type == T_X86_32_LINUX || type == T_X86_64_LINUX || type == T_ARM64;
}

int is_operator(token_type_t type) {
  return type == T_PLUS || type == T_MINUS || type == T_MUL || type == T_DIV || type == T_MOD;
}

int is_type_token(token_type_t type) {
  return type == T_INT || type == T_SHT || type == T_CHR || type == T_DBL || type == T_FLT || type == T_BOOL;
}

int get_precedence(token_type_t type) {
  switch (type) {
    case T_PLUS:
    case T_MINUS:
      return 1;
    case T_MUL:
    case T_DIV:
      return 2;
    default:
      return -1;
  }
}

program_t* parse(tokenizer3_t* t) {
  return parse_program(t);
}

program_t* parse_program(tokenizer3_t* t) {
  const int FUNC_DEFAULT_MAX = 5;
  const int USE_DEFAULT_MAX  = 5;
  program_t* p = calloc(1, sizeof(program_t));
  p->func_list_max = FUNC_DEFAULT_MAX;
  p->use_list_max = USE_DEFAULT_MAX;
  p->func_list = calloc(p->func_list_max, sizeof(func_t));
  p->use_list = calloc(p->use_list_max, sizeof(use_t));

  tokenizer3_advance(t);

  // Expect use statements second (use statements are only allowed at the start)
  tokenizer3_show_token_offset(t, 2);
  while (tokenizer3_get(t, 2).type == T_USE) {
    use_t* use = parse_use(t);
    p->use_list[p->use_list_count++] = use;
  }
  if (p->use_list_count != 0)
    tokenizer3_advance(t);
  
  // Expect functions
  while (tokenizer3_get(t, 2).type == T_FN) {
    printf("Func\n");
    tokenizer3_advance(t);
    func_t* func = parse_func(t);
    p->func_list[p->func_list_count++] = func;
  }

  return p;
}

use_t* parse_use(tokenizer3_t* t) {
  printf("Parsing use\n");
  use_t* u = calloc(1, sizeof(use_t));
  tokenizer3_advance(t);
  if (!tokenizer3_expect_offset(t, 2, T_ID)) {
    fprintf(stderr, "Expected ID, got something else\n"); 
    exit(1);
  }
  strncpy(u->name, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);
  return u;
}

block_t* parse_block(tokenizer3_t* t) {
  return NULL;
}

asm_block_t* parse_asm_block(tokenizer3_t* t) {
  return NULL;
}

string_lit_t* parse_string_lit(tokenizer3_t* t) {
  return NULL;
}

var_t* parse_var(tokenizer3_t* t) {
  return NULL;
}

func_decl_t* parse_func_decl(tokenizer3_t* t) {
  return NULL;
}

func_t* parse_func(tokenizer3_t* t) {
  return NULL;
}

func_call_t* parse_func_call(tokenizer3_t* t) {
  return NULL;
}

assign_t* parse_assign(tokenizer3_t* t) {
  return NULL;
}

return_t* parse_return(tokenizer3_t* t) {
  return NULL;
}

param_list_t* parse_param_list(tokenizer3_t* t) {
  return NULL;
}

arg_list_t* parse_arg_list(tokenizer3_t* t) {
  return NULL;
}

statement_t* parse_statement(tokenizer3_t* t) {
  return NULL;
}

if_t* parse_if(tokenizer3_t* t) {
  return NULL;
}

expression_t* parse_expression_postfix(token_t* postfix, int postfix_len) {
  expression_t* exprs[postfix_len];
  int top = -1;
  for (int i = 0; i < postfix_len; i++) {
    expression_t* e = calloc(1, sizeof(expression_t));
    if (postfix[i].type == T_ID) {
      e->type = EXPR_STRING;
      strncpy(e->value.s, postfix[i].value.s, 30);
      exprs[++top] = e;
    }
    else if (postfix[i].type == T_NUM) {
      e->type = EXPR_NUM;
      e->value.i = postfix[i].value.i;
      exprs[++top] = e;
    }
    else {
      expression_t* l = exprs[top - 1];
      expression_t* r = exprs[top];
      
      top -= 2;
      e->type = EXPR_COMPOUND;
      e->value.operation = postfix[i].type;
      e->left = l;
      e->right = r;
      exprs[++top] = e;
    }
  }
  return exprs[0];
}

expression_t* parse_expression(tokenizer3_t* t) {
  // convert entire expression to postfix
  token_t postfix[100] = {0};
  int postfix_len = 0;
  
  get_postfix_rep(t, postfix, &postfix_len);
  printf("postfix_len: %d\n", postfix_len);
  for (int i = 0; i < postfix_len; i++) {
    printf("%d, ", i);
    tokenizer3_token_print(postfix[i], t);
  }
  return parse_expression_postfix(postfix, postfix_len);
}

void get_postfix_rep(tokenizer3_t* t, token_t* postfix_out, int* postfix_length) {
  int j = 0;
  token_t stack[100] = {0};
  int top = -1;
  
  while (1) {
    int is_id_or_num      = tokenizer3_expect_offset(t, 2, T_ID) || tokenizer3_expect_offset(t, 2, T_NUM);
    int is_math_operator  = tokenizer3_expect_offset(t, 2, T_PLUS) || tokenizer3_expect_offset(t, 2, T_MINUS) || tokenizer3_expect_offset(t, 2, T_MUL) || tokenizer3_expect_offset(t, 2, T_DIV) || tokenizer3_expect_offset(t, 2, T_MOD);
    int is_comp_operator  = 
      tokenizer3_expect_offset(t, 2, T_GT) || tokenizer3_expect_offset(t, 2, T_GTEQ) || tokenizer3_expect_offset(t, 2, T_LT) || tokenizer3_expect_offset(t, 2, T_LTEQ) || tokenizer3_expect_offset(t, 2, T_NEQ) || tokenizer3_expect_offset(t, 2, T_DEQ);
    int is_logic_operator = tokenizer3_expect_offset(t, 2, T_DAND) || tokenizer3_expect_offset(t, 2, T_DOR);
    int is_lparen         = tokenizer3_expect_offset(t, 2, T_LP);
    int is_rparen         = tokenizer3_expect_offset(t, 2, T_RP);

    if (is_id_or_num) {
      postfix_out[j++] = tokenizer3_get(t, 2);
    }
    else if (is_math_operator) {
      while (top > -1 && get_precedence(stack[top].type) >= get_precedence(tokenizer3_get(t, 2).type)) {
        postfix_out[j++] = stack[top--];
      }
      stack[++top] = tokenizer3_get(t, 2);
    }
    else if (is_comp_operator) {
      while (top > -1 && (stack[top].type != T_DAND && stack[top].type != T_DOR)) {
        postfix_out[j++] = stack[top--];
      }
      stack[++top] = tokenizer3_get(t, 2);
    }
    else if (is_logic_operator) {
      while (top > -1) {
        postfix_out[j++] = stack[top--];
      }
      stack[++top] = tokenizer3_get(t, 2);
    }
    else if (is_lparen) {
      stack[++top] = tokenizer3_get(t, 2);
    }
    else if (is_rparen) {
      while (stack[top].type != T_LP) {
        postfix_out[j++] = stack[top--];
      }
      if (top > -1 && stack[top].type != T_LP) {
        // error
      }
      else {
        top--;
      }
    }
    else {
      break;
    }
    tokenizer3_advance(t);
  }

  while (top > -1) {
    if (stack[top].type == T_LP) {
      // error
    }
    postfix_out[j++] = stack[top--];
  }
  *postfix_length = j;
}
