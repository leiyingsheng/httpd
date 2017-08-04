#pragma once

#include "map.h"
#include "utils.h"

struct respone {
  char* protocol;
  int statusCode;
  char* message;
  struct map* header;

  struct growData* body;
};

struct respone* newRespone(int statusCode);

int encodeRespone(int clientFd, struct respone* resp);

int cleanRespone(struct respone* resp);