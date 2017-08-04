#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "map.h"

#define HASH_SIZE 50
#define BUF_SIZE 128

static inline unsigned int hash(char* str) {
  unsigned int val;

  for (val = 0; *str != '\0'; ++str) {
    val = *str + 31 * val;
  }

  return val % HASH_SIZE;
};

struct map* newMap() {
  struct map* p = (struct map*)malloc(sizeof(struct map));
  p->entryTab = (struct entry**)calloc(HASH_SIZE, sizeof(struct entry));

  if (NULL == p->entryTab) {
    return NULL;
  }

  return p;
}

static struct entry* lookup(struct map* m, char* key) {
  struct entry* p;

  for (p = m->entryTab[hash(key)]; p != NULL; p = p->next) {
    if (strcmp(key, p->key) == 0) {
      return p;
    }
  }

  return NULL;
};

int setMap(struct map* m, char* key, char* val) {
  struct entry* p;
  unsigned int hashval;

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

  p->val = strdup(val);

  return 0;
};

char* getMap(struct map* m, char* key) {
  struct entry* p = lookup(m, key);

  if (NULL == p) {
    return NULL;
  }

  return p->val;
}

int cleanMap(struct map* m) {
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

int printMap(struct map* m) {
  if (NULL == m) {
    return -1;
  }

  int i;
  struct entry* p;

  for (i = 0; i < HASH_SIZE; ++i) {
    for (p = m->entryTab[i]; p != NULL; p = p->next) {
      printf("%s:%s\n", p->key, p->val);
    }
  }

  return 0;
}

int mapToHeader(struct growData* gd, struct map* m) {
  if (NULL == m) {
    return -1;
  }

  int i, lineLen;
  struct entry* p;
  char* buf = (char*)malloc(BUF_SIZE);

  for (i = 0; i < HASH_SIZE; ++i) {
    for (p = m->entryTab[i]; p != NULL; p = p->next) {
      lineLen = strlen(p->key) + sizeof ": \r\n" + strlen(p->val);
      if (lineLen > BUF_SIZE) {
        printf("line too long\n");
        free(buf);
        return -1;
      }
      sprintf(buf, "%s: %s\r\n", p->key, p->val);
      appendGrowData(gd, buf, lineLen);
    }
  }

  appendGrowData(gd, "\r\n", sizeof "\r\n");

  free(buf);
  return 0;
}