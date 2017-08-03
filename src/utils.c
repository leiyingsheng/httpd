#include "utils.h"
#include <stdio.h>
#include <string.h>

/* find out the null line */
char* endOfHeader(const char* srcStr) {
  char *curp = srcStr, *prevp;

  prevp = curp;
  do {
    curp = strstr(prevp, "\n");

    if (curp && curp - prevp == 1) {
      return prevp;
    }

    prevp = curp + 1;
  } while (curp != NULL);

  return NULL;
}

/* trim front and back space char */
char* trim(char* str) {
  char* ss = str;
  // printf("trim:\n%s===\n", ss);//TODO:debug
  while (*ss == ' ') {
    ++ss;
  }

  for (str += strlen(str); str != ss; --str) {
    if (*str != ' ') {
      *str = '\0';
      break;
    }
  }

  // printf("%s===\n", ss);//TODO:debug

  return ss;
}