#include "request.h"
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define BUF_SIZE 512

int handle_request(int clientFd) {
  struct context* ctx = newContext(clientFd);

  parse_header(ctx);

  char str[] =
      "HTTP / 1.0 200 BAD REQUEST\r\n"
      "Content - type : text / html\r\n"
      "\r\n"
      "<p>hello world</p>";
  write(ctx->clientFd, str, sizeof(str));

  close(ctx->clientFd);

  return 0;
}

int parse_header(struct context* ctx) {
  char buf[BUF_SIZE];
  int n;

  do {
    n = read(ctx->clientFd, buf, BUF_SIZE);
    if (n < 0) {
      perror("read failed");
      return -1;
    }

    printf("read:\n%s\n", buf);

  } while (n == BUF_SIZE);

  return 0;
}