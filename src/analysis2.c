#include "analysis2.h"
#include <stdio.h>

void analyze2_module(module_t* module, chaining_ht_str_symbol_t ht) {
  for (int i = 0; i < module->use_list_count; i++) {
    analyze2_use(module->use_list[i], ht);
  }
  for (int i = 0; i < module->func_list_count; i++) {
    analyze2_func(module->func_list[i], ht);
  }
}

void analyze2_use(use_t* use, chaining_ht_str_symbol_t ht) {
  // not much to analyze here
}

void analyze2_block(block_t* block, chaining_ht_str_symbol_t ht) {
  for (int i = 0; i < block->statement_count; i++) {
    analyze2_statement(block->statements[i], ht);
  }
}

void analyze2_asm_block(asm_block_t* asm_block, chaining_ht_str_symbol_t ht) {
  // not checking validity of assembly at all. not in the scope of the project
}

void analyze2_func(func_t* func, chaining_ht_str_symbol_t ht) {
  analyze2_func_decl(func->decl, ht);
  analyze2_block(func->block, ht);
}

void analyze2_func_decl(func_decl_t* func_decl, chaining_ht_str_symbol_t ht) {
  entry_symbol e = {0};
  e.type = SYM_FUNC;
  e.data.func_signature.param_count = func_decl->params->params_count;
  e.data.func_signature.return_type = (int) func_decl->return_type;
  if (chaining_ht_str_symbol_contains(ht, func_decl->name)) {
    fprintf(stderr, "Function symbol, '%s' already defined\n", func_decl->name);
    return;
  }
  chaining_ht_str_symbol_put(ht, func_decl->name, e);
}

void analyze2_func_call_expr(expression_t* func_call_expr, chaining_ht_str_symbol_t ht) {

}

void analyze2_var(var_t* var, chaining_ht_str_symbol_t ht) {

}

void analyze2_string_lit(string_lit_t* strlit, chaining_ht_str_symbol_t ht) {

}

void analyze2_assign(assign_t* assign, chaining_ht_str_symbol_t ht) {

}

void analyze2_return(return_t* ret, chaining_ht_str_symbol_t ht) {

}

void analyze2_param_list(param_list_t* parlist, chaining_ht_str_symbol_t ht) {

}

void analyze2_expression(expression_t* expr, chaining_ht_str_symbol_t ht) {
  switch (expr->type) {
    case EXPR_STRING: 
      // check if this variable is declared
      if (!chaining_ht_str_symbol_contains(ht, expr->value.s)) {
        fprintf(stderr, "Symbol not defined, '%s'\n", expr->value.s);        
      }
      break;
    case EXPR_NUM:
      // nothing to do
      break;
    case EXPR_FUNC_CALL:
      break;
    case EXPR_COMPOUND:
      analyze2_expression(expr->left, ht);
      analyze2_expression(expr->right, ht);
      break;
    default:
      break;
  }
}

void analyze2_if(if_t* iff, chaining_ht_str_symbol_t ht) {

}

void analyze2_while(while_t* whle, chaining_ht_str_symbol_t ht) {

}

void analyze2_statement(statement_t* stmt, chaining_ht_str_symbol_t ht) {
  if (stmt->iff) {
    analyze2_if(stmt->iff, ht);
  }
  if (stmt->ret) {
    analyze2_return(stmt->ret, ht);
  }
  if (stmt->func_call_expr) {
    analyze2_func_call_expr(stmt->func_call_expr, ht);
  }
  if (stmt->assign) {
    analyze2_assign(stmt->assign, ht);
  }
  if (stmt->asm_block) {
    analyze2_asm_block(stmt->asm_block, ht);
  }
  if (stmt->whle) {
    analyze2_while(stmt->whle, ht);
  }
}
