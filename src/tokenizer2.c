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
    t->col++;\
    return;\
  }

#define TOK_CHECK_STR(str, len, tp) \
  if (strncmp(t->cursor, str, len) == 0) {\
    t->current = (token_t) {.type = tp, LOC_FIELD(t, len) };\
    t->cursor+=len;\
    t->col+=len;\
    return;\
  }

#define TOK_CHECK_EOF(tok) \
  if (t->cursor == t->source_str + t->source_length) {\
    t->current = (token_t) {.type = T_EOF, LOC_FIELD(t, 1) };\
    t->cursor++;\
    t->col++;\
    return;\
  }

int tokenizer_process_digit(tokenizer_t*, int*);
void tokenizer_process_id(tokenizer_t*, int*);

// FUNCTIONS
tokenizer_t* tokenizer_new_from_str(const char* str) {
  tokenizer_t* t = calloc(1, sizeof(tokenizer_t));
  int len = strlen(str) + 1;
  t->source_str = malloc(len);
  strcpy(t->source_str, str);
  t->source_str[len - 1] = 0;
  t->cursor = t->source_str;
  tokenizer_advance_t(t);
  return t;
}

tokenizer_t* tokenizer_new_from_file(FILE* f) {
  tokenizer_t* t = calloc(1, sizeof(tokenizer_t));

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  fseek(f, 0, SEEK_SET);

  t->source_length = size;
  t->source_str = malloc(sizeof(char) * size + 2);
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
  t->source_str[read] = '\0'; //add null term (fread doesn't add this)
  printf("%lu\n", read);
  printf("%p\n", t->source_str);

  t->cursor = t->source_str;
#if 1
  printf("%s\n", t->source_str);
#endif
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
  if (strncmp(t->cursor, "//", 2) == 0) {
    while (*(t->cursor + 1) != '\n') {
      t->cursor++;
    }
  }
}

void tokenizer_advance_t_internal(tokenizer_t* t) {
  
}

void tokenizer_advance_t(tokenizer_t* t) {
  t->prev = t->current;
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
  TOK_CHECK_STR("x86_32-linux", 12, T_X86_32_LINUX)
  TOK_CHECK_STR("x86_64-linux", 12, T_X86_64_LINUX)
  TOK_CHECK_STR("arm64", 5, T_ARM64)
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
    int value = tokenizer_process_digit(t, &digit_len);
    t->current = (token_t) {.type = T_NUM, LOC_FIELD(t, digit_len)}; 
    t->cursor += digit_len;
    t->col += digit_len;
    t->current.value.i = value;
    return;
  }
  if (*t->cursor == '_' || isalpha(*t->cursor)) {
    int length = 0;
    tokenizer_process_id(t, &length);
    t->current = (token_t) {.type = T_ID, LOC_FIELD(t, length)};
    strncpy(t->current.value.s, t->cursor, length);
    t->cursor += length;
    t->col += length;
    return;
  }
  t->current = (token_t) {.type = T_UNKNOWN, LOC_FIELD(t, 1)};
  t->cursor += 1;
  t->col += 1;
  // printf("missed everything\n");
}

void tokenizer_rewind_t(tokenizer_t* t) {
  t->cursor -= t->current.loc.length;
  t->col -= t->current.loc.length;
  t->cursor -= t->prev.loc.length;
  t->col -= t->prev.loc.length;
  tokenizer_advance_t(t);
}

token_t tokenizer_get_t(tokenizer_t* t) {
  return t->current;
}

void tokenizer_show_next_t(tokenizer_t* t) {
#if 1
  token_t token = tokenizer_get_t(t);
  printf("%s", token_type_stringify(token.type));
  switch (token.type) {
  case T_NUM: printf("%.*s", token.loc.length, t->source_str + token.loc.begin_index); break;
  default:    printf("..."); break;
  }
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
  while (*temp_curs == '_' || isalpha(*temp_curs) != 0) {
    temp_curs++;
    length++;
  }
  *id_length = length;
}

void token_print(token_t t, tokenizer_t* tokenizer) {
  /*#define PRINT_TOKEN(type_s, t) \
    printf("%10s,  begin: %3d, len: %3d, line: %3d, col: %3d, text: %.*s", type_s, t.loc.begin_index, t.loc.length, t.loc.line, t.loc.column, t.loc.length, tokenizer->source_str + t.loc.begin_index);
    */
#define PRINT_TOKEN(type_s, t) \
    printf("{%7s, %.*s}\n", type_s, t.loc.length, tokenizer->source_str + t.loc.begin_index);
  switch (t.type) {
    case T_NL:      /*PRINT_TOKEN("NL", t);*/                 break;
    case T_SPC:           PRINT_TOKEN("SPC", t);              break;
    case T_TAB:           PRINT_TOKEN("TAB", t);              break;
    case T_FN:            PRINT_TOKEN("FN", t);               break;
    case T_IF:            PRINT_TOKEN("IF", t);               break;
    case T_FOR:           PRINT_TOKEN("FOR", t);              break;
    case T_WHILE:         PRINT_TOKEN("WHILE", t);            break;
    case T_ASM:           PRINT_TOKEN("ASM", t);              break;
    case T_RB:            PRINT_TOKEN("RB", t);               break;
    case T_LB:            PRINT_TOKEN("LB", t);               break;
    case T_RP:            PRINT_TOKEN("RP", t);               break;
    case T_LP:            PRINT_TOKEN("LP", t);               break;
    case T_EQ:            PRINT_TOKEN("EQ", t);               break;
    case T_NOT:           PRINT_TOKEN("NOT", t);              break;
    case T_COLON:         PRINT_TOKEN("COLON", t);            break;
    case T_SEMICOLON:     PRINT_TOKEN("SEMICOLON", t);        break;
    case T_COMMA:         PRINT_TOKEN("COMMA", t);            break;
    case T_ARROW:         PRINT_TOKEN("ARROW", t);            break;
    case T_INT:           PRINT_TOKEN("INT", t);              break;
    case T_CHR:           PRINT_TOKEN("CHR", t);              break;
    case T_SHT:           PRINT_TOKEN("SHT", t);              break;
    case T_DBL:           PRINT_TOKEN("DBL", t);              break;
    case T_FLT:           PRINT_TOKEN("FLT", t);              break;
    case T_BOOL:          PRINT_TOKEN("BOOL", t);             break;
    case T_GT:            PRINT_TOKEN("GT", t);               break;
    case T_LT:            PRINT_TOKEN("LT", t);               break;
    case T_SQT:           PRINT_TOKEN("SQT", t);              break;
    case T_DQT:           PRINT_TOKEN("DQT", t);              break;
    case T_BACKSLASH:     PRINT_TOKEN("BACKSLASH", t);        break;
    case T_PLUS:          PRINT_TOKEN("PLUS", t);             break;
    case T_MINUS:         PRINT_TOKEN("MINUS", t);            break;
    case T_MUL:           PRINT_TOKEN("MUL", t);              break;
    case T_DIV:           PRINT_TOKEN("DIV", t);              break;
    case T_NUM:           PRINT_TOKEN("NUM", t);              break;
    case T_ID:            PRINT_TOKEN("ID", t);               break;
    case T_X86_32_LINUX:  PRINT_TOKEN("X86_32_ASM_LINUX", t); break;
    case T_X86_64_LINUX:  PRINT_TOKEN("X86_64_ASM_LINUX", t); break;
    case T_ARM64:         PRINT_TOKEN("ARM64_ASM", t);        break;
    case T_EOF:           PRINT_TOKEN("EOF", t);              break;
    default:              printf("unimplemented token\n");    break;
  }
}
