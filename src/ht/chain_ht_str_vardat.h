#ifndef CHAINING_HT_STR_VARDAT_H
#define CHAINING_HT_STR_VARDAT_H

// forward declare chaining_entry
typedef struct chaining_node_str_var_t chaining_node_str_var_t; 

typedef struct {
  char key[100];
  int scope_depth;
  int scope_number;
  int type;
} entry_var;

struct chaining_node_str_var_t {
  entry_var value;
  chaining_node_str_var_t* next;
  chaining_node_str_var_t* prev;
};

typedef struct {
  chaining_node_str_var_t** buffer;
  int M;
} chaining_ht_str_var_t;

chaining_ht_str_var_t    chaining_ht_str_var_alloc(int);
void                     chaining_ht_str_var_free(chaining_ht_str_var_t);

int                      chaining_ht_str_var_hash(chaining_ht_str_var_t, char*);

void                     chaining_ht_str_var_show_entry(entry_var);
void                     chaining_ht_str_var_show(chaining_ht_str_var_t, int);

void                     chaining_ht_str_var_put(chaining_ht_str_var_t, char*, entry_var);
int                      chaining_ht_str_var_remove(chaining_ht_str_var_t, char*);
entry_var                chaining_ht_str_var_find(chaining_ht_str_var_t, char*);
int                      chaining_ht_str_var_contains(chaining_ht_str_var_t, char*);

#endif
