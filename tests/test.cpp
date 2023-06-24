#include <gtest/gtest.h>

extern "C" {
#include "../src/parsers/parser3.h"
#include "../src/expression_validation.h"
}

#define SETUP(test_string, tokenizer_name, error_ctx_name) \
{\
  tokenizer3_t tokenizer_name = tokenizer3_new_from_str(test_string);\
  error_context_t error_ctx_name = error_context_new()\

#define FINISH() \
  tokenizer3_free(&t);\
  error_context_free(ctx);\
}\

TEST(tokenizing, general) {
  SETUP("({}-+kaf/%", t, ctx);

  token_type_t expected_token_stream[9] 
    = { T_LP, T_LB, T_RB, T_MINUS, T_PLUS, T_ID, T_DIV, T_MOD, T_EOF };
  int i = 0;

  while (!tokenizer3_expect_offset(&t, 2, T_EOF)) {
    if (i >= 9) {
      FAIL();
      return;
    }
    tokenizer3_advance(&t);
    EXPECT_EQ(tokenizer3_get(&t, 2).type, expected_token_stream[i]) << i;
    i++;
  }

  FINISH();
}

TEST(parsing, use) {
  SETUP("use io", t, ctx);

  tokenizer3_advance(&t);
  use_t* u = parse_use(&t, &ctx);

  EXPECT_STREQ(u->name, "io");

  free_use(u);
  free(u);

  FINISH();
}

TEST(parsing, var) {
  SETUP("a: int", t, ctx);

  tokenizer3_advance(&t);
  var_t* v = parse_var(&t, &ctx);

  ASSERT_EQ(v->type, T_INT);
  EXPECT_STREQ(v->name, "a");

  free_var(v);
  free(v);

  FINISH();
}

TEST(parsing, func_decl_1) {
  SETUP("main(): int", t, ctx);
  
  tokenizer3_advance(&t);
  func_decl_t* decl = parse_func_decl(&t, &ctx);

  EXPECT_STREQ(decl->name, "main");
  EXPECT_EQ(decl->params->params_count, 0);

  free_func_decl(decl);
  free(decl);

  FINISH();
}

TEST(parsing, func_decl_2) {
  SETUP("main(a: int): int", t, ctx);
  
  tokenizer3_advance(&t);
  func_decl_t* decl = parse_func_decl(&t, &ctx);

  EXPECT_STREQ(decl->name, "main");
  ASSERT_EQ(decl->params->params_count, 1);
  EXPECT_STREQ(decl->params->params[0]->name, "a");
  EXPECT_EQ(decl->params->params[0]->type, T_INT);

  free_func_decl(decl);
  free(decl);

  FINISH();
}

TEST(parsing, func_1_no_body) {
  SETUP("fn main(): int {};", t, ctx);

  tokenizer3_advance(&t);
  func_t* func = parse_func(&t, &ctx);

  free_func(func);
  free(func);

  FINISH();
}

TEST(parsing, var_assign_1) {
  SETUP("a: int = 4;", t, ctx);

  FINISH();
}

TEST(parsing, validate_expression_success_1) {
  SETUP("4 + 4 - (3 * 4) % 4", t, ctx);
  
  tokenizer3_advance(&t);
  int postfix_len = 0;
  EXPECT_TRUE(validate_expression(&t, &ctx, &postfix_len));

  FINISH();
}

TEST(parsing, validate_expression_success_w_func_call_1) {
  SETUP("4 + 4 - add(3 * 4) % 4", t, ctx);
  
  tokenizer3_advance(&t);
  int postfix_len = 0;
  ASSERT_TRUE(validate_expression(&t, &ctx, &postfix_len));
  
  // Root
  EXPECT_TRUE(e != NULL);
  EXPECT_TRUE(e->type == EXPR_COMPOUND);
  EXPECT_TRUE(e->value.operation == T_MOD);

  FINISH();
}

TEST(parsing, validate_expression_fail_w_func_call_1) {
  SETUP("4 + 4 - add(3 * 4 % 4", t, ctx);
  
  tokenizer3_advance(&t);
  int postfix_len = 0;
  EXPECT_FALSE(validate_expression(&t, &ctx, &postfix_len));

  FINISH();
}

TEST(expr_parsing, validate_expression_fail_1_missing_number) {
  SETUP("4 + 4 - (3 * 4) % ", t, ctx);

  tokenizer3_advance(&t);
  int postfix_len = 0;
  EXPECT_FALSE(validate_expression(&t, &ctx, &postfix_len));

  FINISH();
}

TEST(expr_parsing, validate_expression_fail_2_missing_lp) {
  SETUP("4 + 4 - 3 * 4) % 4", t, ctx);

  tokenizer3_advance(&t);
  int postfix_len = 0;
  EXPECT_FALSE(validate_expression(&t, &ctx, &postfix_len));

  FINISH();
}

TEST(expr_parsing, validate_expression_fail_2_missing_rp) {
  SETUP("4 + 4 - (3 * 4 % 4", t, ctx);

  tokenizer3_advance(&t);
  int postfix_len = 0;
  EXPECT_FALSE(validate_expression(&t, &ctx, &postfix_len));

  FINISH();
}
