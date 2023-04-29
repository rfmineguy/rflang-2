#include "parser2.h"
#include "errors.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
// + t->current.loc.length
#define ERROR(msg, ...) \
  error_print((error_t) {.line=t->line, .column=t->col}, t, msg, __FUNCTION__, __LINE__, ##__VA_ARGS__);\
  exit(1)

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
  if (p->use_list_count != 0)
    tokenizer_advance_t(t);
  
  // Expect functions
  while (tokenizer_get_t(t).type == T_FN) {
    tokenizer_advance_t(t);
    func_t* func = parse_func(t);
    p->func_list[p->func_list_count++] = func;
    // show_func(func, 0);
  }

  return p;
}

use_t* parse_use(tokenizer_t* t) {
  use_t* u = calloc(1, sizeof(use_t));
  tokenizer_advance_t(t);
  if (!tokenizer_expect_t(t, T_ID)) {
    ERROR("Expected id, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
    exit(1);
  }
  strncpy(u->name, t->source_str + tokenizer_get_t(t).loc.begin_index, tokenizer_get_t(t).loc.length);
  tokenizer_advance_t(t);
  return u;
}

block_t* parse_block(tokenizer_t* t) {
  block_t* b = calloc(1, sizeof(block_t));
  printf("Block\n");
  if (!tokenizer_expect_t(t, T_LB)) {
    ERROR("Expected T_LB, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
    exit(1);
  }
  b->statements = calloc(1, sizeof(statement_t) * 5);
  tokenizer_advance_t(t);
  while (tokenizer_get_t(t).type != T_RB) {
    statement_t* s = parse_statement(t);
    if (s) {
      b->statements[b->statement_count++] = s;
    }
    tokenizer_advance_t(t);
  }
  tokenizer_advance_t(t);
  tokenizer_advance_t(t);
  printf("\n");
  return b;
}

var_t* parse_var(tokenizer_t* t) {
  var_t* v = calloc(1, sizeof(var_t));
  if (!tokenizer_expect_t(t, T_ID)) {
    ERROR("Expected T_ID, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
  }
  strncpy(v->name, t->source_str + tokenizer_get_t(t).loc.begin_index, tokenizer_get_t(t).loc.length);
  tokenizer_advance_t(t);

  if (!tokenizer_expect_t(t, T_COLON)) {
    ERROR("Expected T_COLON, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
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
  printf("Parsing assign\n");
  assign_t* a = calloc(1, sizeof(assign_t));
  var_t* v = parse_var(t);
  if (!tokenizer_expect_t(t, T_EQ)) {
    ERROR("Expected T_EQ, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
  }
  tokenizer_advance_t(t);
  expression_t* e = parse_expression(t);
  a->var = v;
  a->expr = e;
  return a;
}

return_t* parse_return(tokenizer_t* t) {
  return_t* ret = calloc(1, sizeof(return_t));
  expression_t* e = parse_expression(t);
  ret->expr = e;
  return ret;
}

param_list_t* parse_param_list(tokenizer_t* t) {
  param_list_t* plist = calloc(1, sizeof(param_list_t));
  plist->params = calloc(10, sizeof(var_t));
  if (!tokenizer_expect_t(t, T_LP)) {
    ERROR("Expected T_LP, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
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
      printf("column: %d\n", t->col);
      ERROR("Expected T_COMMA, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
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
    ERROR("Expected T_ID, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
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
      tokenizer_advance_t(t);
    }
    else {
      ERROR("Expected type, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
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

/*
 * T_NUM T_ID T_PLUS T_NUM T_MINUS
 * 0     1    2      3     4
 */

void show_stack(expression_t** stack, int length) {
  printf("Stack: %d\n", length);
  for (int i = 0; i < length; i++) {
    if (stack[i]) {
      printf("Showing expression\n");
      show_expression(stack[i], 1);
    }
  }
}

expression_t* parse_expression_v2(token_t* postfix, int postfix_len) {
  expression_t* exprs[postfix_len];
  int main_counter = 0;
  int stack_top = 0;
  while (main_counter < postfix_len) {
    expression_t* e = calloc(1, sizeof(expression_t));
    if (postfix[main_counter].type == T_ID) {
      e->type = EXPR_STRING;
      strncpy(e->value.s, postfix[main_counter].value.s, 30);
      exprs[stack_top] = e;
      stack_top++;
    }
    else if (postfix[main_counter].type == T_NUM) {
      e->type = EXPR_NUM;
      e->value.i = postfix[main_counter].value.i;
      exprs[stack_top] = e;
      stack_top++;
    }
    else {
      //NOTE: Confirm the offsets
      expression_t* left  = exprs[stack_top - 1];
      expression_t* right = exprs[stack_top - 2];
      exprs[stack_top - 1] = NULL;
      exprs[stack_top - 2] = NULL;
      stack_top --;
      stack_top --;
      e->type = EXPR_COMPOUND;
      e->value.operation = postfix[main_counter].type;
      e->left = left;
      e->right = right;
      exprs[stack_top] = e;
      stack_top++;
    }
    main_counter ++;
  }

  return exprs[0];
}

expression_t* parse_expression(tokenizer_t* t) {
  // convert entire expression to postfix
  token_t postfix[100] = {0};
  int postfix_len = 0;
  get_postfix_rep(t, postfix, &postfix_len);
  printf("postfix_len: %d\n", postfix_len);
  for (int i = 0; i < postfix_len; i++) {
    token_print(postfix[i], t);
  }
  //return parse_expression_ex(t, NULL, postfix, postfix_len - 1, -1);
  return parse_expression_v2(postfix, postfix_len);
}

void get_postfix_rep(tokenizer_t* t, token_t* postfix_out, int* postfix_length) {
  int j = 0;
  token_t stack[100];
  int top = -1;

  // NOTE: Need to break at some point (when?)
  // NOTE: Converts stream of tokens into postfix (stored in 'postfix' array) 
  while (1) {
    if (tokenizer_expect_t(t, T_ID) || tokenizer_expect_t(t, T_NUM)) {
      postfix_out[j++] = tokenizer_get_t(t);
    }
    else if (tokenizer_expect_t(t, T_LP)) {
      stack[++top] = tokenizer_get_t(t);
    }
    else if (tokenizer_expect_t(t, T_RP)) {
      while (top > -1 && stack[top].type != T_LP) {
        postfix_out[j++] = stack[top--];
      }
      if (top > -1 && stack[top].type != T_LP) {
        return;
      }
      else {
        top--;
      }
    }
    else if (is_operator(tokenizer_get_t(t).type)) {
      while (top > -1 && get_precedence(stack[top].type) > get_precedence(tokenizer_get_t(t).type)) {
        postfix_out[j++] = stack[top--];
      }
      stack[++top] = tokenizer_get_t(t);
    }
    else {
      break;
    }
    tokenizer_advance_t(t);
  }
  while (top > -1) {
    if (stack[top].type == T_LP) {
      return;
    }
    postfix_out[j++] = stack[top--];
  }
  *postfix_length = j;
}

statement_t* parse_statement(tokenizer_t* t) {
  printf("Parsing statement\n");
  statement_t* s = calloc(1, sizeof(statement_t));
  if (tokenizer_expect_t(t, T_ID)) {
    tokenizer_advance_t(t);
    if (tokenizer_expect_t(t, T_LP)) {
      assert(0 && "Encountered function call");
    }
    else {
      //assign
      printf("Parsing assign\n");
      tokenizer_rewind_t(t);
      assign_t* a = parse_assign(t);
      s->assign = a;
      // tokenizer_advance_t(t);
    }
  }
  else if (tokenizer_expect_t(t, T_IF)) {
    assert(0 && "Encountered if");
  }
  else if (tokenizer_expect_t(t, T_FOR)) {
    assert(0 && "Encountered for (UNSUPPORTED FOR NOW)");
  }
  else if (tokenizer_expect_t(t, T_WHILE)) {
    assert(0 && "Encountered while");
  }
  else if (tokenizer_expect_t(t, T_RETURN)) {
    printf("Parse return\n");
    tokenizer_advance_t(t);
    s->ret = parse_return(t);
  }
  else if (tokenizer_expect_t(t, T_ASM)) {
    assert(0 && "Encountered asm");
  }
  else if (tokenizer_expect_t(t, T_RB)) {
    free(s);
    s = NULL;
    return NULL;
  }
  else {
    ERROR("Unexpected token: %s\n", token_type_stringify(tokenizer_get_t(t).type));
  }

  // printf("Next\n");
  tokenizer_show_next_t(t);
  if (!tokenizer_expect_t(t, T_SEMICOLON)) {
    free(s);
    s = NULL;
    ERROR("Expected SEMI, got %s\n", token_type_stringify(tokenizer_get_t(t).type));
  }
  return s;
}

if_t* parse_if(tokenizer_t* t) {
  if_t* iff = calloc(1, sizeof(if_t));
  return iff;
}

program_t* parse(tokenizer_t* t) {
  return parse_program(t);
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
  for (int i = 0; i < block->statement_count; i++) {
    free_statement(block->statements[i]);
    block->statements[i] = NULL;
  }
  free(block->statements);
  block->statements = NULL;
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
  if (!expr) return;
  free_expression(expr->left);
  free(expr->left);
  free_expression(expr->right);
  free(expr->right);
  printf("Freed expression\n");
}

void tabs(int count) {
  for (int i = 0; i < count; i++) {
    printf("  ");
  }
}

void free_statement(statement_t* stmt) {
  if (stmt->iff) {
    // free_iff(stmt->iff);
  }
  if (stmt->ret) {
    free_return(stmt->ret);
    free(stmt->ret);
    stmt->ret = NULL;
  }
  if (stmt->func_call) {
    free_func_call(stmt->func_call);
    free(stmt->func_call);
    stmt->func_call = NULL;
  }
  if (stmt->assign) {
    free_assign(stmt->assign);
    free(stmt->assign);
    stmt->assign = NULL;
  }
  free(stmt);
  stmt = NULL;
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
  tabs(level + 1); printf("\\_ Statements\n");
  if (block->statement_count == 0) {
    tabs(level + 2); printf("\\_ NULL\n"); 
    return;
  }
  for (int i = 0; i < block->statement_count; i++) {
    show_statement(block->statements[i], level + 2);
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
  show_var(assign->var, level);
  show_expression(assign->expr, level);
}

void show_return(return_t* _return, int level) {
  tabs(level - 1); printf("\\_ Return\n");
  if (!_return) {
    tabs(level + 1); printf("\\_ NULL\n");
    return;
  }
  show_expression(_return->expr, level);
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
  tabs(level); printf("\\_ Expr");
  if (!expr) {
    // printf("\n");
    tabs(level+1); printf("\\_ NULL\n");
    return;
  }
  switch (expr->type) {
    case EXPR_NUM: {
      printf("\n");
      tabs(level + 1); 
      printf("\\_Value {num: %d}\n", expr->value.i);
      break;
    }
    case EXPR_STRING: {
      printf("\n");
      tabs(level + 1); 
      printf("\\_Value {str: %s}\n", expr->value.s); 
      break;
    }
    case EXPR_COMPOUND: {
      // tabs(level + 1);
      switch (expr->value.operation) {
        case T_PLUS:  printf("{+}\n"); break;
        case T_MINUS: printf("{-}\n"); break;
        case T_MUL:   printf("{*}\n"); break;
        case T_DIV:   printf("{/}\n"); break;
        case T_MOD:   printf("{*}\n"); break;
        default: break;
      }
      if (expr->left) show_expression(expr->left, level + 1);
      if (expr->right) show_expression(expr->right, level + 1);
      break;
    }
  }
}

void show_iff(if_t* iff, int level) {
  tabs(level); printf("\\_ If\n");
  if (!iff) {
     tabs(level + 1); printf("\\_ NULL\n");
     return;
  }
  // tabs(level + 1); show_condition(iff->condition, level + 1);
  tabs(level + 1); show_block(iff->block, level + 1);
}

void show_statement(statement_t* stmt, int level) {
  tabs(level); printf("\\_ Statement\n");
  if (!stmt) {
    tabs(level); printf("\\_ NULL\n");
    return;
  }
  if (stmt->iff) {
    show_iff(stmt->iff, level + 2); 
  }
  else if (stmt->ret) {
    show_return(stmt->ret, level + 2);
  }
  else if (stmt->func_call) {
    show_func_call(stmt->func_call, level + 2);
  }
  else if (stmt->assign) {
    show_assign(stmt->assign, level + 2);
  }
  else {
    tabs(level); printf("\\_ %p\n", stmt->ret);
    tabs(level); printf("\\_ Statement empty\n");
  }
}
