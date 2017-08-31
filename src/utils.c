#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <error.h>

#include "utils.h"

#define BUF_SIZE 512

/* find out the null line */
char* endOfHeader(const char* srcStr) {
  char *curp = (char*)srcStr, *prevp;

  prevp = curp;
  do {
    curp = strstr(prevp, "\r");

    if (curp && curp - prevp == 1) {
      return curp;
    }

    prevp = curp + 1;

  } while (curp != NULL);

  return NULL;
}

/* trim front and back space char & '\r' '\n' */
char* trim(char* str) {
  if (str == NULL) {
    return NULL;
  }

  char* ss = str;

  while (*ss == ' ' || *ss == '\r' || *ss == '\n') {
    ++ss;
  }

  for (str += strlen(str) - 1; str > ss; --str) {
    if (*str != ' ' && *str != '\r' && *str != '\n') {
      *(str + 1) = '\0';
      break;
    }
  }

  return ss;
}

int readFile(const char* filePath, struct growData* gd) {
  printf("read file: %s\n", filePath);
  FILE* f = fopen(filePath, "r");
  if (!f) {
    perror("read failed");
    return -1;
  }

  char* buf = (char*)malloc(BUF_SIZE);

  while (!feof(f)) {
    if (fread(buf, BUF_SIZE, 1, f) >= 0) {
      // printf("read content:%s\n", buf);
      appendGrowData(gd, buf, strlen(buf));
    }
  }

  free(buf);
  fclose(f);
  return 0;
}

/////////////////////////
// growData with malloc()

struct growData* newGrowData(size_t size) {
  struct growData* gd = (struct growData*)malloc(sizeof(struct growData));
  memset(gd, 0, sizeof *gd);

  gd->data = (char*)malloc(size);
  memset(gd->data, 0, size);

  gd->size = size;

  return gd;
}

int appendGrowData(struct growData* gd, const char* data, size_t n) {
  char* newP;

  if (gd->size - strlen(gd->data) <= n) {  // out of space
    newP = realloc(gd->data, gd->size + n);
    if (!newP) {
      return -1;
    }
    gd->data = newP;
    gd->size += n;
  }
  gd->data = strncat(gd->data, data, n);
  return 0;
}

int cleanGrowData(struct growData* gd) {
  free(gd->data);
  free(gd);
  return 0;
}

////////////////////////