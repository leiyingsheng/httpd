#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "map.h"

#define BUF_SIZE 128

static inline unsigned int hash(char* str) {
  unsigned int val;

  for (val = 0; *str != '\0'; ++str) {
    val = *str + 31 * val;
  }

  return val % HASH_SIZE;
};

struct Map* newMap() {
  struct Map* p = (struct Map*)malloc(sizeof(struct Map));
  p->entryTab = (struct entry**)calloc(HASH_SIZE, sizeof(struct entry*));

  if (NULL == p->entryTab) {
    return NULL;
  }

  return p;
}

static struct entry* lookup(struct Map* m, char* key) {
  struct entry* p;

  for (p = m->entryTab[hash(key)]; p != NULL; p = p->next) {
    if (strcmp(key, p->key) == 0) {
      return p;
    }
  }

  return NULL;
};

/* val must be malloced */
int setMap(struct Map* m, char* key, void* val) {
  struct entry* p;
  unsigned int hashval;

  // entry not exist
  if (NULL == (p = lookup(m, key))) {
    p = (struct entry*)malloc(sizeof(struct entry));
    if (NULL == p) {
      return -1;
    }

    p->key = strdup(key);
    hashval = hash(key);
    p->next = m->entryTab[hashval];
    m->entryTab[hashval] = p;

  } else {
    free(p->val);
  }

  p->val = val;

  return 0;
};

void* getMap(struct Map* m, char* key) {
  struct entry* p = lookup(m, key);

  if (NULL == p) {
    return NULL;
  }

  return p->val;
}

int cleanMap(struct Map* m) {
  int i;
  struct entry *p, *tmp;

  for (i = 0; i < HASH_SIZE; ++i) {
    p = m->entryTab[i];
    while (p != NULL) {  // clean up the list
      tmp = p;
      p = p->next;
      free(tmp->key);
      free(tmp->val);
      free(tmp);
    }
    m->entryTab[i] = NULL;
  }

  free(m->entryTab);
  // m->entryTab = NULL;

  return 0;
}

int printMap(struct Map* m) {
  if (NULL == m) {
    return -1;
  }

  int i;
  struct entry* p;

  for (i = 0; i < HASH_SIZE; ++i) {
    for (p = m->entryTab[i]; p != NULL; p = p->next) {
      printf("%s:%s\n", p->key, (char*)p->val);
    }
  }

  return 0;
}
