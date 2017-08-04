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

struct growData* newgrowData(size_t size) {
  struct growData* rd = (struct growData*)malloc(sizeof(struct growData));

  rd->data = (char*)malloc(size);
  rd->size = size;

  return rd;
}

int appendGrowData(struct growData* rd, const char* data, size_t n) {
  char* newP;

  if (rd->size - strlen(rd->data) <= n) {  // out of space
    newP = realloc(rd->data, rd->size + n);
    if (!newP) {
      return -1;
    }
    rd->data = newP;
    rd->size += n;
  }
  rd->data = strncat(rd->data, data, n);
  return 0;
}

int cleanGrowData(struct growData* rd) {
  free(rd->data);
  free(rd);
  return 0;
}

////////////////////////