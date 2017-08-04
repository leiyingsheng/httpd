#pragma once

#include "map.h"
#include "utils.h"

struct respone {
  char* protocol;
  unsigned char statusCode;
  char* message;
  struct map* header;

  struct growData* body;
};

char* encodeRespone(struct respone* resp);