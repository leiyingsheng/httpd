#pragma once

#include "map.h"
#include "utils.h"

struct Context {
  int clientFd;
  char* method;
  char* url;
  char* protocol;
  struct Map* header;

  struct growData* rawHeader;
  struct growData* rawBody;
};

struct Context* newContext(int clientFd);

int cleanContext(struct Context* ctx);