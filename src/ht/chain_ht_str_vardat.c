#include "chain_ht_str_vardat.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

const char* map_type_to_string(int type) {
  switch (type) {
    case 1:  return "FUNC_ID";
    case 2:  return "VAR_ID";
    default: return "INVALID_TYPE";
  }
}

chaining_ht_str_var_t chaining_ht_str_var_alloc(int max_size) {
  chaining_ht_str_var_t ht = {0};
  ht.buffer = malloc(sizeof(chaining_node_str_var_t*) * max_size);
  ht.M = max_size;
  for (int i = 0; i < ht.M; i++) {
    ht.buffer[i] = NULL;  //initialize each linked list as NULL
  }
  return ht;
}

void chaining_ht_str_var_free(chaining_ht_str_var_t ht) {
  //TODO: Free the linked lists then free the main buffer
  for (int i = 0; i < ht.M; i++) {
    // free ht.buffer[i];
    chaining_node_str_var_t* e = ht.buffer[i];
    while (e != NULL) {
      chaining_node_str_var_t* t = e;
      e = e->next;
      free(t);
      t = NULL;
    }
  }
  free(ht.buffer);
  ht.buffer = NULL;
}

int chaining_ht_str_var_hash(chaining_ht_str_var_t ht, char* key) {
  int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
    hash += key[i] * (key[i] - 5);
  }
  return hash % ht.M;
}

void chaining_ht_str_var_show_entry(entry_var entry) {
  printf("{key: %7s, scope_depth: %d, scope_number: %d, type: %s}", entry.key, entry.scope_depth, entry.scope_number, map_type_to_string(entry.type));
}

void chaining_ht_str_var_show(chaining_ht_str_var_t ht, int context) {
  printf("Showing chaining hash table {%d}\n", ht.M);
  for (int i = 0; i < ht.M; i++) {
    chaining_node_str_var_t* e = ht.buffer[i];
    if (context) printf("%d:\n", i);
    while (e != NULL) {
      printf("\t");
      chaining_ht_str_var_show_entry(e->value);
      printf("\n");
      e = e->next;
    }
    if (context) printf("NULL\n");
  }
}

void chaining_ht_str_var_put(chaining_ht_str_var_t ht, char* key, entry_var entry_data) {
  int hash = chaining_ht_str_var_hash(ht, key);
  // append the entry to the end of the corresponding linked list
  chaining_node_str_var_t* list = ht.buffer[hash];
  if (!list) {
    ht.buffer[hash] = calloc(1, sizeof(chaining_node_str_var_t));
    ht.buffer[hash]->value = entry_data;
    ht.buffer[hash]->next = NULL;
    ht.buffer[hash]->prev = NULL;
  }
  else {
    // insert before the head of the list
    chaining_node_str_var_t* e = calloc(1, sizeof(chaining_node_str_var_t));
    e->value = entry_data;
    e->next = list;
    e->prev = NULL;
    ht.buffer[hash] = e;
    list->prev = e;
  }
}

int chaining_ht_str_var_remove(chaining_ht_str_var_t ht, char* key) {
  int hash = chaining_ht_str_var_hash(ht, key);
  chaining_node_str_var_t* list = ht.buffer[hash];
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

entry_var chaining_ht_str_var_find(chaining_ht_str_var_t ht, char* key) {
  int hash = chaining_ht_str_var_hash(ht, key);
  chaining_node_str_var_t* list = ht.buffer[hash];
  if (!list) {
    // list where this element would go is empty
    return (entry_var) {0};
  }
  else {
    while (list != NULL) {
      if (strncmp(list->value.key, key, 100) == 0) {
        return list->value;
      }
      list = list->next;
    }
  }
  return (entry_var) {0}; // should never be here. when using this function you should always check if the key exists
}

int chaining_ht_str_var_contains(chaining_ht_str_var_t ht, char* key) {
  int hash = chaining_ht_str_var_hash(ht, key);
  chaining_node_str_var_t* list = ht.buffer[hash];
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
