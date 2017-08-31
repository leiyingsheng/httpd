#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
// #include <regex.h>

#include "request.h"
#include "utils.h"

#define BUF_SIZE 1024

int readRequest(struct context* ctx) {
  char* buf = (char*)malloc(BUF_SIZE);
  int n;
  char* eoh = NULL;

  printf("readRequest: start\n");
  do {
    n = read(ctx->clientFd, buf, BUF_SIZE);
    if (n < 0) {
      perror("read failed");
      free(buf);
      return -1;
    }
    // printf("%.*s", n, buf);

    if (!eoh) {  // header
      eoh = endOfHeader(buf);

      appendGrowData(ctx->rawHeader, buf, eoh ? eoh - buf : n);

      if (eoh) {  // rest of body
        appendGrowData(ctx->rawBody, eoh + 1, n - (eoh - buf - 1));
      }
    } else {
      appendGrowData(ctx->rawBody, buf, n);
    }

  } while (n == BUF_SIZE);
  printf("readRequest: end\n");

  free(buf);
  return 0;
}

/* read header field into context map */
int parseHeader(struct context* ctx) {
  if (strlen(ctx->rawHeader->data) == 0) {
    return -1;
  }

  char *buf = strdup(ctx->rawHeader->data), *tmpp = buf;
  char *line, *key;

  // TODO:err handle
  line = strsep(&tmpp, "\r");  // request-line

  key = strsep(&line, " ");  // http method
  ctx->method = strdup(key);

  key = strsep(&line, " ");  // request url
  ctx->url = strdup(key);

  ctx->protocol = strdup(line);  // protocol

  // printf("%s %s %s\n", ctx->method, ctx->url, ctx->protocol);

  // read field
  for (line = strsep(&tmpp, "\r"); line != NULL;
       line = strsep(&tmpp, "\r")) {  // line

    if (strlen(line) == 0) {  // null line
      continue;
    }

    key = strsep(&line, ":");  // cut off key
    if (line == NULL) {        // null val
      continue;
    }
    setMap(ctx->header, trim(key), trim(line));
  }

  free(buf);
  return 0;
}
