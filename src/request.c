#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
// #include <regex.h>

#include "request.h"
#include "utils.h"

#define BUF_SIZE 1024

int handle_request(int clientFd) {
  struct context* ctx = newContext(clientFd);

  read_all(ctx);
  parse_header(ctx);

  print_map(ctx->header);

  char str[] =
      "HTTP / 1.0 200 OK\r\n"
      "Content - type : text / html\r\n"
      "\r\n"
      "<p>hello world</p>";
  write(ctx->clientFd, str, sizeof(str));

  close(ctx->clientFd);
  context_cleanup(ctx);

  return 0;
}

int read_all(struct context* ctx) {
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

      appendRawData(ctx->rawHeader, buf, eoh ? eoh - buf : n);

      if (eoh) {  // rest of body
        appendRawData(ctx->rawBody, eoh, n - (eoh - buf));
      }
    } else {
      appendRawData(ctx->rawBody, buf, n);
    }

  } while (n == BUF_SIZE);

  printf("parse header:\n%s\n", ctx->rawHeader->data);
  printf("parse body:\n%s\n", ctx->rawBody->data);

  free(buf);
  return 0;
}

/* read header field into context map */
int parse_header(struct context* ctx) {
  char *buf = strdup(ctx->rawHeader->data), *tmpp = buf;
  char *line, *key;

  // TODO:err handle
  line = strsep(&tmpp, "\n");  // request header

  key = strsep(&line, " ");  // http method
  ctx->method = strdup(key);

  key = strsep(&line, " ");  // request path
  ctx->path = strdup(key);

  ctx->protocol = strdup(line);  // protocol

  // printf("%s %s %s\n", ctx->method, ctx->path, ctx->protocol);

  // read field
  for (line = strsep(&tmpp, "\n"); line != NULL;
       line = strsep(&tmpp, "\n")) {  // line

    if (strlen(line) == 0) {
      continue;
    }

    key = strsep(&line, ":");  // cut off key
    map_set(ctx->header, key, trim(line));
  }

  free(buf);
  return 0;
}
