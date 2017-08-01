#pragma once
#include "map.h"

struct context {
  int clientFd;
  struct map* header;
};

struct context* newContext(int clientFd);

int context_cleanup(struct context* ctx);