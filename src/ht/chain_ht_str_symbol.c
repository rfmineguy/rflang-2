#include "chain_ht_str_symbol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

chaining_ht_str_symbol_t chaining_ht_str_symbol_alloc(int max_size) {
  chaining_ht_str_symbol_t ht = {0};
  ht.buffer = malloc(sizeof(chaining_node_str_symbol_t*) * max_size);
  ht.M = max_size;
  for (int i = 0; i < ht.M; i++) {
    ht.buffer[i] = NULL;  //initialize each linked list as NULL
  }
  return ht;
}

void chaining_ht_str_symbol_free(chaining_ht_str_symbol_t ht) {
  //TODO: Free the linked lists then free the main buffer
  for (int i = 0; i < ht.M; i++) {
    // free ht.buffer[i];
    chaining_node_str_symbol_t* e = ht.buffer[i];
    while (e != NULL) {
      chaining_node_str_symbol_t* t = e;
      e = e->next;
      free(t);
      t = NULL;
    }
  }
  free(ht.buffer);
  ht.buffer = NULL;
}

int chaining_ht_str_symbol_hash(chaining_ht_str_symbol_t ht, char* key) {
  int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
    hash += key[i] * (key[i] - 5);
  }
  return hash % ht.M;
}

void chaining_ht_str_symbol_show_entry(entry_symbol entry) {
  switch (entry.type) {
  case SYM_FUNC:
    printf("{type = func, symbol = %10s, param_count = %3d, return_type = %3d}\n", 
        entry.key, entry.data.func_signature.param_count, entry.data.func_signature.return_type);
    break;
  case SYM_VAR:
    printf("{type = var, symbol = %s}\n", entry.key);
    break;
  }
}

void chaining_ht_str_symbol_show(chaining_ht_str_symbol_t ht) {
  printf("Showing chaining hash table {%d}\n", ht.M);
  for (int i = 0; i < ht.M; i++) {
    chaining_node_str_symbol_t* e = ht.buffer[i];
    while (e != NULL) {
      printf("\t");
      chaining_ht_str_symbol_show_entry(e->value);
      // printf("\n");
      e = e->next;
    }
  }
}

void chaining_ht_str_symbol_put(chaining_ht_str_symbol_t ht, char* key, entry_symbol symbol) {
  int hash = chaining_ht_str_symbol_hash(ht, key);
  // append the entry to the end of the corresponding linked list
  chaining_node_str_symbol_t* list = ht.buffer[hash];
  strncpy(symbol.key, key, 100);
  if (!list) {
    ht.buffer[hash] = calloc(1, sizeof(chaining_node_str_symbol_t));
    ht.buffer[hash]->value = symbol;
    ht.buffer[hash]->next = NULL;
    ht.buffer[hash]->prev = NULL;
  }
  else {
    // insert before the head of the list
    chaining_node_str_symbol_t* e = calloc(1, sizeof(chaining_node_str_symbol_t));
    e->value = symbol;
    e->next = list;
    e->prev = NULL;
    ht.buffer[hash] = e;
    list->prev = e;
  }
}

int chaining_ht_str_symbol_remove(chaining_ht_str_symbol_t ht, char* key) {
  int hash = chaining_ht_str_symbol_hash(ht, key);
  chaining_node_str_symbol_t* list = ht.buffer[hash];
  if (!list) {
    // list empty nothing to remove
    return 0;
  }
  else if (strncmp(list->value.key, key, 100) == 0) {
    // only element in the list
    ht.buffer[hash] = list->next;
    free(list);
    list = NULL;
    return 1;
  }
  else {
    while (list != NULL) {
      if (strncmp(list->value.key, key, 100) == 0) {
        // remove this element
        if (list->prev)
          list->prev->next = list->next;
        if (list->next)
          list->next->prev = list->prev;
        free(list);
        list = NULL;
        return 1; //succesful remove
      }
      list = list->next;
    }
  }
  return 0; //unsuccessful remove
}

entry_symbol chaining_ht_str_symbol_find(chaining_ht_str_symbol_t ht, char* key) {
  int hash = chaining_ht_str_symbol_hash(ht, key);
  chaining_node_str_symbol_t* list = ht.buffer[hash];
  if (!list) {
    // list where this element would go is empty
    return (entry_symbol) {0};
  }
  else {
    while (list != NULL) {
      if (strncmp(list->value.key, key, 100) == 0) {
        return list->value;
      }
      list = list->next;
    }
  }
  return (entry_symbol) {0}; // should never be here. when using this function you should always check if the key exists
}

int chaining_ht_str_symbol_contains(chaining_ht_str_symbol_t ht, char* key) {
  int hash = chaining_ht_str_symbol_hash(ht, key);
  chaining_node_str_symbol_t* list = ht.buffer[hash];
  if (!list) {
    // list where this element would go is empty
    return 0;
  }
  else {
    while (list != NULL) {
      if (strncmp(list->value.key, key, 100) == 0) {
        return 1;
      }
      list = list->next;
    }
  }
  return 0;
}
