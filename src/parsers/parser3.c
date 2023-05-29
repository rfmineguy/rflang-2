#include "parser3.h"
#include "../tokenizer3.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

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
  while (tokenizer3_get(t, 2).type == T_USE) {
    use_t* use = parse_use(t);
    p->use_list[p->use_list_count++] = use;
  }

  // Expect functions
  while (tokenizer3_get(t, 2).type == T_FN) {
    printf("Func\n");
    // tokenizer3_advance(t);
    func_t* func = parse_func(t);
    p->func_list[p->func_list_count++] = func;
  }

  return p;
}

use_t* parse_use(tokenizer3_t* t) {
  use_t* u = calloc(1, sizeof(use_t));
  tokenizer3_advance(t);
  if (!tokenizer3_expect_offset(t, 2, T_ID)) {
    fprintf(stderr, "Expected ID, got something else\n"); 
    exit(1);
  }
  strncpy(u->name, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);
  printf("=> Parsed use\n");
  return u;
}

block_t* parse_block(tokenizer3_t* t) {
  const int MAX_STATEMENTS = 10;
  block_t* b = calloc(1, sizeof(block_t));
  if (!tokenizer3_expect_offset(t, 2, T_LB)) {
     fprintf(stderr, "<%s>: %d, Expected LB, got something else\n", __FUNCTION__, __LINE__);
     exit(1);
  }
  b->statements = calloc(MAX_STATEMENTS, sizeof(statement_t)); //NOTE: This is VERY inflexible. Maybe use a linked list here?
  tokenizer3_advance(t);
  while (tokenizer3_get(t, 2).type != T_RB) {
    statement_t* s = parse_statement(t);
    if (s) {
       b->statements[b->statement_count++] = s;
    }
    tokenizer3_advance(t);
  }
  tokenizer3_advance(t);    //consume last '}'
  printf("=> Parsed block\n");
  return b;
}

asm_block_t* parse_asm_block(tokenizer3_t* t) {
  asm_block_t* block = calloc(1, sizeof(asm_block_t));
  tokenizer3_advance(t);
  tokenizer3_show_token_offset(t, 2);
  if (!is_asm_type_specifier(tokenizer3_get(t, 2).type)) {
    fprintf(stderr, "<%s>: %d, Expected asm type specifier, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  block->asm_type = tokenizer3_get(t, 2).type;
  tokenizer3_advance(t);
  if (!tokenizer3_expect_offset(t, 2, T_LB)) {
    fprintf(stderr, "<%s>: %d, Expected '{', got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  block->asm_source_code_begin = tokenizer3_get(t, 2).loc.begin_index + t->source_code;
  while (!tokenizer3_expect_offset(t, 2, T_RB)) {
    tokenizer3_advance(t);
  }
  block->asm_source_code_end = tokenizer3_get(t, 2).loc.begin_index + t->source_code;
  tokenizer3_show_token_offset(t, 2);
  return block;
}

string_lit_t* parse_string_lit(tokenizer3_t* t) {
  string_lit_t* s = calloc(1, sizeof(string_lit_t));
  if (!tokenizer3_expect_offset(t, 2, T_DQT)) {
    fprintf(stderr, "<%s>: %d, Expected ID, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  tokenizer3_advance(t);
  s->str_lit_begin = tokenizer3_get(t, 2).loc.begin_index + t->source_code;
  while (!tokenizer3_expect_offset(t, 2, T_DQT)) {
    tokenizer3_advance(t);
  }
  s->str_lit_end = tokenizer3_get(t, 2).loc.begin_index + t->source_code;
  s->length = s->str_lit_end - s->str_lit_begin;
  printf("=> Parsed string_lit\n");
  return s;
}

var_t* parse_var(tokenizer3_t* t) {
  var_t* v = calloc(1, sizeof(var_t));
  if (!tokenizer3_expect_offset(t, 2, T_ID)) {
    fprintf(stderr, "<%s>: %d, Expected ID, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  strncpy(v->name, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);

  if (!tokenizer3_expect_offset(t, 2, T_COLON)) {
    fprintf(stderr, "<%s>: %d, Expected ID, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  tokenizer3_advance(t);

  if (is_type_token(tokenizer3_get(t, 2).type)) {
    v->type = tokenizer3_get(t, 2).type;
    v->type_name = token_type_stringify(v->type);
    tokenizer3_advance(t);
  }

  while (tokenizer3_expect_offset(t, 2, T_MUL)) {
    v->indirection ++;
    tokenizer3_advance(t);
  }
  printf("=> Parsed var\n");
  return v;
}

func_decl_t* parse_func_decl(tokenizer3_t* t) {
  func_decl_t* decl = calloc(1, sizeof(func_decl_t));
  if (!tokenizer3_expect_offset(t, 2, T_ID)) {
    fprintf(stderr, "<%s>: %d, Expected ID, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  strncpy(decl->name, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);

  decl->params = parse_param_list(t);

  printf("=> Parsed func_decl\n");
  return decl;
}

func_t* parse_func(tokenizer3_t* t) {
  func_t* f = calloc(1, sizeof(func_t));
  tokenizer3_advance(t);                      // skip past T_FN
  
  f->decl = parse_func_decl(t);
  if (tokenizer3_expect_offset(t, 2, T_COLON) && is_type_token(tokenizer3_get(t, 3).type)) {
    f->return_type = tokenizer3_get(t, 3).type;
    f->return_type_str = token_type_stringify(f->return_type);
    f->has_return_type = 1;
    tokenizer3_advance(t);
    tokenizer3_advance(t);
  }
  else {
    f->has_return_type = 0;
  }
  f->block = parse_block(t);

  printf("=> Parsed func\n");
  return f;
}

//TODO
func_call_t* parse_func_call(tokenizer3_t* t) {
  func_call_t* func_call = calloc(1, sizeof(func_call_t));

  return func_call;
}

assign_t* parse_var_assign(tokenizer3_t* t) {
  assign_t* a = calloc(1, sizeof(assign_t));
  a->type |= ASSIGN_LHS_VAR;
  a->left_hand_side.var = parse_var(t);
 if (!tokenizer3_expect_offset(t, 2, T_EQ)) {
    fprintf(stderr, "<%s>: %d, Expected LP, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  tokenizer3_advance(t);
  if (tokenizer3_expect_offset(t, 2, T_DQT)) {
    a->right_hand_side.str_lit = parse_string_lit(t);
    a->type |= ASSIGN_RHS_STR_LIT;
    tokenizer3_advance(t);
  }
  else {
    a->right_hand_side.expr = parse_expression(t);
    a->type |= ASSIGN_RHS_EXPR;
  }
  printf("=> Parsed var_assign\n"); 
  return a;
}

assign_t* parse_id_assign(tokenizer3_t* t) {
  assign_t* a = calloc(1, sizeof(assign_t));
  a->type |= ASSIGN_LHS_ID;
  strncpy(a->left_hand_side.id, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);
  if (!tokenizer3_expect_offset(t, 2, T_EQ)) {
    fprintf(stderr, "<%s>: %d, Expected EQ, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }

  tokenizer3_advance(t);
  if (tokenizer3_expect_offset(t, 2, T_DQT)) {
    a->right_hand_side.str_lit = parse_string_lit(t);
    a->type |= ASSIGN_RHS_STR_LIT;
    tokenizer3_advance(t);
  }
  else {
    a->right_hand_side.expr = parse_expression(t);
    a->type |= ASSIGN_RHS_EXPR;
  }

  printf("=> Parsed id_assign\n");
  return a;
}

return_t* parse_return(tokenizer3_t* t) {
  return_t* r = calloc(1, sizeof(return_t));
  tokenizer3_advance(t);

  r->expr = parse_expression(t); 

  printf("=> Parsed return\n");
  return r;
}

param_list_t* parse_param_list(tokenizer3_t* t) {
  param_list_t* p = calloc(1, sizeof(param_list_t));
  p->params = calloc(10, sizeof(var_t));
  if (!tokenizer3_expect_offset(t, 2, T_LP)) {
    fprintf(stderr, "<%s>: %d, Expected LP, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  tokenizer3_advance(t);
  while (tokenizer3_get(t, 2).type != T_RP) {
    var_t* v = parse_var(t);
    p->params[p->params_count++] = v;
    if (tokenizer3_expect_offset(t, 2, T_COMMA)) {
      tokenizer3_advance(t);
      continue;
    }
    else if (tokenizer3_expect_offset(t, 2, T_RP)) {
      tokenizer3_advance(t);
      break;
    }
    else {
      fprintf(stderr, "<%s>: %d, Expected COMMA, got something else\n", __FUNCTION__, __LINE__);
      exit(1);
    }
  }
  printf("=> Parsed param_list\n");
  return p;
}

// TODO
arg_list_t* parse_arg_list(tokenizer3_t* t) {
  return NULL;
}

statement_t* parse_statement(tokenizer3_t* t) {
  statement_t* s = calloc(1, sizeof(statement_t));
  if (tokenizer3_expect_offset(t, 2, T_ID)) {
    if (tokenizer3_expect_offset(t, 3, T_COLON)) {
      s->assign = parse_var_assign(t);
    }
    else if (tokenizer3_expect_offset(t, 3, T_EQ)) {
      s->assign = parse_id_assign(t);
    }
    else if (tokenizer3_expect_offset(t, 3, T_LP)) {
      // func call
    }
  }
  else if (tokenizer3_expect_offset(t, 2, T_IF)) {
    s->iff = parse_if(t);
  }
  else if (tokenizer3_expect_offset(t, 2, T_FOR)) {
    assert(0 && "For loops not supported");
  }
  else if (tokenizer3_expect_offset(t, 2, T_WHILE)) {
    s->whle = parse_while(t);
  }
  else if (tokenizer3_expect_offset(t, 2, T_RETURN)) {
    s->ret = parse_return(t);
  }
  else if (tokenizer3_expect_offset(t, 2, T_ASM)) {
    s->asm_block = parse_asm_block(t);
  }
  else if (tokenizer3_expect_offset(t, 2, T_RB)) {
    // empty
  }
  else {
    // something REALLY unexpected
  }
  return s;
}

if_t* parse_if(tokenizer3_t* t) {
  if_t* iff = calloc(1, sizeof(if_t));
  tokenizer3_advance(t);
  iff->condition = parse_expression(t);
  iff->block = parse_block(t);
  return iff;
}

while_t* parse_while(tokenizer3_t* t) {
  while_t* whle = calloc(1, sizeof(while_t));
  tokenizer3_advance(t);
  whle->condition = parse_expression(t);
  whle->block = parse_block(t);
  return whle;
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
  // printf("=> Parsed expression\n");
  // show_expression(exprs[0], 0);
  return exprs[0];
}

expression_t* parse_expression(tokenizer3_t* t) {
  // convert entire expression to postfix
  token_t postfix[100] = {0};
  int postfix_len = 0;
  
  get_postfix_rep(t, postfix, &postfix_len);
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
