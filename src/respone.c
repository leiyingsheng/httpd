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

/* encode respone and send to client */
int encodeRespone(int clientFd, struct respone* resp) {
  struct growData* buf = newGrowData(1024);
  char statusLine[32];

  sprintf(statusLine, "%s %d %s\r\n", resp->protocol, resp->statusCode,
          resp->message);  // status-line
  appendGrowData(buf, statusLine, strlen(statusLine));

  mapToHeader(buf, resp->header);  // append respone header

  dprintf(clientFd, "%s%s", buf->data, resp->body->data);

  cleanGrowData(buf);
  return 0;
}

int cleanRespone(struct respone* resp) {
  cleanMap(resp->header);
  cleanGrowData(resp->body);
  free(resp);
  return 0;
}