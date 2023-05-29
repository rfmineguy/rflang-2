#include "parse_tree.h"
#include <stdlib.h>
#include <stdio.h>

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

void free_asm_block(asm_block_t* asm_block) {
  // nothing to free (yet)
}

void free_string_lit(string_lit_t* string_lit) {
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
  if (assign->type & ASSIGN_LHS_VAR) {
    free_var(assign->left_hand_side.var);
    free(assign->left_hand_side.var);
    assign->left_hand_side.var = NULL;
  }
  if (assign->type & ASSIGN_LHS_ID) {
    // nothing to do
  }
  if (assign->type & ASSIGN_RHS_STR_LIT) {
    // nothing to do
  }
  if (assign->type & ASSIGN_RHS_EXPR) {
    free_expression(assign->right_hand_side.expr);
    free(assign->right_hand_side.expr);
    assign->right_hand_side.expr = NULL;
  }
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
  for (int i = 0; i < arg_list->arg_count; i++) {
    free_expression(arg_list->args[i]);
    free(arg_list->args[i]);
    arg_list->args[i] = NULL;
  }
  free(arg_list->args);
  arg_list->args = NULL;
}

void free_expression(expression_t* expr) {
  if (!expr) return;
  free_expression(expr->left);
  free(expr->left);
  free_expression(expr->right);
  free(expr->right);
}

void free_iff(if_t* iff) {
  if (!iff) return;
  free_expression(iff->condition);
  free(iff->condition);
  free_block(iff->block);
  free(iff->block);
}

void free_while(while_t* whle) {
  if (!whle) return;
  free_expression(whle->condition);
  free(whle->condition);
  free_block(whle->block);
  free(whle->block);
}

void tabs(int count) {
  for (int i = 0; i < count; i++) {
    printf("  ");
  }
}

void free_statement(statement_t* stmt) {
  if (stmt->iff) {
    free_iff(stmt->iff);
    free(stmt->iff);
    stmt->iff = NULL;
  }
  if (stmt->whle) {
    free_while(stmt->whle);
    free(stmt->whle);
    stmt->whle = NULL;
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
  if (stmt->asm_block) {
    free_asm_block(stmt->asm_block);
    free(stmt->asm_block);
    stmt->asm_block = NULL;
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

void show_asm_block(asm_block_t* asm_block, int level) {
  tabs(level - 1); printf("\\_ AsmBlock\n");
  if (!asm_block) {
    tabs(level + 1); printf("\\_ NULL\n");
    return;
  }
  tabs(level); printf("\\_ asm_type: %s\n", token_type_stringify(asm_block->asm_type));
  tabs(level); printf("\\_ begin: %p\n", asm_block->asm_source_code_begin);
  tabs(level); printf("\\_ end: %p\n", asm_block->asm_source_code_end);
  tabs(level); printf("\\_ length: %zu\n", asm_block->asm_source_code_end - asm_block->asm_source_code_begin);
}

void show_string_lit(string_lit_t* str_lit, int level) {
  tabs(level - 1); printf("\\_ StringLit\n");
  if (!str_lit) {
    tabs(level + 1); printf("\\_ NULL\n");
    return;
  }
  size_t length = str_lit->str_lit_end - str_lit->str_lit_begin;
  tabs(level); printf("\\_ begin: %p\n", str_lit->str_lit_begin);
  tabs(level); printf("\\_ end: %p\n", str_lit->str_lit_end);
  tabs(level); printf("\\_ length: %zu\n", length);
  tabs(level); printf("\\_ str: %*.s\n", (int)length, str_lit->str_lit_begin);
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
  tabs(level); printf("\\_ name: %s\n", func_call->name);
  show_arg_list(func_call->args, level + 1);
}

void show_assign(assign_t* assign, int level) {
  tabs(level - 1); printf("\\_ Assign\n");
  if (!assign) {
    tabs(level); printf("\\_ NULL\n");
    return;
  }
  if (assign->type & ASSIGN_LHS_VAR) {
    show_var(assign->left_hand_side.var, level);
  }
  if (assign->type & ASSIGN_LHS_ID) {
    tabs(level); printf("\\_ ID: %s\n", assign->left_hand_side.id);
  }
  if (assign->type & ASSIGN_RHS_STR_LIT) {
    show_string_lit(assign->right_hand_side.str_lit, level + 1);
    // nothing to do
  }
  if (assign->type & ASSIGN_RHS_EXPR) {
    show_expression(assign->right_hand_side.expr, level);
  }
  /*
  show_var(assign->var, level);
  switch (assign->type) {
  case ASSIGN_EXPR:
    show_expression(assign->value.expr, level + 1);
    break;
  case ASSIGN_STR_LIT:
    show_string_lit(assign->value.str_lit, level + 1);
    break;
  default:
    break;
  }
  */
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

void show_arg_list(arg_list_t* arg_list, int level) {
  tabs(level); printf("\\_ ArgList\n");
  if (!arg_list) {
    tabs(level+1); printf("\\_ NULL\n");
    return;
  }
  for (int i = 0; i < arg_list->arg_count; i++) {
    show_expression(arg_list->args[i], level + 1);
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
        case T_MOD:   printf("{%%}\n"); break;
        case T_DEQ:   printf("{==}\n"); break;
        case T_LTEQ:  printf("{<=}\n"); break;
        case T_GTEQ:  printf("{>=}\n"); break;
        case T_NEQ:   printf("{!=}\n"); break;
        case T_GT:    printf("{>}\n"); break;
        case T_LT:    printf("{<}\n"); break;
        case T_DOR:   printf("{||}\n"); break;
        case T_DAND:  printf("{&&}\n"); break;
        case T_OR:    printf("{|}\n"); break;
        case T_AND:   printf("{|}\n"); break;
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
  show_expression(iff->condition, level + 1);
  show_block(iff->block, level + 1);
}

void show_while(while_t* whle, int level) {
  tabs(level); printf("\\_ While\n");
  if (!whle) {
    tabs(level); printf("\\_ NULL\n");
    return;
  }
  show_expression(whle->condition, level + 1);
  show_block(whle->block, level + 1);
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
  else if (stmt->whle) {
    show_while(stmt->whle, level + 1);
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
  else if (stmt->asm_block) {
    show_asm_block(stmt->asm_block, level + 2);
  }
  else {
    tabs(level); printf("\\_ %p\n", stmt->ret);
    tabs(level); printf("\\_ Statement empty\n");
  }
}
