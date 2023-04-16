#include "parser2.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

program_t* parse_program(tokenizer_t* t) {
  /// INITIAL SETUP
  const int FUNC_DEFAULT_MAX = 5;
  const int USE_DEFAULT_MAX  = 5;
  program_t* p = calloc(1, sizeof(program_t));
  p->func_list_max = FUNC_DEFAULT_MAX;
  p->use_list_max = USE_DEFAULT_MAX;
  p->func_list = calloc(p->func_list_max, sizeof(func_t));
  p->use_list = calloc(p->use_list_max, sizeof(use_t));

  /// PROGRAM PARSING
  // Expect use statements first (use statements are only allowed at the start)
  while (tokenizer_get_t(t).type == T_USE) {
    use_t* use = parse_use(t);
    p->use_list[p->use_list_count++] = use;
  }
  tokenizer_advance_t(t);
  
  // Expect functions
  while (tokenizer_get_t(t).type == T_FN) {
    tokenizer_advance_t(t);
    func_t* func = parse_func(t);
    p->func_list[p->func_list_count++] = func;
  }
  show_program(p, 1);

  return p;
}

use_t* parse_use(tokenizer_t* t) {
  use_t* u = calloc(1, sizeof(use_t));
  tokenizer_advance_t(t);
  if (!tokenizer_expect_t(t, T_ID)) {
    fprintf(stderr, "Expected id, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
    exit(1);
  }
  strncpy(u->name, t->source_str + tokenizer_get_t(t).loc.begin_index, tokenizer_get_t(t).loc.length);
  tokenizer_advance_t(t);
  return u;
}

block_t* parse_block(tokenizer_t* t) {
  // for now lets just skip the block
  printf("Block\n");
  while (tokenizer_get_t(t).type != T_RB) {
    tokenizer_show_next_t(t);
    tokenizer_advance_t(t);
  }
  tokenizer_advance_t(t);
  tokenizer_advance_t(t);
  printf("\n");
  return NULL;
}

var_t* parse_var(tokenizer_t* t) {
  var_t* v = calloc(1, sizeof(var_t));
  if (!tokenizer_expect_t(t, T_ID)) {
    fprintf(stderr, "Expected id, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
    exit(1);
  }
  strncpy(v->name, t->source_str + tokenizer_get_t(t).loc.begin_index, tokenizer_get_t(t).loc.length);
  tokenizer_advance_t(t);

  if (!tokenizer_expect_t(t, T_COLON)) {
    fprintf(stderr, "Expected colon, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
    exit(1);
  }
  tokenizer_advance_t(t);
  tokenizer_show_next_t(t);
  token_t token = tokenizer_get_t(t);
  int is_type = token.type == T_INT || 
                token.type == T_SHT ||
                token.type == T_CHR ||
                token.type == T_DBL ||
                token.type == T_FLT ||
                token.type == T_BOOL;
  if (is_type) {
    v->type = tokenizer_get_t(t).type;
    v->type_name = token_type_stringify(tokenizer_get_t(t).type);
    tokenizer_advance_t(t);
  }

  while (tokenizer_expect_t(t, T_MUL)) {
    v->indirection ++;
    tokenizer_advance_t(t);
  }

  return v;
}

assign_t* parse_assign(tokenizer_t* t) {
  return NULL;
}

return_t* parse_return(tokenizer_t* t) {
  return_t* ret = calloc(1, sizeof(return_t));

  return ret;
}

param_list_t* parse_param_list(tokenizer_t* t) {
  param_list_t* plist = calloc(1, sizeof(param_list_t));
  plist->params = calloc(10, sizeof(var_t));
  if (!tokenizer_expect_t(t, T_LP)) {
    fprintf(stderr, "Expected '(', got %s\n", token_type_stringify(tokenizer_get_t(t).type));
    exit(1);
  }
  tokenizer_advance_t(t);
  while (tokenizer_get_t(t).type != T_RP) {
    var_t* v = parse_var(t);
    plist->params[plist->params_count++] = v;
    if (tokenizer_expect_t(t, T_COMMA)) {
      tokenizer_advance_t(t);
      continue;
    }
    else if (tokenizer_expect_t(t, T_RP)) {
      break;
    }
    else {
      fprintf(stderr, "Expected ',', got %s\n", token_type_stringify(tokenizer_get_t(t).type)); 
      exit(1);
    }
  }

  return plist;
}

arg_list_t* parse_arg_list(tokenizer_t* t) {
  return NULL;
}

func_decl_t* parse_func_decl(tokenizer_t* t) {
  func_decl_t* f = calloc(1, sizeof(func_decl_t));
  if (!tokenizer_expect_t(t, T_ID)) {
    fprintf(stderr, "Expected id, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
    exit(1);
  }
  strncpy(f->name, t->source_str + tokenizer_get_t(t).loc.begin_index, tokenizer_get_t(t).loc.length);
  tokenizer_advance_t(t);

  // Expect param list
  f->params = parse_param_list(t);
  return f;
}

func_t* parse_func(tokenizer_t* t) {
  func_t* f = calloc(1, sizeof(func_t));
  func_decl_t* decl = parse_func_decl(t);
  tokenizer_advance_t(t);
  if (tokenizer_expect_t(t, T_COLON)) {
    tokenizer_advance_t(t);
    token_t token = tokenizer_get_t(t);
    int is_type = token.type == T_INT || 
                  token.type == T_SHT ||
                  token.type == T_CHR ||
                  token.type == T_DBL ||
                  token.type == T_FLT ||
                  token.type == T_BOOL;
    if (is_type) {
      f->has_return_type = 1;
      f->return_type = tokenizer_get_t(t).type;
      f->return_type_str = token_type_stringify(tokenizer_get_t(t).type);
    }
    else {
      fprintf(stderr, "Expected type, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
      exit(1);
    }
  }
  block_t* b = parse_block(t);
  printf("After block\n");
  f->decl = decl;
  f->block = b;
  return f;
}

func_call_t* parse_func_call(tokenizer_t* t) {
  return NULL;
}

expression_t* parse_expression(tokenizer_t* t) {
  return NULL;
}

void parse(tokenizer_t* t) {
  program_t* prgm = parse_program(t);
  free_program(prgm);
  free(prgm);
}

void free_program(program_t* p) {
  for (int i = 0; i < p->use_list_count; i++) {
    free_use(p->use_list[i]);
    free(p->use_list[i]);
    p->use_list[i] = NULL;
  }
  free(p->use_list);
  p->use_list = NULL;
  for (int i = 0; i < p->func_list_count; i++) {
    free_func(p->func_list[i]);
    free(p->func_list[i]);
    p->func_list[i] = NULL;
  }
  free(p->func_list);
  p->func_list = NULL;
}

void free_use(use_t* use) {
  // nothing to free (yet)
}

void free_block(block_t* block) {
  // nothing to free (yet)
}

void free_var(var_t* var) {
  // nothing to free (yet)
}

void free_func_decl(func_decl_t* func_decl) {
  free_param_list(func_decl->params);
  free(func_decl->params);
  func_decl->params = NULL;
}

void free_func(func_t* func) {
  free_func_decl(func->decl);
  free(func->decl);
  func->decl = NULL;
  free_block(func->block);
  free(func->block);
  func->block = NULL;
}

void free_func_call(func_call_t* func_call) {
  free_arg_list(func_call->args);
  free(func_call->args);
  func_call->args = NULL;
}

void free_assign(assign_t* assign) {
  free_var(assign->var);
  free(assign->var);
  assign->var = NULL;
  free_expression(assign->expr);
  free(assign->expr);
  assign->expr = NULL;
}

void free_return(return_t* returnp) {
  free_expression(returnp->expr);
  free(returnp->expr);
  returnp->expr = NULL;
}

void free_param_list(param_list_t* param_list) {
  for (int i = 0; i < param_list->params_count; i++) {
    free_var(param_list->params[i]);
    free(param_list->params[i]);
    param_list->params[i] = NULL;
  }
  free(param_list->params);
  param_list->params = NULL;
}

void free_arg_list(arg_list_t* arg_list) {

}

void free_expression(expression_t* expr) {

}

void tabs(int count) {
  for (int i = 0; i < count; i++) {
    printf("  ");
  }
}

void show_program(program_t* p, int level) {
  tabs(level - 1); printf("\\_ Program\n");
  if (!p) {
    tabs(level); printf("\\_ NULL\n");
    return;
  }
  tabs(level); printf("\\_ UseList\n");
  for (int i = 0; i < p->use_list_count; i++) {
    show_use(p->use_list[i], level + 1);
  }
  tabs(level); printf("\\_ FuncList\n");
  for (int i = 0; i < p->func_list_count; i++) {
    show_func(p->func_list[i], level + 1);
  }
}

void show_use(use_t* u, int level) {
  tabs(level); printf("\\_ Use\n");
  tabs(level); printf("  \\_ name: %s\n", u->name);
}

void show_block(block_t* block, int level) {
  tabs(level - 1); printf("\\_ Block\n");
  if (!block) {
    tabs(level + 1); printf("\\_ NULL\n");
    return;
  }
}

void show_func(func_t* func, int level) {
  tabs(level); printf("\\_ Func\n");
  tabs(level - 1); show_func_decl(func->decl, level + 1);
  tabs(level + 1); printf("\\_ Return Type : %d = %s\n", func->return_type, func->has_return_type ? func->return_type_str : "N/A");
  tabs(level - 1); show_block(func->block, level + 1);
}

void show_func_decl(func_decl_t* decl, int level) {
  tabs(level - 1); printf("\\_ FuncDecl\n");
  if (!decl) {
    tabs(level); printf("\\_ NULL\n");
    return;
  }
  tabs(level + 1); printf("\\_ name: %s\n", decl->name);
  show_param_list(decl->params, level + 1);
}

void show_func_call(func_call_t* func_call, int level) {
  tabs(level - 1); printf("\\_ FuncCall\n");
  if (!func_call) {
    tabs(level); printf("\\_ NULL\n");
    return;
  }
}

void show_assign(assign_t* assign, int level) {
  tabs(level - 1); printf("\\_ Assign\n");
  if (!assign) {
    tabs(level); printf("\\_ NULL\n");
    return;
  }
}

void show_return(return_t* _return, int level) {
  tabs(level - 1); printf("\\_ Return\n");
  if (!_return) {
    tabs(level + 1); printf("\\_ NULL\n");
    return;
  }
  tabs(level); show_expression(_return->expr, level + 1);
}

void show_param_list(param_list_t* params, int level) {
  tabs(level); printf("\\_ Params\n");
  if (!params) {
    tabs(level + 1); printf("\\_ NULL\n");
    return;
  }
  for (int i = 0; i < params->params_count; i++) {
    show_var(params->params[i], level + 1);
  }
}

void show_var(var_t* var, int level) {
  tabs(level); printf("\\_ Var\n");
  if (!var) {
    tabs(level+1); printf("\\_ NULL\n");
    return;
  }
  tabs(level + 1); printf("\\_ name: %s\n", var->name);
  tabs(level + 1); printf("\\_ type: %s\n", var->type_name);
  tabs(level + 1); printf("\\_ indirection: %d\n", var->indirection);
}

void show_expression(expression_t* expr, int level) {
  tabs(level); printf("\\_ Expr\n");
  if (!expr) {
    tabs(level+1); printf("\\ _NULL\n");
    return;
  }
}
