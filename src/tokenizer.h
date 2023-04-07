#ifndef TOKENIZER_H
#define TOKENIZER_H
#include <stdlib.h>

#define LOC_FIELD(tokenizer, len) \
  .loc = (token_loc_t) {.begin_index = tokenizer->cursor - tokenizer->source.contents, .length = len, .line = tokenizer->line_number, .column = tokenizer->column_number } 

typedef enum {
  T_UNKNOWN = -1,
  T_NL, T_SPC, T_TAB,
  T_FN, T_PROC,
  T_IF, T_FOR, T_WHILE,
  T_ASM,
  T_RB, T_LB,
  T_RP, T_LP,
  T_EQ, T_NOT,
  T_COLON, T_SEMICOLON, T_COMMA,

  T_ARROW,
  
  // Types
  T_INT, T_SHT, T_CHR, T_DBL, T_FLT, T_BOOL,

  // Comparison
  T_GT, T_LT,
  
  // Quotes
  T_DQT, T_SQT,
  
  // Escape
  T_BACKSLASH,

  // Binary ops
  T_PLUS, T_MINUS, T_MUL, T_DIV, T_MOD,

  // Other
  T_NUM,
  T_ID,
  T_EOF
} token_type_t;

typedef struct {
  char* contents;
  const char* contents_end;
  size_t length;
} source_code_t;

// token_loc
typedef struct {
  int begin_index, length;
  int line, column;
} token_loc_t;

// token_t
typedef struct {
  token_type_t type;
  token_loc_t loc;
} token_t;

// tokenizer_t
typedef struct {
  source_code_t source;
  char* cursor;
  int line_number, column_number;
} tokenizer_t;

/*
 * Desc:  
 *   Create a new tokenizer "instance" given a filepath
 * Notes:
 *   Completely exits program upon invalid filepath
 *   Does not handle possible malloc errors
 *      - User must use the 'tokenizer_free' function to avoid leaks
 * Params:
 *   1) A filepath to a rflang source file
 * Return:
 *   A tokenizer object containing all the information in order to get tokenizing
 */
tokenizer_t tokenizer_new(const char*);

/*
 * Desc:  
 *   Frees a previously allocated tokenizer instance
 * Params:
 *   1) A tokenizer instance previously allocated via 'tokenizer_new'
 * Return:
 *   N/A
 */
void tokenizer_free(tokenizer_t);

/*
 * Desc:  
 *   Advance the tokenizer to the next token and process the current token
 * Notes:
 *   User must manually consume spaces
 * Params:
 *   1) a valid pointer to a tokenizer instance
 * Return:
 *   A token object representnig the next token
 */
token_t tokenizer_next_t(tokenizer_t*);

/*
 * Desc:  
 *   Consume the space tokens where the tokenizer currently is
 * Note:
 *   Meant to make skipping space character a little easier
 *   Likely to not work with tab characters (NOTE: Untested)
 * Params:
 *   1) a valid pointer to a tokenizer instance
 * Return:
 *   The number of space tokens consumed
 */
int tokenizer_consume_spaces(tokenizer_t*);

/*
 * Desc:  
 *   Advance the tokenizer forward by 'x' characters (NOT tokens)
 * Note:
 *   Does not ensure that the number is a reasonable one (i.e. -5 or 300000)
 * Params:
 *   1) a valid pointer to a tokenizer instance
 *   2) the number of characters to advance by
 * Return:
 *   N/A
 */
void tokenizer_advance(tokenizer_t*, int);

/*
 * Desc: 
 *   Analyze next token without advancing the tokenizer
 * Note:
 *   The purpose of this function is mainly as a utlity for the 'tokenizer_next_t(...)' function
 * Params:
 *   1) a valid pointer to a tokenizer instance
 * Return:
 *   A token object representing the next token
 */
token_t tokenizer_peek_t(tokenizer_t*);

/*
 * Desc:
 *   Processes a string of numbers
 * Params:
 *   1) a valid pointer to a tokenizer instance
 *   2) either NULL or a valid memory address to store the length of the string of numbers
 * Return:
 *   The value of the string of numbers parsed as an integer
 */
int tokenizer_process_digit(tokenizer_t*, int*);

/*
 * Desc:
 *   Processes a string of characters
 * Params:
 *   1) a valid pointer to a tokenizer instance
 *   2) either NULL or a valid memory address to store the length of the id
 * Return:
 *   N/A
 */
void tokenizer_process_id(tokenizer_t*, int*);

/*
 * Desc:
 *   Processes a string of characters
 * Params:
 *   1) the token you'd like to print
 *   2) a valid pointer to a tokenizer instance 
 *      - this must be the tokenizer that generated the token to work properly
 * Return:
 *   N/A
 */
void token_print(token_t, tokenizer_t*);

#endif
