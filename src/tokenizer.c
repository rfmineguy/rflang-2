#include "tokenizer.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

//
//MACROS
//
#define TOK_CHECK_CH(ch, tp) \
  if (*tokenizer->cursor == ch) {\
    return (token_t) {.type = tp, LOC_FIELD(tokenizer, 1) };\
  }

#define TOK_CHECK_STR(str, len, tp) \
  if (strncmp(tokenizer->cursor, str, len) == 0) {\
    return (token_t) {.type = tp, LOC_FIELD(tokenizer, len) };\
  }


//
//FUNCTIONS
//
tokenizer_t tokenizer_new(const char* file) {
  tokenizer_t t = {0};
  FILE* f = fopen(file, "r");
  if (!f) {
    fprintf(stderr, "Could not open [%s]\n", file);
    exit(1);
  }
  else {
    printf("Opened file [%s]\n", file);
  }
  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);
  t.source.contents = malloc(size);
  int read_byte_count = fread(t.source.contents, 1, size, f);
  if (read_byte_count != size) {
    fprintf(stderr, "Some problem reading file\n");
  }
  t.source.length = size;
  t.source.contents_end = t.source.contents + size;
  t.cursor = t.source.contents;
  t.line_number = 1;
  fclose(f);

  return t;
}

void tokenizer_free(tokenizer_t tokenizer) {
  free(tokenizer.source.contents);
  tokenizer.source.contents = NULL;
}

token_t tokenizer_peek_t(tokenizer_t* tokenizer) {
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
  TOK_CHECK_CH('\n', T_NL);
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

  // Tokenize a number token
  if (isdigit(*tokenizer->cursor)) {
    int digit_length = 0;
    int val = tokenizer_process_digit(tokenizer, &digit_length);
    return (token_t) {.type = T_NUM, LOC_FIELD(tokenizer, digit_length) };
  }
  // Tokenize an id token
  if (isalpha(*tokenizer->cursor)) {
    int length = 0;
    tokenizer_process_id(tokenizer, &length);
    return (token_t) {.type = T_ID, LOC_FIELD(tokenizer, length) };
  }
  // Tokenize the end of the file
  if (tokenizer->cursor == tokenizer->source.contents_end) {
    return (token_t) {.type = T_EOF, LOC_FIELD(tokenizer, 1) };
  }
  return (token_t) {.type = T_UNKNOWN, LOC_FIELD(tokenizer, 1) };
}

token_t tokenizer_next_t(tokenizer_t* tokenizer) {
  token_t token = tokenizer_peek_t(tokenizer);
  if (token.type == T_NL) {
    tokenizer->column_number = 0;
    tokenizer_advance(tokenizer, 1);
  }
  else {
    tokenizer_advance(tokenizer, token.loc.length);
  }
  // token_print(token, tokenizer);
  return token;
}

int tokenizer_consume_spaces(tokenizer_t* tokenizer) {
  int space_count = 0;
  token_t t;
  while ((t = tokenizer_peek_t(tokenizer)).type == T_SPC) {
    tokenizer_advance(tokenizer, 1);
    space_count ++;
  }
  while ((t = tokenizer_peek_t(tokenizer)).type == T_NL) {
    tokenizer_advance(tokenizer, 1);
    space_count ++;
  }
  return space_count;
}

void tokenizer_advance(tokenizer_t* tokenizer, int amount) {
  tokenizer->column_number += amount;
  tokenizer->cursor += amount;
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

void token_print(token_t t, tokenizer_t* tokenizer) {
  #define print_token(type_s, t) \
    printf("%10s,  begin: %3d, len: %3d, line: %3d, col: %3d, text: %.*s" "\n", type_s, t.loc.begin_index, t.loc.length, t.loc.line, t.loc.column, t.loc.length, tokenizer->source.contents + t.loc.begin_index);
  switch (t.type) {
    case t_nl:      /*print_token("nl", t);*/      break;
    case t_spc:       print_token("spc", t);       break;
    case t_tab:       print_token("tab", t);       break;
    case t_fn:        print_token("fn", t);        break;
    case t_if:        print_token("if", t);        break;
    case t_for:       print_token("for", t);       break;
    case t_while:     print_token("while", t);     break;
    case t_asm:       print_token("asm", t);       break;
    case t_rb:        print_token("rb", t);        break;
    case t_lb:        print_token("lb", t);        break;
    case t_rp:        print_token("rp", t);        break;
    case t_lp:        print_token("lp", t);        break;
    case t_eq:        print_token("eq", t);        break;
    case t_not:       print_token("not", t);       break;
    case t_colon:     print_token("colon", t);     break;
    case t_semicolon: print_token("semicolon", t); break;
    case t_comma:     print_token("comma", t);     break;
    case t_arrow:     print_token("arrow", t);     break;
    case t_int:       print_token("int", t);       break;
    case t_chr:       print_token("chr", t);       break;
    case t_sht:       print_token("sht", t);       break;
    case t_dbl:       print_token("dbl", t);       break;
    case t_flt:       print_token("flt", t);       break;
    case t_bool:      print_token("bool", t);      break;
    case t_gt:        print_token("gt", t);        break;
    case t_lt:        print_token("lt", t);        break;
    case t_sqt:       print_token("sqt", t);       break;
    case t_dqt:       print_token("dqt", t);       break;
    case t_backslash: print_token("backslash", t); break;
    case t_plus:      print_token("plus", t);      break;
    case t_minus:     print_token("minus", t);     break;
    case t_mul:       print_token("mul", t);       break;
    case t_div:       print_token("div", t);       break;
    case t_num:       print_token("num", t);       break;
    case t_id:        print_token("id", t);        break;
    case t_eof:       print_token("eof", t);       break;
    default:          printf("unimplemented token\n"); break;
  }
}

