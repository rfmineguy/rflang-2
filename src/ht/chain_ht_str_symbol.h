#ifndef CHAINING_HT_STR_SYMBOL_H
#define CHAINING_HT_STR_SYMBOL_H

typedef struct chaining_node_str_symbol_t chaining_node_str_symbol_t; 

typedef enum {
  SYM_FUNC
} symbol_type;

typedef struct {
  char key[100];
  symbol_type type;
} entry_symbol;

struct chaining_node_str_symbol_t {
  entry_symbol value;
  chaining_node_str_symbol_t* next;
  chaining_node_str_symbol_t* prev;
};

typedef struct {
  chaining_node_str_symbol_t** buffer;
  int M;
} chaining_ht_str_symbol_t;

chaining_ht_str_symbol_t chaining_ht_str_symbol_alloc(int);
void                     chaining_ht_str_symbol_free(chaining_ht_str_symbol_t);

int                      chaining_ht_str_symbol_hash(chaining_ht_str_symbol_t, char*);

void                     chaining_ht_str_symbol_show_entry(entry_symbol);
void                     chaining_ht_str_symbol_show(chaining_ht_str_symbol_t);

void                     chaining_ht_str_symbol_put(chaining_ht_str_symbol_t, char*, entry_symbol);
int                      chaining_ht_str_symbol_remove(chaining_ht_str_symbol_t, char*);
entry_symbol             chaining_ht_str_symbol_find(chaining_ht_str_symbol_t, char*);
int                      chaining_ht_str_symbol_contains(chaining_ht_str_symbol_t, char*);

#endif
