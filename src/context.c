#include "context.h"

#include <stdlib.h>
#include <string.h>

#define RAW_DATA_INIT_SIZE 512

struct context* newContext(int clientFd) {
  struct context* ctx = (struct context*)malloc(sizeof(struct context));
  ctx->clientFd = clientFd;
  ctx->header = newMap();

  ctx->rawHeader = newRawData(RAW_DATA_INIT_SIZE);
  ctx->rawBody = newRawData(RAW_DATA_INIT_SIZE);

  return ctx;
}

int context_cleanup(struct context* ctx) {
  map_clean(ctx->header);

  free(ctx->method);
  free(ctx->path);
  free(ctx->protocol);

  cleanRawData(ctx->rawHeader);
  cleanRawData(ctx->rawBody);

  free(ctx);

  return 0;
}

struct rawData* newRawData(size_t size) {
  struct rawData* rd = (struct rawData*)malloc(sizeof(struct rawData));

  rd->data = (char*)malloc(size);
  rd->size = size;

  return rd;
}

int appendRawData(struct rawData* rd, const char* data, size_t n) {
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

int cleanRawData(struct rawData* rd) {
  free(rd->data);
  free(rd);
  return 0;
}