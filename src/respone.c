#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "respone.h"

#define BODY_INIT_SIZE 512

struct respone* newRespone(int statusCode) {
  struct respone* resp = (struct respone*)malloc(sizeof(struct respone));

  resp->statusCode = statusCode;
  resp->header = newMap();
  resp->body = newGrowData(BODY_INIT_SIZE);
  return resp;
}

int encodeRespone(int clientFd, struct respone* resp) {
  struct growData* buf = newGrowData(1024);

  appendGrowData(buf, "HTTP/1.1 200 OK\r\n", sizeof "HTTP/1.1 200 OK\r\n");
  mapToHeader(buf, resp->header);
  dprintf(clientFd, "%s%s", buf->data, resp->body->data);
  //   dprintf(clientFd, "%s %s %s\r\n%s%s", resp->protocol, resp->statusCode,
  //           resp->message, buf, resp->body->data);
  cleanGrowData(buf);
  return 0;
}

int cleanRespone(struct respone* resp) {
  cleanMap(resp->header);
  cleanGrowData(resp->body);
  free(resp);
  return 0;
}