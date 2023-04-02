#include "token.h"
#include <stdio.h>

void rfc_token_print(token_t* p_token) {
  #define GENERIC_STRING_TEMP "t: %2d | line: %3d | col: %2d | pos: %3d | length: %2d"
  #define GENERIC_VARARGS     p_token->token_type, p_token->line, p_token->column, p_token->pos, p_token->length
  switch (p_token->token_type) {
    case T_NEWLINE: {
      printf("Token | c   = %7s | " GENERIC_STRING_TEMP " |\n", "\\n", GENERIC_VARARGS);
      break;
    }
    case T_TAB: {
      printf("Token | c   = %7s | " GENERIC_STRING_TEMP " |\n", "\\t", GENERIC_VARARGS);
      break;
    }
    case T_SPACE ... T_COMP_LT : {
      printf("Token | c   = %7c | " GENERIC_STRING_TEMP " |\n", p_token->data.char_token, GENERIC_VARARGS);
      break;
    }
    case T_STRING_LITERAL: {
      printf("Token | str = %7s | " GENERIC_STRING_TEMP " |\n", p_token->data.string_lit_token.str, GENERIC_VARARGS);
      break;
    }
    case T_NUMBER_LITERAL: {
      printf("Token | val = %7d | " GENERIC_STRING_TEMP " |\n", p_token->data.number_token.value, GENERIC_VARARGS);
      break;
    }
    case T_EOF: {
      printf("Token | val = %s | " GENERIC_STRING_TEMP " |\n", "EOF", GENERIC_VARARGS);
      break;
    }
    default: {
      printf("Token | str = %7s | " GENERIC_STRING_TEMP " |\n", p_token->data.string_lit_token.str, GENERIC_VARARGS);
      break;
    }
  }
}

void rfc_set_token_list(token_t* token_list, int token_count, token_t set_token) {
  for (int i = 0; i < token_count; i++) {
    token_list[i] = set_token;
  }
}

void rfc_display_token_list(token_t* token_list, int token_count) {
  const int IGNORE_SPACES = 1;
  for (int i = 0; i < token_count; i++) {
    int is_token_space = token_list[i].token_type == T_SPACE;
    int is_token_valid = token_list[i].valid;
    if (IGNORE_SPACES && is_token_space)
      continue;
    if (!is_token_valid)
      continue;
    rfc_token_print(&token_list[i]);
  }
}
