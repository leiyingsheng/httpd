#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "utils.h"

/* find out the null line */
char* endOfHeader(const char* srcStr) {
  char *curp = srcStr, *prevp;

  prevp = curp;
  do {
    curp = strstr(prevp, "\n");

    if (curp && curp - prevp == 1) {
      return curp;
    }

    prevp = curp + 1;
  } while (curp != NULL);

  return NULL;
}

/* trim front and back space char */
char* trim(char* str) {
  if (str == NULL) {
    return NULL;
  }

  char* ss = str;

  while (*ss == ' ') {
    ++ss;
  }

  for (str += strlen(str) - 1; str > ss; --str) {
    if (*str != ' ') {
      *(str + 1) = '\0';
      break;
    }
  }

  return ss;
}

/////////////////////////
// growData with malloc()

struct growData* newGrowData(size_t size) {
  struct growData* gd = (struct growData*)malloc(sizeof(struct growData));

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