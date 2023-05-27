#include "chaining_ht.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

chaining_ht_t chaining_ht_alloc(int max_size) {
  chaining_ht_t ht = {0};
  ht.buffer = malloc(sizeof(chaining_node_t*) * max_size);
  ht.M = max_size;
  for (int i = 0; i < ht.M; i++) {
    ht.buffer[i] = NULL;  //initialize each linked list as NULL
  }
  return ht;
}

void chaining_ht_free(chaining_ht_t ht) {
  //TODO: Free the linked lists then free the main buffer
}

int chaining_ht_hash(chaining_ht_t ht, char* key) {
  int hash = 0;
    for (int i = 0; i < strlen(key); i++) {
    hash += key[i] * (key[i] - 5);
  }
  return hash % ht.M;
}

void chaining_ht_show_entry_data(chaining_entry_data_t entry) {
  printf("{key: %s, scope_depth: %d, scope_number: %d, type: %d}", entry.key, entry.scope_depth, entry.scope_number, entry.type);
}

void chaining_ht_show(chaining_ht_t ht) {
  printf("Showing chaining hash table {%d}\n", ht.M);
  for (int i = 0; i < ht.M; i++) {
    chaining_node_t* e = ht.buffer[i];
    printf("%d: ", i);
    while (e != NULL) {
      chaining_ht_show_entry_data(e->value);
      printf(" -> ");
      e = e->next;
    }
    printf("NULL\n");
  }
}

void chaining_ht_put(chaining_ht_t ht, char* key, chaining_entry_data_t entry_data) {
  int hash = chaining_ht_hash(ht, key);
  // append the entry to the end of the corresponding linked list
  chaining_node_t* list = ht.buffer[hash];
  if (!list) {
    ht.buffer[hash] = calloc(1, sizeof(chaining_node_t));
    ht.buffer[hash]->value = entry_data;
    ht.buffer[hash]->next = NULL;
    ht.buffer[hash]->prev = NULL;
  }
  else {
    // insert at the head of the list
    chaining_node_t* e = calloc(1, sizeof(chaining_node_t));
    e->value = entry_data;
    e->next = list;
    e->prev = NULL;
    ht.buffer[hash] = e;
    list->prev = e;
  }
}

int chaining_ht_remove(chaining_ht_t ht, char* key) {
  int hash = chaining_ht_hash(ht, key);
  chaining_node_t* list = ht.buffer[hash];
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

chaining_entry_data_t chaining_ht_find(chaining_ht_t ht, char* key) {
  int hash = chaining_ht_hash(ht, key);
  chaining_node_t* list = ht.buffer[hash];
  if (!list) {
    // list where this element would go is empty
    return (chaining_entry_data_t) {0};
  }
  else {
    while (list != NULL) {
      if (strncmp(list->value.key, key, 100) == 0) {
        return list->value;
      }
      list = list->next;
    }
  }
  return (chaining_entry_data_t) {0}; // should never be here. when using this function you should always check if the key exists
}

int chaining_ht_contains(chaining_ht_t ht, char* key) {
  int hash = chaining_ht_hash(ht, key);
  chaining_node_t* list = ht.buffer[hash];
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
