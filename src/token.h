#ifndef TOKEN_H
#define TOKEN_H

typedef enum {
  T_UNKNOWN = -1,
  T_NEWLINE = 1,
  T_EOF,
  T_IDENTIFIER,
  T_POUND,
  T_TAB, T_SPACE, T_BACK_SLASH,
  T_SQUOTE, T_DQUOTE,
  T_ASSIGN,
  T_MINUS, T_PLUS, T_MUL, T_FORWARD_SLASH, T_MODULUS,
  T_COMMA, T_COLON, T_SEMI_COLON,
  T_LPAREN, T_RPAREN,
  T_LBRACE, T_RBRACE,
  T_LSQ_BRACE, T_RSQ_BRACE,
  T_BIT_AND, T_BIT_OR, T_BIT_NOT,
  T_COMP_GT, T_COMP_LT,
  T_NUMBER_LITERAL, T_STRING_LITERAL,
  T_FN,
  T_IF, T_WHILE, T_FOR,
  T_CHAR, T_INT, T_SHORT,
  T_ASM,
  T_RETURN,
  T_ARROW,
  T_USE
} token_type_t;
typedef struct {
  int value;
  int char_length;
} number_token_t;
typedef struct {
  char str[100];
  int str_length;
} string_lit_token_t;

typedef struct {
  // different token types
  union {
    char char_token;
    number_token_t number_token;
    string_lit_token_t string_lit_token; // used for string literals, keywords, identifiers, etc
  } data;

  token_type_t token_type;
  int column, line, pos;
  int valid;
  int length;
} token_t;

void rfc_set_token_list(token_t* token_list, int token_count, token_t set_token);

/*
Inputs:
  token_t*  => a pointer to a SINGLE token      (think about making this pass by value?)
Outputs:
  N/A
*/
void rfc_token_print(token_t*);

/*
Inputs:
  token_t*  => a pointer to a buffer to store the tokenized file
  int       => the length of the buffer
Outputs:
  N/A
*/
void rfc_display_token_list(token_t*, int);

#endif