#pragma once
#include "map.h"
#include <stddef.h>

struct context {
  int clientFd;
  char* method;
  char* path;
  char* protocol;
  struct map* header;

  struct rawData* rawHeader;
  struct rawData* rawBody;
};

struct rawData {
  size_t size;
  char* data;
};

struct rawData* newRawData(size_t size);

int appendRawData(struct rawData* rd, const char* data, size_t n);

int cleanRawData(struct rawData* rd);

struct context* newContext(int clientFd);

int context_cleanup(struct context* ctx);