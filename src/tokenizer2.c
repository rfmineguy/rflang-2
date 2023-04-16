#include "tokenizer2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

// MACROS
#define LOC_FIELD(tokenizer, len) \
  .loc = (token_loc_t) {\
    .begin_index = t->cursor - t->source_str,\
    .length = len,\
    .line = t->line,\
    .column = t->col } 

#define TOK_CHECK_CH(ch, tp) \
  if (*t->cursor == ch) {\
    t->current = (token_t) {.type = tp, LOC_FIELD(t, 1) };\
    t->cursor++;\
    return;\
  }

#define TOK_CHECK_STR(str, len, tp) \
  if (strncmp(t->cursor, str, len) == 0) {\
    t->current = (token_t) {.type = tp, LOC_FIELD(t, len) };\
    t->cursor+=len;\
    return;\
  }

#define TOK_CHECK_EOF(tok) \
  if (t->cursor == t->source_str + t->source_length) {\
    t->current = (token_t) {.type = T_EOF, LOC_FIELD(t, 1) };\
    t->cursor++;\
    return;\
  }

int tokenizer_process_digit(tokenizer_t*, int*);
void tokenizer_process_id(tokenizer_t*, int*);

// FUNCTIONS
// 
tokenizer_t* tokenizer_new(FILE* f) {
  tokenizer_t* t = calloc(1, sizeof(tokenizer_t));

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  t->source_length = size;
  t->source_str = malloc(sizeof(char) * size);
  if (!t->source_str) {
    fprintf(stderr, "Failed to allocate memory\n");
    free(t->source_str);
    t->source_str = NULL;
    exit(1);
  }

  size_t read = 0;
  if ((read = fread(t->source_str, 1, size, f)) != size) {
    fprintf(stderr, "Failed to read entire file. Only read %lu bytes\n", read); 
    free(t->source_str);
    t->source_str = NULL;
    exit(1);
  }
  printf("%lu\n", read);

  t->cursor = t->source_str;
  tokenizer_advance_t(t);
  return t;
}

void tokenizer_free(tokenizer_t* t) {
  free(t->source_str);
  t->source_str = NULL;
}

int tokenizer_expect_t(tokenizer_t* t, token_type_t type) {
  return t->current.type == type;
}

void tokenizer_consume_whitespace(tokenizer_t* t) {
  if (*t->cursor == '\n') {
    t->cursor++;
    t->line++;
    t->col = 0;
    t->current = (token_t) {.type = T_NL, LOC_FIELD(t, 1) };
  }
  while (*t->cursor == ' ' || *t->cursor == '\t') {
    t->cursor++;
  }
}

void tokenizer_advance_t_internal(tokenizer_t* t) {
  
}

void tokenizer_advance_t(tokenizer_t* t) {
  tokenizer_consume_whitespace(t);
  // if (*t->cursor == '\n') {
  //   t->cursor++;
  //   t->line++;
  //   t->col = 0;
  //   t->current = (token_t) {.type = T_NL, LOC_FIELD(t, 1) };
  //   return;
  // }
  TOK_CHECK_STR("fn", 2, T_FN)
  TOK_CHECK_STR("if", 2, T_IF)
  TOK_CHECK_STR("for", 3, T_FOR)
  TOK_CHECK_STR("while", 5, T_WHILE)
  TOK_CHECK_STR("asm", 3, T_ASM)
  TOK_CHECK_STR("int", 3, T_INT)
  TOK_CHECK_STR("short", 5, T_SHT)
  TOK_CHECK_STR("char", 4, T_CHR)
  TOK_CHECK_STR("double", 6, T_DBL)
  TOK_CHECK_STR("float", 5, T_FLT)
  TOK_CHECK_STR("bool", 4, T_BOOL)
  TOK_CHECK_STR("use", 3, T_USE)
  TOK_CHECK_STR("return", 6, T_RETURN)
  TOK_CHECK_STR("->", 2, T_ARROW)
  TOK_CHECK_CH(' ', T_SPC)
  TOK_CHECK_CH('\t', T_TAB)
  TOK_CHECK_CH('>', T_GT)
  TOK_CHECK_CH('<', T_LT)
  TOK_CHECK_CH('(', T_LP)
  TOK_CHECK_CH(')', T_RP)
  TOK_CHECK_CH('{', T_LB)
  TOK_CHECK_CH('}', T_RB)
  TOK_CHECK_CH(';', T_SEMICOLON)
  TOK_CHECK_CH(':', T_COLON)
  TOK_CHECK_CH(',', T_COMMA)
  TOK_CHECK_CH('=', T_EQ)
  TOK_CHECK_CH('+', T_PLUS)
  TOK_CHECK_CH('-', T_MINUS)
  TOK_CHECK_CH('*', T_MUL)
  TOK_CHECK_CH('/', T_DIV)
  TOK_CHECK_CH('%', T_MOD)
  TOK_CHECK_CH('\'', T_SQT)
  TOK_CHECK_CH('\"', T_DQT)
  TOK_CHECK_CH('\\', T_BACKSLASH)
  TOK_CHECK_EOF(t)
  if (isdigit(*t->cursor)) {
    int digit_len = 0;
    tokenizer_process_digit(t, &digit_len);
    t->current = (token_t) {.type = T_INT, LOC_FIELD(t, digit_len)}; 
    t->cursor += digit_len;
    return;
  }
  if (isalpha(*t->cursor)) {
    int length = 0;
    tokenizer_process_id(t, &length);
    t->current = (token_t) {.type = T_ID, LOC_FIELD(t, length)};
    t->cursor += length;
    return;
  }
  // printf("missed everything\n");
}

token_t tokenizer_get_t(tokenizer_t* t) {
  return t->current;
}

void tokenizer_show_next_t(tokenizer_t* t) {
#if 1
  printf("%s\n", token_type_stringify(tokenizer_get_t(t).type));
#else
  printf(".");
#endif
}

// processing
int tokenizer_process_digit(tokenizer_t* tokenizer, int* digit_length) {
  int value = 0, digits = 0;
  char* temp_curs = tokenizer->cursor;
  while (isdigit(*temp_curs) != 0) {
    value += ((*temp_curs) - '0') * pow(10, digits);
    digits++;
    temp_curs++;
  }
  *digit_length = digits;
  return value;
}

void tokenizer_process_id(tokenizer_t* tokenizer, int* id_length) {
  int length = 0;
  char* temp_curs = tokenizer->cursor;
  while (isalpha(*temp_curs) != 0) {
    temp_curs++;
    length++;
  }
  *id_length = length;
}

