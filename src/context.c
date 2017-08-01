#include "context.h"

#include <stdlib.h>

struct context* newContext(int clientFd) {
  struct context* ctx = (struct context*)malloc(sizeof(struct context));
  ctx->clientFd = clientFd;
  ctx->header = newMap();
  return ctx;
}

int context_cleanup(struct context* ctx) {
  map_clean(ctx->header);
  free(ctx);
  ctx = NULL;
  return 0;
}