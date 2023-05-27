#ifndef CHAINING_HT_H
#define CHAINING_HT_H

// forward declare chaining_entry
typedef struct chaining_node_t chaining_node_t; 

typedef struct {
  char key[100];
  int scope_depth;
  int scope_number;
  int type;
} chaining_entry_data_t; 

struct chaining_node_t {
  chaining_entry_data_t value;
  chaining_node_t* next;
  chaining_node_t* prev;
};

typedef struct {
  chaining_node_t** buffer;
  int M;
} chaining_ht_t;

chaining_ht_t         chaining_ht_alloc(int);
void                  chaining_ht_free(chaining_ht_t);

int                   chaining_ht_hash(chaining_ht_t, char*);

void                  chaining_ht_show_entry(chaining_node_t);
void                  chaining_ht_show(chaining_ht_t);

void                  chaining_ht_put(chaining_ht_t, char*, chaining_entry_data_t);
int                   chaining_ht_remove(chaining_ht_t, char*);
chaining_entry_data_t chaining_ht_find(chaining_ht_t, char*);
int                   chaining_ht_contains(chaining_ht_t, char*);

#endif
