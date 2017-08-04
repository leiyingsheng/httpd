#pragma once

#include "map.h"
#include "utils.h"

struct context {
  int clientFd;
  char* method;
  char* path;
  char* protocol;
  struct map* header;

  struct growData* rawHeader;
  struct growData* rawBody;
};

struct context* newContext(int clientFd);

int cleanContext(struct context* ctx);