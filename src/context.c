#include <stdlib.h>
#include <string.h>

#include "context.h"

#define RAW_DATA_INIT_SIZE 512

struct Context* newContext(int clientFd) {
  struct Context* ctx = (struct Context*)malloc(sizeof(struct Context));
  memset(ctx, 0, sizeof *ctx);
  ctx->clientFd = clientFd;
  ctx->header = newMap();

  ctx->rawHeader = newGrowData(RAW_DATA_INIT_SIZE);
  ctx->rawBody = newGrowData(RAW_DATA_INIT_SIZE);

  return ctx;
}

int cleanContext(struct Context* ctx) {
  cleanMap(ctx->header);

  if (ctx->method != NULL)
    free(ctx->method);

  if (ctx->url != NULL)
    free(ctx->url);

  if (ctx->protocol != NULL)
    free(ctx->protocol);

  cleanGrowData(ctx->rawHeader);
  cleanGrowData(ctx->rawBody);

  free(ctx);

  return 0;
}
