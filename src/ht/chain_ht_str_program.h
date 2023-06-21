#ifndef CHAINING_HT_STR_PROG_H
#define CHAINING_HT_STR_PROG_H
#include "../parsers/parser3.h"

// forward declare chaining_entry
typedef struct chaining_node_str_program_t chaining_node_str_program_t; 

typedef struct {
  char key[100];
  program_t* p;
} entry_program;

struct chaining_node_str_program_t {
  entry_program value;
  chaining_node_str_program_t* next;
  chaining_node_str_program_t* prev;
};

typedef struct {
  chaining_node_str_program_t** buffer;
  int M;
} chaining_ht_str_program_t;

chaining_ht_str_program_t    chaining_ht_str_program_alloc(int);
void                     chaining_ht_str_program_free(chaining_ht_str_program_t);

int                      chaining_ht_str_program_hash(chaining_ht_str_program_t, char*);

void                     chaining_ht_str_program_show_entry(entry_program);
void                     chaining_ht_str_program_show(chaining_ht_str_program_t, int);

void                     chaining_ht_str_program_put(chaining_ht_str_program_t, char*, program_t*);
int                      chaining_ht_str_program_remove(chaining_ht_str_program_t, char*);
entry_program            chaining_ht_str_program_find(chaining_ht_str_program_t, char*);
int                      chaining_ht_str_program_contains(chaining_ht_str_program_t, char*);

#endif
