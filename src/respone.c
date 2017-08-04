#include <stdlib.h>

#include "respone.h"

#define BODY_INIT_SIZE 512

struct respone* newRespone() {
  struct respone* resp = (struct respone*)malloc(sizeof(struct respone));

  resp->header = newMap();
  resp->body = newgrowData(512);
  return resp;
}

char* encodeRespone(struct respone* resp) {
  return NULL;
}

int cleanRespone(struct respone* resp) {
  cleanMap(resp->header);
  cleanGrowData(resp->body);
  free(resp);
  return 0;
}