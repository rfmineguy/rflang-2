#ifndef CHAINING_HT_STR_PROG_H
#define CHAINING_HT_STR_PROG_H
#include "../parsers/parser3.h"

typedef struct chaining_node_str_module_t chaining_node_str_module_t; 

typedef enum {
  MODULE_PARSE_TREE,
  MODULE_NAME
} entry_print_style;

typedef struct {
  char key[100];
  module_t* p;
} entry_module;

struct chaining_node_str_module_t {
  entry_module value;
  chaining_node_str_module_t* next;
  chaining_node_str_module_t* prev;
};

typedef struct {
  chaining_node_str_module_t** buffer;
  int M;
} chaining_ht_str_module_t;

chaining_ht_str_module_t chaining_ht_str_module_alloc(int);
void                     chaining_ht_str_module_free(chaining_ht_str_module_t);

int                      chaining_ht_str_module_hash(chaining_ht_str_module_t, char*);

void                     chaining_ht_str_module_show_entry(entry_module, entry_print_style);
void                     chaining_ht_str_module_show(chaining_ht_str_module_t, entry_print_style);

void                     chaining_ht_str_module_put(chaining_ht_str_module_t, char*, module_t*);
int                      chaining_ht_str_module_remove(chaining_ht_str_module_t, char*);
entry_module             chaining_ht_str_module_find(chaining_ht_str_module_t, char*);
int                      chaining_ht_str_module_contains(chaining_ht_str_module_t, char*);

#endif
