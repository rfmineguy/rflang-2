#include "expression_validation.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define STATE_NEUTRAL 0
#define STATE_FUNC    1

/*
int expr_validate(const char* expr) {
  int status = 1;
  int lp_depth = 0;
  int func_depth = 0;
  int state = STATE_NEUTRAL;
  for (int i = 0; i < strlen(expr); i++) {
    if (is_id_or_num(expr[i]) || expr[i] == ' ' || expr[i] == '\t') {
      continue;
    }
    else if (is_operator(expr[i])) {
      int prev_is_valid = is_id_or_num(expr[i-1]) || expr[i-1] == ')';
      int post_is_valid = is_id_or_num(expr[i+1]) || expr[i+1] == '(';
      if (!prev_is_valid || !post_is_valid) {
        status = 0;
        fprintf(stderr, "Unexpected operator [%d] %c\n", i, expr[i]);
      }
    }
    else if (expr[i] == '(') {
      if (is_id(expr[i - 1])) {
        state = STATE_FUNC;
        func_depth ++;
      }
      if (isdigit(expr[i - 1])) {
        status = 0;
        fprintf(stderr, "Unexpected number [%d] %c\n", i, expr[i - 1]);
      }
      lp_depth ++;
    }
    else if (expr[i] == ')') {
      if (lp_depth <= 0) {
        status = 0;
        fprintf(stderr, "Stray [%d] ')'\n", i);
      }
      if (state == STATE_FUNC) {
        func_depth --;
      }
      if (func_depth == 0) {
        state = STATE_NEUTRAL;
      }
      lp_depth --;
    }
    else if (expr[i] == ',') {
      if (state != STATE_FUNC) {
        status = 0;
        fprintf(stderr, "Invalid use of [%d] ','\n", i);
      }
      else {
        int prev_is_valid = is_id_or_num(expr[i-1]) || expr[i-1] == ')';
        int post_is_valid = is_id_or_num(expr[i+1]) || expr[i+1] == '(';
        if (!prev_is_valid || !post_is_valid) {
          status = 0;
          fprintf(stderr, "Unexpected operator [%d] %c\n", i, expr[i]);
        }
      }
    }
    else {
      status = 0;
      fprintf(stderr, "Unexpected token [%d] %c\n", i, expr[i]);
    }
  }
  if (lp_depth != 0) {
    status = 0;
    fprintf(stderr, "Unmatched '('\n");
  }
  return status;
}
*/

int validate_expression(const tokenizer3_t* t, error_context_t* ctx, int* out_expr_length) {
  tokenizer3_t copy = *t; // copy the tokenizer
  int status = 1;
  int lp_depth = 0;
  int func_depth = 0;
  int length = 0;
  int state = STATE_NEUTRAL;
  token_t prev_token, curr_token, next_token;
  while (1) {
    prev_token = tokenizer3_get(&copy, 1);
    curr_token = tokenizer3_get(&copy, 2);
    next_token = tokenizer3_get(&copy, 3);
    token_type_t prev_token_type = prev_token.type;
    token_type_t curr_token_type = curr_token.type;
    token_type_t next_token_type = next_token.type;

    int is_operator    = curr_token_type == T_PLUS
                      || curr_token_type == T_MINUS
                      || curr_token_type == T_MUL
                      || curr_token_type == T_DIV
                      || curr_token_type == T_MOD;
    
    // NOTE: tokenizer doesn't give whitespace, but for completeness we keep it.. lol
    if (curr_token_type == T_ID || curr_token_type == T_NUM || curr_token_type == T_SPC || curr_token_type == T_TAB) {
      tokenizer3_advance(&copy);
      length++;
      continue;
    }
    else if (is_operator) {
      int prev_is_valid = prev_token_type == T_ID || prev_token_type == T_NUM || prev_token_type == T_RP;
      int post_is_valid = next_token_type == T_ID || next_token_type == T_NUM || next_token_type == T_LP;
      if (!prev_is_valid || !post_is_valid) {
        status = 0;
        // NOTE: Unexpected operator
        error_push(ctx, error_new(E_EXPR_UNEXPECTED_OPERATOR, curr_token));
      }
    }
    else if (curr_token_type == T_LP) {
      if (prev_token_type == T_ID) {
        state = STATE_FUNC;
        func_depth ++;
      }
      if (prev_token_type == T_NUM) {
        status = 0;
        // NOTE: Unexpected number
        error_push(ctx, error_new(E_EXPR_UNEXPECTED_NUMBER, prev_token));
      }
      lp_depth++;
    }
    else if (curr_token_type == T_RP) {
      if (lp_depth <= 0) {
        // NOTE: Stray ')'
        status = 0;
        error_push(ctx, error_new(E_EXPR_STRAY_RP, curr_token));
      }
      if (state == STATE_FUNC) {
        func_depth --;
      }
      if (func_depth == 0) {
        state = STATE_NEUTRAL;
      }
      lp_depth --;
    }
    else if (curr_token_type == T_COMMA) {
      if (state != STATE_FUNC) {
        status = 0;
        //NOTE: Invalid use of COMMA
        error_push(ctx, error_new(E_EXPR_INVALID_COMMA_USE, curr_token));
      }
      else {
        int prev_is_valid = prev_token_type == T_ID || prev_token_type == T_NUM || prev_token_type == T_RP;
        int post_is_valid = next_token_type == T_ID || next_token_type == T_NUM || next_token_type == T_LP;
        if (!prev_is_valid || !post_is_valid) {
          status = 0;
          //NOTE: Unexpected token
          error_push(ctx, error_new(E_EXPR_UNEXPECTED_TOKEN, curr_token));
        }
      }
    }
    else {
      break; // we are done
      
      
      // NOTE: Unexpected token.
      status = 0;
      error_push(ctx, error_new(E_EXPR_UNEXPECTED_TOKEN, curr_token));
    }
    tokenizer3_advance(&copy);
    length++;
  }
  if (lp_depth != 0) {
    // NOTE: Unmatched '('
    status = 0;
    error_push(ctx, error_new(E_MISMATCHED_LP, curr_token));
  }
  *out_expr_length = length;
  return status;
}
