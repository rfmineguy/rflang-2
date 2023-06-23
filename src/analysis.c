#include "analysis.h"
#include <string.h>

int analyze_module(module_t* module) {
  analyze_context_t ctx = {0};
  ctx.ht = chaining_ht_str_var_alloc(10);
  
  printf("... analyze_module\n");
  for (int i = 0; i < module->use_list_count; i++) {
    analyze_use(module->use_list[i], &ctx);
  }
  for (int i = 0; i < module->func_list_count; i++) {
    analyze_func(module->func_list[i], &ctx);
  }

  chaining_ht_str_var_show(ctx.ht, 0);
  chaining_ht_str_var_free(ctx.ht);
  return 1; 
}

int analyze_use(use_t* use, analyze_context_t* ctx) {
  // TODO_: maybe later we can analyze the use to get context from that file?
  return 1; 
}

int analyze_block(block_t* block, analyze_context_t* ctx) {
  ctx->scope_number++;
  ctx->scope_depth++;

  for (int i = 0; i < block->statement_count; i++) {
    analyze_statement(block->statements[i], ctx);
  }

  ctx->scope_depth--;
  return 1;
}

int analyze_func(func_t* func, analyze_context_t* ctx) {
  int decl_status = analyze_func_decl(func->decl, ctx);
  int block_status = analyze_block(func->block, ctx);
  return decl_status;
}

int analyze_func_decl(func_decl_t* func_decl, analyze_context_t* ctx) {
  entry_var d = {0};
  strncpy(d.key, func_decl->name, 100);
  d.scope_depth = ctx->scope_depth;
  d.scope_number = ctx->scope_number;
  d.type = 1; // FUNC
  chaining_ht_str_var_put(ctx->ht, func_decl->name, d);
  
  int params_status = analyze_param_list(func_decl->params, ctx);

  return 1;
}

int analyze_param_list(param_list_t* params, analyze_context_t* ctx) {
  for (int i = 0; i < params->params_count; i++) {
    int var_status = analyze_var(params->params[i], ctx);
  }
  return 1;
}

int analyze_var(var_t* var, analyze_context_t* ctx) {
  entry_var d = {0};
  strncpy(d.key, var->name, 100);
  d.scope_depth = ctx->scope_depth;
  d.scope_number = ctx->scope_number;
  d.type = 2; // VAR
  chaining_ht_str_var_put(ctx->ht, var->name, d);
  return 1;
}

int analyze_string_lit(string_lit_t* str_lit, analyze_context_t* ctx) {
  // not much to do here
  return 1;
}

int analyze_statement(statement_t* stmt, analyze_context_t* ctx) {
  if (stmt->iff) {
    analyze_iff(stmt->iff, ctx);
  }
  if (stmt->ret) {
    analyze_return(stmt->ret, ctx);
  }
  if (stmt->func_call_expr) {
    analyze_expression(stmt->func_call_expr, ctx, 0);
  }
  if (stmt->assign) {
    analyze_assign(stmt->assign, ctx);
  }
  if (stmt->asm_block) {
    analyze_asm_block(stmt->asm_block, ctx);
  }
  if (stmt->whle) {
    analyze_while(stmt->whle, ctx);
  }
  return 1;  
}

int analyze_iff(if_t* iff, analyze_context_t* ctx) {
  int condition_status = analyze_expression(iff->condition, ctx, 0);
  int block_status = analyze_block(iff->block, ctx);
  return 1;  
}

int analyze_return(return_t* ret, analyze_context_t* ctx) {
  int expression_status = analyze_expression(ret->expr, ctx, 0);
  return 1;  
}

int analyze_func_call(expression_t* func_call_expr, analyze_context_t* ctx, int depth) {
  func_call_t* func_call = func_call_expr->value.func_call;
  if (!chaining_ht_str_var_contains(ctx->ht, func_call->name)) {
    fprintf(stderr, "%s : '%s' not defined\n", __func__, func_call->name);
  }
  else {
    for (int i = 0; i < depth; i++) {
      printf("| ");
    }
    printf("============== ANALYZE FUNC_CALL ===============\n");
    printf("  name/id : '%s'\n", func_call->name);
    printf("  args : ");
    arg_list_t* args = func_call->args;
    for (int i = 0; i < args->arg_count; i++) {
      analyze_expression(args->args[i], ctx, depth + 1);
    }
  }
  return 1;  
}

int analyze_assign(assign_t* assign, analyze_context_t* ctx) {
  printf("analyze_assign\n");
  if (assign->type & ASSIGN_LHS_VAR) {
    int lhs_var_status = analyze_var(assign->left_hand_side.var, ctx);
  }
  if (assign->type & ASSIGN_LHS_ID) {
    int lhs_id_status = 1; // trivial to analyze an ID (string literal)
    // check if the id is in the HashTable. if not its illegal code
    if (!chaining_ht_str_var_contains(ctx->ht, assign->left_hand_side.id)) {
      fprintf(stderr, "'%s' not defined\n", assign->left_hand_side.id);
    }
    else {
      entry_var data = chaining_ht_str_var_find(ctx->ht, assign->left_hand_side.id);
      if (data.scope_number != ctx->scope_number || data.scope_depth < ctx->scope_depth) {
        fprintf(stderr, "'%s' out of scope\n", assign->left_hand_side.id);
      }
    }
  }
  if (assign->type & ASSIGN_RHS_STR_LIT) {
    int rhs_str_status = analyze_string_lit(assign->right_hand_side.str_lit, ctx);
  }
  if (assign->type & ASSIGN_LHS_ID) {
    int rhs_expr_status = analyze_expression(assign->right_hand_side.expr, ctx, 0);
  }
  return 1;  
}

int analyze_asm_block(asm_block_t* asm_block, analyze_context_t* ctx) {
  // not much to do here.. TODO: yet
  return 1;  
}

int analyze_while(while_t* whle, analyze_context_t* ctx) {

  return 1;  
}

int analyze_expression(expression_t* expr, analyze_context_t* ctx, int depth) {
  // figure out whether the expression is a constant expression recursively
  // NOTE: Its contstant if there is no identifiers in it
  for (int i = 0; i < depth; i++) {
    printf("| ");
  }
  switch (expr->type) {
    case EXPR_NUM: {
      // printf("expression is NUM literal type\n");
      printf("NUM - value = %d\n", expr->value.i);
      break;
    }
    case EXPR_STRING: {
      // printf("expression is ID type\n");
      printf("ID - value = %s\n", expr->value.s);
      break;
    }
    case EXPR_FUNC_CALL: {
      printf("expression is FUNC_CALL type\n");
      analyze_func_call(expr, ctx, depth + 1);
      break;
    }
    case EXPR_COMPOUND: {
      printf("expression is COMPOUND type [operator = %s]\n", token_type_stringify(expr->value.operation));
      analyze_expression(expr->left, ctx, depth + 1);
      analyze_expression(expr->right, ctx, depth + 1);
      break;
    } 
  }
  return 1;
}
