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

  printf("header:\n%s\n", ctx->rawHeader->data);
  printf("body:\n%s\n", ctx->rawBody->data);

  free(buf);
  return 0;
}

/* read header field into context map */
int parseHeader(struct context* ctx) {
  char *buf = strdup(ctx->rawHeader->data), *tmpp = buf;
  char *line, *key;

  // TODO:err handle
  line = strsep(&tmpp, "\n");  // status line

  key = strsep(&line, " ");  // http method
  ctx->method = strdup(key);

  key = strsep(&line, " ");  // request path
  ctx->path = strdup(key);

  ctx->protocol = strdup(line);  // protocol

  // printf("%s %s %s\n", ctx->method, ctx->path, ctx->protocol);

  // read field
  for (line = strsep(&tmpp, "\n"); line != NULL;
       line = strsep(&tmpp, "\n")) {  // line

    if (strlen(line) == 0) {  // null line
      continue;
    }

    key = strsep(&line, ":");  // cut off key
    if (line == NULL) {        // null val
      continue;
    }
    setMap(ctx->header, key, trim(line));
  }

  free(buf);
  return 0;
}
