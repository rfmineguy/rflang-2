#include "parser3.h"
#include "../tokenizer3.h"
#include "../logging/logger_w_channels.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <unistd.h> //sleep

// CHANNEL defs in 'logging/logger_w_channels.h'

int is_asm_type_specifier(token_type_t type) {
  return type == T_X86_32_LINUX || type == T_X86_64_LINUX || type == T_ARM64;
}

int is_operator(token_type_t type) {
  return type == T_PLUS || type == T_MINUS || type == T_MUL || type == T_DIV || type == T_MOD;
}

int is_type_token(token_type_t type) {
  return type == T_INT || type == T_SHT || type == T_CHR || type == T_DBL || type == T_FLT || type == T_BOOL || type == T_VOID;
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
  logger_channel_set_stream(logger_get_global(), WARNING_CHANNEL, stdout);
  logger_channel_set_stream(logger_get_global(), ERROR_CHANNEL, stdout);
  logger_channel_set_stream(logger_get_global(), PARSE_STATUS_CHANNEL, stdout);
  logger_channel_enable(logger_get_global(), WARNING_CHANNEL,      "Warn",        ANSI_RESET);
  logger_channel_enable(logger_get_global(), ERROR_CHANNEL,        "Error",       ANSI_RED);
  logger_channel_enable(logger_get_global(), PARSE_STATUS_CHANNEL, "ParseStatus", ANSI_RESET);
  return parse_program(t);
}

program_t* parse_program(tokenizer3_t* t) {
  error_context_t ctx = error_context_new();

  const int FUNC_DEFAULT_MAX = 5;
  const int USE_DEFAULT_MAX  = 5;
  program_t* p = calloc(1, sizeof(program_t));
  p->func_list_max = FUNC_DEFAULT_MAX;
  p->use_list_max = USE_DEFAULT_MAX;
  p->func_list = calloc(p->func_list_max, sizeof(func_t));
  p->use_list = calloc(p->use_list_max, sizeof(use_t));

  tokenizer3_advance(t);

  if (tokenizer3_expect_offset(t, 2, T_ID)) {
    error_push(&ctx, error_new(E_INVALID_START_TOKEN, tokenizer3_get(t, 2)));
    tokenizer3_advance(t);
    // fprintf(stderr, "Program starting with ID is invalid\n");
    // exit(1);
  }
  // Expect use statements second (use statements are only allowed at the start)
  while (tokenizer3_get(t, 2).type == T_USE) {
    use_t* use = parse_use(t, &ctx);
    p->use_list[p->use_list_count++] = use;
  }

  // Expect functions
  while (tokenizer3_get(t, 2).type == T_FN) {
    func_t* func = parse_func(t, &ctx);
    p->func_list[p->func_list_count++] = func;
    if (!tokenizer3_expect_offset(t, 2, T_SEMICOLON)) {
      error_push(&ctx, error_new(E_MISSING_SEMICOLON, tokenizer3_get(t, 2)));
    }
    tokenizer3_advance(t);
  }
  error_show_all(&ctx);
  error_context_free(ctx);

  return p;
}

use_t* parse_use(tokenizer3_t* t, error_context_t* ctx) {
  use_t* u = calloc(1, sizeof(use_t));
  tokenizer3_advance(t);
  if (!tokenizer3_expect_offset(t, 2, T_ID)) {
    error_push(ctx, error_new(E_MISSING_USE_PATH, tokenizer3_get(t, 1)));
    tokenizer3_advance(t);
    // fprintf(stderr, "Expected ID, got something else\n"); 
    // exit(1);
  }
  strncpy(u->name, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);

  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> Parsed use\n");

  return u;
}

block_t* parse_block(tokenizer3_t* t, error_context_t* ctx) {
  const int MAX_STATEMENTS = 10;
  block_t* b = calloc(1, sizeof(block_t));
  if (!tokenizer3_expect_offset(t, 2, T_LB)) {
    error_push(ctx, error_new(E_MISSING_LB, tokenizer3_get(t, 2)));
  }
  b->statements = calloc(MAX_STATEMENTS, sizeof(statement_t)); //NOTE: This is VERY inflexible. Maybe use a linked list here?
  tokenizer3_advance(t);
  while (tokenizer3_get(t, 2).type != T_RB) {
    statement_t* s = parse_statement(t, ctx);
    if (s) {
       b->statements[b->statement_count++] = s;
    }
    tokenizer3_advance(t);
    if (tokenizer3_get(t, 2).type == T_EOF) {
      // encountered end of the file
      return b;
    }
    // printf("block: "); tokenizer3_show_token_offset(t, 2);
  }
  tokenizer3_advance(t);    //consume last '}'
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> Parsed block\n");
  return b;
}

asm_block_t* parse_asm_block(tokenizer3_t* t, error_context_t* ctx) {
  asm_block_t* block = calloc(1, sizeof(asm_block_t));
  tokenizer3_advance(t);
  // tokenizer3_show_token_offset(t, 2);
  if (!is_asm_type_specifier(tokenizer3_get(t, 2).type)) {
    error_push(ctx, MAKE_GENERAL_EXPECT_ERROR("Assembly type", tokenizer3_get(t, 2)));
    tokenizer3_advance(t);
  }
  block->asm_type = tokenizer3_get(t, 2).type;
  tokenizer3_advance(t);
  if (!tokenizer3_expect_offset(t, 2, T_LB)) {
    error_push(ctx, MAKE_GENERAL_EXPECT_ERROR("Left brace", tokenizer3_get(t,2)));
  }
  block->asm_source_code_begin = tokenizer3_get(t, 2).loc.begin_index + tokenizer3_get(t, 2).loc.length + t->source_code;
  while (!tokenizer3_expect_offset(t, 2, T_RB)) {
    tokenizer3_advance(t);
  }
  block->asm_source_code_end = tokenizer3_get(t, 2).loc.begin_index + t->source_code;
  // tokenizer3_show_token_offset(t, 2);

  tokenizer3_advance(t);
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> Parsed asm block\n");
  return block;
}

string_lit_t* parse_string_lit(tokenizer3_t* t, error_context_t* ctx) {
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
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> Parsed string literal\n");
  return s;
}

var_t* parse_var(tokenizer3_t* t, error_context_t* ctx) {
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
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> Parsed var\n");
  return v;
}

func_decl_t* parse_func_decl(tokenizer3_t* t, error_context_t* ctx) {
  func_decl_t* decl = calloc(1, sizeof(func_decl_t));
  if (!tokenizer3_expect_offset(t, 2, T_ID)) {
    fprintf(stderr, "<%s>: %d, Expected ID, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  strncpy(decl->name, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);

  decl->params = parse_param_list(t, ctx);
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed func_decl\n");
  return decl;
}

func_t* parse_func(tokenizer3_t* t, error_context_t* ctx) {
  func_t* f = calloc(1, sizeof(func_t));
  tokenizer3_advance(t);                      // skip past T_FN
  
  f->decl = parse_func_decl(t, ctx);
  int has_colon = tokenizer3_expect_offset(t, 2, T_COLON);
  int has_type  = is_type_token(tokenizer3_get(t, 3).type);
  if (has_colon && has_type) {
    f->return_type = tokenizer3_get(t, 3).type;
    f->return_type_str = token_type_stringify(f->return_type);
    f->has_return_type = 1;
    tokenizer3_advance(t);
    tokenizer3_advance(t);
  }
  else if (has_colon && !has_type) {
    error_push(ctx, error_new(E_MISSING_FUNC_RETURN_TYPE, tokenizer3_get(t, 2)));
    tokenizer3_advance(t);
  }
  else if (!has_colon && !has_type) {
    f->has_return_type = 1;
    f->return_type = T_VOID;
  }
  f->block = parse_block(t, ctx);

  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed func\n");
  return f;
}

func_call_t* parse_func_call(tokenizer3_t* t, error_context_t* ctx) {
  func_call_t* func_call = calloc(1, sizeof(func_call_t));
  if (!tokenizer3_expect_offset(t, 2, T_ID)) {
    fprintf(stderr, "<%s>: %d, Expected ID, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  strncpy(func_call->name, t->source_code + tokenizer3_get(t, 2).loc.begin_index, tokenizer3_get(t, 2).loc.length);
  tokenizer3_advance(t);

  func_call->args = parse_arg_list(t, ctx);
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed func_call\n");
  return func_call;
}

assign_t* parse_var_assign(tokenizer3_t* t, error_context_t* ctx) {
  assign_t* a = calloc(1, sizeof(assign_t));
  a->type |= ASSIGN_LHS_VAR;
  a->left_hand_side.var = parse_var(t, ctx);
  if (!tokenizer3_expect_offset(t, 2, T_EQ)) {
    error_push(ctx, error_new(E_MISSING_ASSIGN_EQ, tokenizer3_get(t, 2)));
  }
  tokenizer3_advance(t);
  if (tokenizer3_expect_offset(t, 2, T_DQT)) {
    a->right_hand_side.str_lit = parse_string_lit(t, ctx);
    a->type |= ASSIGN_RHS_STR_LIT;
    tokenizer3_advance(t);
  }
  else {
    a->right_hand_side.expr = parse_expression(t, ctx);
    a->type |= ASSIGN_RHS_EXPR;
  }
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed var_assign\n");
  return a;
}

assign_t* parse_id_assign(tokenizer3_t* t, error_context_t* ctx) {
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
    a->right_hand_side.str_lit = parse_string_lit(t, ctx);
    a->type |= ASSIGN_RHS_STR_LIT;
    tokenizer3_advance(t);
  }
  else {
    a->right_hand_side.expr = parse_expression(t, ctx);
    a->type |= ASSIGN_RHS_EXPR;
  }

  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed id_assign\n");
  return a;
}

return_t* parse_return(tokenizer3_t* t, error_context_t* ctx) {
  return_t* r = calloc(1, sizeof(return_t));
  tokenizer3_advance(t);

  r->expr = parse_expression(t, ctx); 

  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed return\n");
  return r;
}

param_list_t* parse_param_list(tokenizer3_t* t, error_context_t* ctx) {
  param_list_t* p = calloc(1, sizeof(param_list_t));
  p->params = calloc(10, sizeof(var_t));
  if (!tokenizer3_expect_offset(t, 2, T_LP)) {
    fprintf(stderr, "<%s>: %d, Expected LP, got something else\n", __FUNCTION__, __LINE__);
    exit(1);
  }
  tokenizer3_advance(t);
  while (!tokenizer3_expect_offset(t, 2, T_RP)) {
    var_t* v = parse_var(t, ctx);
    p->params[p->params_count++] = v;
    if (tokenizer3_expect_offset(t, 2, T_COMMA)) {
      tokenizer3_advance(t);
      continue;
    }
    else if (tokenizer3_expect_offset(t, 2, T_RP)) {
      break;
    }
    else {
      fprintf(stderr, "<%s>: %d, Expected COMMA, got something else\n", __FUNCTION__, __LINE__);
      exit(1);
    }
  }
  tokenizer3_advance(t);
  //tokenizer3_show_token_offset(t, 2);
  
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed param_list\n");
  return p;
}

arg_list_t* parse_arg_list(tokenizer3_t* t, error_context_t* ctx) {
  printf("=> Parsing arg_list\n");
  arg_list_t* args = calloc(1, sizeof(arg_list_t));
  args->args = calloc(10, sizeof(expression_t));
  if (!tokenizer3_expect_offset(t, 2, T_LP)) {
     fprintf(stderr, "<%s>: %d, Expected LP, got something else\n", __FUNCTION__, __LINE__);
     exit(1);
  }
  tokenizer3_advance(t);
  tokenizer3_show_token_offset(t, 2);
  while (tokenizer3_get(t, 2).type != T_RP) {
    expression_t* e = parse_expression(t, ctx);
    args->args[args->arg_count++] = e;
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
  // tokenizer3_advance(t);
  // printf("parse_arg_list: ");
  // tokenizer3_show_token_offset(t, 2);
  // printf("=> Parsed arg_list\n");
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed arg_list\n");
  return args;
}

statement_t* parse_statement(tokenizer3_t* t, error_context_t* ctx) {
  statement_t* s = calloc(1, sizeof(statement_t));
  // printf("offset(2): "); tokenizer3_show_token_offset(t, 2);
  if (tokenizer3_expect_offset(t, 2, T_ID)) {
    if (tokenizer3_expect_offset(t, 3, T_COLON)) {
      s->assign = parse_var_assign(t, ctx);
    }
    else if (tokenizer3_expect_offset(t, 3, T_EQ)) {
      s->assign = parse_id_assign(t, ctx);
    }
    else if (tokenizer3_expect_offset(t, 3, T_LP)) {
      s->func_call_expr = parse_expression(t, ctx);
      // assert(0 && "Function calls unsupported right now");

      // func call
    }
  }
  else if (tokenizer3_expect_offset(t, 2, T_IF)) {
    s->iff = parse_if(t, ctx);
  }
  else if (tokenizer3_expect_offset(t, 2, T_FOR)) {
    assert(0 && "For loops not supported");
  }
  else if (tokenizer3_expect_offset(t, 2, T_WHILE)) {
    s->whle = parse_while(t, ctx);
  }
  else if (tokenizer3_expect_offset(t, 2, T_RETURN)) {
    s->ret = parse_return(t, ctx);
  }
  else if (tokenizer3_expect_offset(t, 2, T_ASM)) {
    s->asm_block = parse_asm_block(t, ctx);
  }
  else if (tokenizer3_expect_offset(t, 2, T_RB)) {
    free(s);
    return NULL;
    // empty
  }
  else {
    error_push(ctx, error_new(E_STATEMENT,  tokenizer3_get(t, 2)));
    return NULL;
  }
  // printf("parse_statement: ");
  // usleep(10000000.f/120);
  // printf("offset(2): "); tokenizer3_show_token_offset(t, 2);
  if (!tokenizer3_expect_offset(t, 2, T_SEMICOLON)) {
    error_push(ctx, error_new(E_MISSING_SEMICOLON, tokenizer3_get(t, 2)));
    // tokenizer3_advance(t);
    // fprintf(stderr, "<%s>: %d, Expected SEMICON, got something else %d\n", __FUNCTION__, __LINE__, tokenizer3_get(t, 2).type);
    // exit(1);
  }
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed statement\n");
  return s;
}

if_t* parse_if(tokenizer3_t* t, error_context_t* ctx) {
  if_t* iff = calloc(1, sizeof(if_t));
  tokenizer3_advance(t);
  iff->condition = parse_expression(t, ctx);
  iff->block = parse_block(t, ctx);
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed if\n");
  return iff;
}

while_t* parse_while(tokenizer3_t* t, error_context_t* ctx) {
  while_t* whle = calloc(1, sizeof(while_t));
  tokenizer3_advance(t);
  whle->condition = parse_expression(t, ctx);
  whle->block = parse_block(t, ctx);
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed while\n");
  return whle;
}

expression_t* parse_expression_postfix(token_t* postfix, int postfix_len, error_context_t* ctx) {
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
    else if (postfix[i].type == T_EXPR_FUNC_END) {
      // parse as an expr func
      func_call_t* func_call = calloc(1, sizeof(func_call_t));
      func_call->args = calloc(1, sizeof(arg_list_t));
      func_call->args->args = calloc(10, sizeof(expression_t));
      while (exprs[top]->type != EXPR_STRING) {
        //printf("top=%d\n",top);
        //show_expression(exprs[top], 0);
        func_call->args->args[func_call->args->arg_count++] = exprs[top];
        top--;
      }
      if (top < 0)
        top = 0;
      // printf("Made it to ID, top=%d\n", top);
      strncpy(func_call->name, exprs[top]->value.s, 30);
      e->value.func_call = func_call;
      top--;
      e->type = EXPR_FUNC_CALL;
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
  // show_expression(exprs[0], 0);
  return exprs[0];
}

expression_t* parse_expression(tokenizer3_t* t, error_context_t* ctx) {
  // printf("Parsing expression\n");
  // convert entire expression to postfix
  token_t postfix[100] = {0};
  int postfix_len = 0;
  
  get_postfix_rep(t, postfix, &postfix_len, ctx);
  // for (int i = 0; i < postfix_len; i++) {
  //   tokenizer3_token_print(postfix[i], t);
  // }
  expression_t* e = parse_expression_postfix(postfix, postfix_len, ctx); 
  logger_log(logger_get_global(), PARSE_STATUS_CHANNEL, "=> parsed expression\n");
  return e;
}

void get_postfix_rep(tokenizer3_t* t, token_t* postfix_out, int* postfix_length, error_context_t* ctx) {
  int j = 0;
  token_t stack[100] = {0};
  int top = -1;
  
  int was_id_before = 0;      // used for function call detection
  while (1) {
    int is_id             = tokenizer3_expect_offset(t, 2, T_ID);
    int is_comma          = tokenizer3_expect_offset(t, 2, T_COMMA);
    int is_id_or_num      = is_id                                  || tokenizer3_expect_offset(t, 2, T_NUM);
    int is_math_operator  = tokenizer3_expect_offset(t, 2, T_PLUS) || tokenizer3_expect_offset(t, 2, T_MINUS) || tokenizer3_expect_offset(t, 2, T_MUL) || tokenizer3_expect_offset(t, 2, T_DIV) || tokenizer3_expect_offset(t, 2, T_MOD);
    int is_comp_operator  = 
      tokenizer3_expect_offset(t, 2, T_GT) || tokenizer3_expect_offset(t, 2, T_GTEQ) || tokenizer3_expect_offset(t, 2, T_LT) || tokenizer3_expect_offset(t, 2, T_LTEQ) || tokenizer3_expect_offset(t, 2, T_NEQ) || tokenizer3_expect_offset(t, 2, T_DEQ);
    int is_logic_operator = tokenizer3_expect_offset(t, 2, T_DAND) || tokenizer3_expect_offset(t, 2, T_DOR);
    int is_lparen         = tokenizer3_expect_offset(t, 2, T_LP);
    int is_rparen         = tokenizer3_expect_offset(t, 2, T_RP);

    if (is_id_or_num) {
      postfix_out[j++] = tokenizer3_get(t, 2);
    }
    else if (is_comma) {
      stack[++top] = tokenizer3_get(t, 2);
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
      was_id_before += tokenizer3_expect_offset(t, 1, T_ID);
      stack[++top] = tokenizer3_get(t, 2);
    }
    else if (is_rparen) {
      while (stack[top].type != T_LP) {
        if (stack[top].type == T_COMMA) {
          top--;
          // comma_count++;
          continue;
        }
        postfix_out[j++] = stack[top--];
      }
      if (was_id_before > 0) {
        postfix_out[j++] = (token_t) { .type = T_EXPR_FUNC_END, .value.s = "@" };
        was_id_before--;
      }
      if (top > -1 && stack[top].type != T_LP) {
        error_push(ctx, MAKE_GENERAL_EXPECT_ERROR("Mismatched ')'", tokenizer3_get(t, 2)));
        // error
      }
      else {
        top--;
      }
    }
    else {
      // recenter the tokenizer
      // shift everything back
      break;
    }
    tokenizer3_advance(t);
  }
  // printf("expr_last_t: "); tokenizer3_show_token_offset(t, 2);

  while (top > -1) {
    if (stack[top].type == T_LP) {
      error_push(ctx, error_new(E_MISMATCHED_LP, tokenizer3_get(t, 2)));
      // error
    }
    postfix_out[j++] = stack[top--];
  }
  *postfix_length = j;
}
