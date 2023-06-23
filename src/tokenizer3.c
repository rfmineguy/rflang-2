#include "tokenizer3.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

//assume file is open
tokenizer3_t tokenizer3_new(const char* filename) {
  FILE* f = fopen(filename, "r");
  tokenizer3_t t = {0};
  if (!f) {
    fprintf(stderr, "Problem with file argument\n");
    return t;
  }

  // get length of the file
  fseek(f, 0, SEEK_END);
  t.source_length = ftell(f);
  fseek(f, 0, SEEK_SET);

  // setup initial values of tokenizer
  t.source_filename = filename;
  t.source_code = malloc(sizeof(char) * t.source_length + 1);
  fread(t.source_code, sizeof(char), t.source_length, f);
  t.source_code[t.source_length] = 0;
  t.cursor = t.source_code;
  t.line_start = t.source_code;

  for (int i = 0; i < 5; i++) {
    t.history[i].loc.line_start = t.source_code;
  }
  // make sure the history buffer starts off half full, so our center is at index 2
  for (int i = 0; i < 2; i++) {
    tokenizer3_advance(&t);
  }
  fclose(f);
  return t;
}

tokenizer3_t tokenizer3_new_from_str(const char* source_string) {
  tokenizer3_t t = {0};
  t.source_filename = "None";
  t.source_length = strlen(source_string);
  t.source_code = malloc(sizeof(char) * t.source_length);
  strncpy(t.source_code, source_string, sizeof(char) * t.source_length);
  t.source_code[t.source_length] = 0;
  t.cursor = t.source_code;
  t.line_start = t.source_code;

  for (int i = 0; i < 5; i++) {
    t.history[i].loc.line_start = t.source_code;
  }
  // make sure the history buffer starts off half full, so our center is at index 2
  for (int i = 0; i < 2; i++) {
    tokenizer3_advance(&t);
  }

  return t;
}

void tokenizer3_free(tokenizer3_t* t) {
  free(t->source_code);
  t->source_code = NULL;
}

void tokenizer3_consume_comments(tokenizer3_t* t) {
  // HANDLE MULTILINE COMMENTS
  if (strncmp(t->cursor, "*/", 2) == 0) {
    fprintf(stderr, "Ending comment block alone... skipping it\n");
    t->cursor += 2;
    if (*t->cursor == '\n') {
      t->cursor++;
      t->line_start = t->cursor;
    }
  }
  if (strncmp(t->cursor, "/*", 2) == 0) {
    // printf("Comment\n");
    t->cursor += 2;
    do {
      t->cursor++;
      if (*t->cursor == '\n') {
        t->line++;
        t->col = 0;
        t->line_start = t->cursor;
      }
      // printf("Comment skip loop %c\n", *t->cursor);
    } while (t->cursor < t->source_code + t->source_length && strncmp(t->cursor, "*/", 2) != 0);
    t->cursor += 2;

    if (*t->cursor == '\n') {
      t->line++;
      t->col = 0;
      t->cursor++;
      t->line_start = t->cursor;
    }
  }
  
  // HANDLE SINGLE LINE COMMENTS
  if (strncmp(t->cursor, "//", 2) == 0) {
    while (*t->cursor != '\n') {
      t->cursor ++;
    } 
  }
}

void tokenizer3_consume_whitespace(tokenizer3_t* t) {
  while (*t->cursor == '\n') {
    t->cursor++;
    t->line++;
    t->col = 0;
    t->line_start = t->cursor + 1;
  }
  while (*t->cursor == ' ' || *t->cursor == '\t') {
    t->cursor++;
  }
}

void tokenizer3_advance(tokenizer3_t* t) {
  // when we are in this case, we can't let the tokenizer try to analyze anymore
  // just continually shift the history array until empty at this point
  if (t->cursor > t->source_code + t->source_length) {
    for (int i = 1; i < 5; i++) {
      t->history[i - 1] = t->history[i];
      t->history[i].type = T_EOF;
    }
    return;
  }
  // 1. Shift the history array left (discard the leftmost)
  for (int i = 1; i < 5; i++) {
    t->history[i - 1] = t->history[i];
  }

  // ensure any whitespace is skipped
  tokenizer3_consume_whitespace(t);
  tokenizer3_consume_comments(t);
  tokenizer3_consume_whitespace(t);

  // 2. Replace the last element with the next token
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
  TOK_CHECK_STR("void", 4, T_VOID)
  TOK_CHECK_STR("use", 3, T_USE)
  TOK_CHECK_STR("return", 6, T_RETURN)
  TOK_CHECK_STR("->", 2, T_ARROW)
  TOK_CHECK_STR("==", 2, T_DEQ)
  TOK_CHECK_STR("<=", 2, T_LTEQ)
  TOK_CHECK_STR(">=", 2, T_GTEQ)
  TOK_CHECK_STR("!=", 2, T_NEQ)
  TOK_CHECK_STR(">", 1, T_GT)
  TOK_CHECK_STR("<", 1, T_LT)
  TOK_CHECK_STR("||", 2, T_DOR)
  TOK_CHECK_STR("&&", 2, T_DAND)
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
  TOK_CHECK_CH('&', T_AND)
  TOK_CHECK_CH('|', T_OR)
  TOK_CHECK_CH('\'', T_SQT)
  TOK_CHECK_CH('\"', T_DQT)
  TOK_CHECK_CH('\\', T_BACKSLASH)
  TOK_CHECK_EOF(t)
  if (isdigit(*t->cursor)) {
    number_t number = tokenizer3_as_number(t);
    t->history[4] = (token_t) {.type = T_NUM, LOC_FIELD(t, number.length)};
    t->history[4].value.i = number.value.i; 
    t->cursor += number.length;
    t->col += number.length;
    return;
  }
  if (*t->cursor == '_' || isalpha(*t->cursor) || isdigit(*t->cursor)) {
    identifier_t id = tokenizer3_as_id(t);
    t->history[4] = (token_t) {.type = T_ID, LOC_FIELD(t, id.length)};
    strncpy(t->history[4].value.s, id.begin, id.length < 30 ? id.length : 30);
    t->cursor += id.length;
    t->col += id.length;
    return;
  }
  printf("[%s: %d] Unknown character: '%c'\n", __func__, __LINE__, *t->cursor);
  t->history[4] = (token_t) {.type = T_UNKNOWN, LOC_FIELD(t, 1)};
  t->cursor += 1;
  t->col += 1;
}

token_t tokenizer3_get(tokenizer3_t* t, int offset) {
  int index = offset;
  if (index > 4 || index < 0) {
    fprintf(stderr, "Out of bounds offset\n");
    return (token_t) {};
  }
  return t->history[index];
}

int tokenizer3_expect_offset(tokenizer3_t* t, int offset, token_type_t type) {
  int index = offset;
  if (index > 4 || index < 0) {
    fprintf(stderr, "Out of bounds offset\n");
    return 0;
  }
  return t->history[index].type == type;
}

number_t tokenizer3_as_number(tokenizer3_t* t) {
  number_t number = {0};
  int digit_count = 0;
  char* temp_cursor = t->cursor;
  while (isdigit(*temp_cursor) != 0) {
    number.value.i += ((*temp_cursor) - '0') * pow(10, digit_count);
    digit_count++;
    temp_cursor++;
  }
  number.type = 1; //floats not permited
  number.length = digit_count;

  //reverse number
  int num = number.value.i;
  number.value.i = 0;
  while (num > 0) {
    number.value.i = number.value.i * 10 + num % 10;
    num = num / 10;
  }
  return number;
}

identifier_t tokenizer3_as_id(tokenizer3_t* t) {
  identifier_t id = {0};
  id.begin = t->cursor;
  char* temp_curs = t->cursor;
  while (*temp_curs == '_' || isalpha(*temp_curs) != 0 || isdigit(*temp_curs)) {
    temp_curs++;
    id.length++;
  }
  id.end = id.begin + id.length;
  return id;
}

void tokenizer3_show_history(tokenizer3_t* t) {
  for (int i = 0; i < 5; i++) {
    tokenizer3_token_print(t->history[i], t);
  }
}

void tokenizer3_show_token_offset(tokenizer3_t* t, int offset) {
  tokenizer3_token_print(t->history[offset], t);
}

const char*  tokenizer3_get_token_offset_as_string(tokenizer3_t* t, int offset) {
  switch(t->history[offset].type) {
    case T_EXPR_FUNC_END: return "EXPR_FUNC_END";    break;
    case T_UNKNOWN:       return "UNKNOWN";          break;
    case T_NL:            return "NL";               break;
    case T_SPC:           return "SPC";              break;
    case T_TAB:           return "TAB";              break;
    case T_FN:            return "FN";               break;
    case T_IF:            return "IF";               break;
    case T_FOR:           return "FOR";              break;
    case T_WHILE:         return "WHILE";            break;
    case T_ASM:           return "ASM";              break;
    case T_RB:            return "RB";               break;
    case T_LB:            return "LB";               break;
    case T_RP:            return "RP";               break;
    case T_LP:            return "LP";               break;
    case T_EQ:            return "EQ";               break;
    case T_NOT:           return "NOT";              break;
    case T_COLON:         return "COLON";            break;
    case T_SEMICOLON:     return "SEMICOLON";        break;
    case T_COMMA:         return "COMMA";            break;
    case T_ARROW:         return "ARROW";            break;
    case T_USE:           return "USE";              break;
    case T_RETURN:        return "RETURN";           break;
    case T_INT:           return "INT";              break;
    case T_CHR:           return "CHR";              break;
    case T_SHT:           return "SHT";              break;
    case T_DBL:           return "DBL";              break;
    case T_FLT:           return "FLT";              break;
    case T_BOOL:          return "BOOL";             break;
    case T_DEQ:           return "DEQ";              break;
    case T_LTEQ:          return "LTEQ";             break;
    case T_GTEQ:          return "GTEQ";             break;
    case T_NEQ:           return "NEQ";              break;
    case T_GT:            return "GT";               break;
    case T_LT:            return "LT";               break;
    case T_DOR:           return "DOR";              break;
    case T_DAND:          return "DAND";             break;
    case T_OR:            return "OR";               break;
    case T_AND:           return "AND";              break;
    case T_SQT:           return "SQT";              break;
    case T_DQT:           return "DQT";              break;
    case T_BACKSLASH:     return "BACKSLASH";        break;
    case T_PLUS:          return "PLUS";             break;
    case T_MINUS:         return "MINUS";            break;
    case T_MUL:           return "MUL";              break;
    case T_DIV:           return "DIV";              break;
    case T_MOD:           return "MOD";              break;
    case T_NUM:           return "NUM";              break;
    case T_ID:            return "ID";               break;
    case T_X86_32_LINUX:  return "X86_32_ASM_LINUX"; break;
    case T_X86_64_LINUX:  return "X86_64_ASM_LINUX"; break;
    case T_ARM64:         return "ARM64_ASM";        break;
    case T_EOF:           return "EOF";              break;
    default:              return "unimplemented token\n"; break;
  }
}

void tokenizer3_token_print(token_t t, tokenizer3_t* tokenizer) {
#define PRINT_TOKEN(type_s, t) \
    printf("{%14s, %.*s }\n", type_s, t.loc.length, tokenizer->source_code + t.loc.begin_index);
  printf("Token (%3d) ", t.type);

  switch (t.type) {
    case T_EXPR_FUNC_END: PRINT_TOKEN("EXPR_FUNC_END", t);    break;
    case T_UNKNOWN:       PRINT_TOKEN("UNKNOWN", t);          break;
    case T_NL:            PRINT_TOKEN("NL", t);               break;
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
    case T_USE:           PRINT_TOKEN("USE", t);              break;
    case T_RETURN:        PRINT_TOKEN("RETURN", t);           break;
    case T_INT:           PRINT_TOKEN("INT", t);              break;
    case T_CHR:           PRINT_TOKEN("CHR", t);              break;
    case T_SHT:           PRINT_TOKEN("SHT", t);              break;
    case T_DBL:           PRINT_TOKEN("DBL", t);              break;
    case T_FLT:           PRINT_TOKEN("FLT", t);              break;
    case T_BOOL:          PRINT_TOKEN("BOOL", t);             break;
    case T_DEQ:           PRINT_TOKEN("DEQ", t);              break;
    case T_LTEQ:          PRINT_TOKEN("LTEQ", t);             break;
    case T_GTEQ:          PRINT_TOKEN("GTEQ", t);             break;
    case T_NEQ:           PRINT_TOKEN("NEQ", t);              break;
    case T_GT:            PRINT_TOKEN("GT", t);               break;
    case T_LT:            PRINT_TOKEN("LT", t);               break;
    case T_DOR:           PRINT_TOKEN("DOR", t);              break;
    case T_DAND:          PRINT_TOKEN("DAND", t);             break;
    case T_OR:            PRINT_TOKEN("OR", t);               break;
    case T_AND:           PRINT_TOKEN("AND", t);              break;
    case T_SQT:           PRINT_TOKEN("SQT", t);              break;
    case T_DQT:           PRINT_TOKEN("DQT", t);              break;
    case T_BACKSLASH:     PRINT_TOKEN("BACKSLASH", t);        break;
    case T_PLUS:          PRINT_TOKEN("PLUS", t);             break;
    case T_MINUS:         PRINT_TOKEN("MINUS", t);            break;
    case T_MUL:           PRINT_TOKEN("MUL", t);              break;
    case T_DIV:           PRINT_TOKEN("DIV", t);              break;
    case T_MOD:           PRINT_TOKEN("MOD", t);              break;
    case T_NUM:           PRINT_TOKEN("NUM", t);              break;
    case T_ID:            PRINT_TOKEN("ID", t);               break;
    case T_X86_32_LINUX:  PRINT_TOKEN("X86_32_ASM_LINUX", t); break;
    case T_X86_64_LINUX:  PRINT_TOKEN("X86_64_ASM_LINUX", t); break;
    case T_ARM64:         PRINT_TOKEN("ARM64_ASM", t);        break;
    case T_EOF:           PRINT_TOKEN("EOF", t);              break;
    default:              printf("unimplemented token\n");    break;
  }
}
