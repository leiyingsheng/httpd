/*
*	Copyright © 2017 Joe Xu <joe.0x01@gmail.com>
*	This work is free. You can redistribute it and/or modify it under the
*	terms of the Do What The Fuck You Want To Public License, Version 2,
*	as published by Sam Hocevar. See the COPYING file for more details.
*
 */

#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "request.h"
#include "httpd.h"

int main(void) {
  int listenFd = setupListener();

  struct sockaddr_in clientAddr;
  socklen_t clientAddrLen = sizeof(clientAddr);
  while (1) {
    int clientFd =
        accept(listenFd, (struct sockaddr*)&clientAddr, &clientAddrLen);

    printf("hi %d\n", clientFd);
    handleRequest(clientFd);
  }
  close(listenFd);

  return 0;
}

int setupListener() {
  int listenFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  struct sockaddr_in listenAddr;
  memset(&listenAddr, 0, sizeof(listenAddr));
  listenAddr.sin_family = AF_INET;
  listenAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  listenAddr.sin_port = htons(1234);

  bind(listenFd, (struct sockaddr*)&listenAddr, sizeof(listenAddr));

  listen(listenFd, 20);
  return listenFd;
}

int handleRequest(int clientFd) {
  struct context* ctx = newContext(clientFd);

  readRequest(ctx);
  parseHeader(ctx);

  printMap(ctx->header);

  char str[] =
      "HTTP / 1.0 200 OK\r\n"
      "Content - type : text / html\r\n"
      "\r\n"
      "<p>hello world</p>";
  write(ctx->clientFd, str, sizeof(str));

  close(ctx->clientFd);
  cleanContext(ctx);

  return 0;
}
