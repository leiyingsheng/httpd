#pragma once

#include <unistd.h>

#include "map.h"
#include "utils.h"

/* https://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6 */
#define STAT_SWITCHING_PROTOCOLS 101
#define STAT_OK 200
#define STAT_BAD_REQUEST 400
#define STAT_FORBIDEN 403
#define STAT_NOT_FOUND 404
#define STAT_METHOD_NOT_ALLOWED 405
#define STAT_INTERNAL_ERR 500
#define STAT_NOT_IMPLEMENTED 501
#define STAT_HTTP_VER_NOT_SUPPORTED 505
//...

struct respone {
  char* protocol;
  int statusCode;
  char* message;
  struct map* header;

  struct growData* body;

  struct iovec* encodedData;  // data ready to send
  int vecCap;                 // capital of dataVec
  int vecLen;                 // current dataVec length
};

struct respone* newRespone(int statusCode);

int encodeRespone(struct respone* resp);
int encodeHeader(struct respone* resp);
int sendRespone(struct respone* resp, int clientFd);

int cleanRespone(struct respone* resp);

