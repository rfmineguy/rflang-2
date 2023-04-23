#include <gtest/gtest.h>

extern "C" {
#include "../src/parser2.h"
}

TEST(test_module, zero_eq_zero) {
  ASSERT_EQ(0, 0);
}

TEST(parsing_mod, use) {
  const char* use_string = "use io";
  tokenizer_t* t = tokenizer_new_from_str(use_string);
  use_t* u = parse_use(t);
  tokenizer_free(t);
  ASSERT_TRUE(u) << "";
}

TEST(parsing_mod, func_decl) {
  const char* func_decl_string = "";
}
