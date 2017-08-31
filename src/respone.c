#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/uio.h>
#include <unistd.h>

#include "respone.h"

#define BODY_INIT_SIZE 512
#define HEADER_PAIR_NUM 50
#define BUFFER_SIZE 128
#define DATA_VECTOR_CAP 10
#define CRLF "\r\n"

struct Respone* newRespone(int statusCode) {
  struct Respone* resp = (struct Respone*)malloc(sizeof(struct Respone));

  resp->statusCode = statusCode;
  resp->header = newMap();
  resp->body = newGrowData(BODY_INIT_SIZE);

  resp->encodedData = calloc(DATA_VECTOR_CAP, sizeof(struct iovec));
  resp->vecCap = DATA_VECTOR_CAP;
  resp->vecLen = 0;

  return resp;
}

// dump all Respone data into encodeedData vector
int encodeRespone(struct Respone* resp) {
  char statusLine[32];

  sprintf(statusLine, "%s %d %s" CRLF, resp->protocol, resp->statusCode,
          resp->message);  // status-line
  resp->encodedData[resp->vecLen].iov_base = strdup(statusLine);
  resp->encodedData[resp->vecLen].iov_len = strlen(statusLine);
  ++resp->vecLen;

  encodeHeader(resp);

  if (resp->vecLen >= resp->vecCap) {
    printf("run out of encodedData vector\n");
    return -1;
  }
  resp->encodedData[resp->vecLen].iov_base = strdup(resp->body->data);
  resp->encodedData[resp->vecLen].iov_len = strlen(resp->body->data);
  ++resp->vecLen;

  return 0;
}

// dump header into Respone's encodedData vector
int encodeHeader(struct Respone* resp) {
  char buf[BUFFER_SIZE];

  int i, pairLen;
  struct entry* p;
  struct Map* m = resp->header;

  for (i = 0; i < HASH_SIZE; ++i) {
    for (p = m->entryTab[i]; p != NULL; p = p->next) {
      pairLen = strlen(p->key) + sizeof ": " CRLF + strlen(p->val);

      if (pairLen > BUFFER_SIZE) {
        printf("pair too long\n");
        return -1;
      }

      if (resp->vecLen >= resp->vecCap) {
        printf("run out of encodedData vector\n");
        return -1;
      }

      sprintf(buf, "%s: %s" CRLF, p->key, (char*)p->val);

      resp->encodedData[resp->vecLen].iov_base = strdup(buf);
      resp->encodedData[resp->vecLen].iov_len = pairLen - 1;
      ++resp->vecLen;
    }
  }

  // break line
  resp->encodedData[resp->vecLen].iov_base = CRLF;
  resp->encodedData[resp->vecLen].iov_len = sizeof CRLF - 1;
  ++resp->vecLen;

  return 0;
}

int sendRespone(struct Respone* resp, int clientFd) {
  int n;
  n = writev(clientFd, resp->encodedData, resp->vecLen);
  if (n <= 0) {
    perror("send Respone");
  }

  return n;
}

int cleanRespone(struct Respone* resp) {
  cleanMap(resp->header);
  cleanGrowData(resp->body);

  --resp->vecLen;
  while (resp->vecLen >= 0) {
    // to skip `CRLF`
    if (strcmp(resp->encodedData[resp->vecLen].iov_base, CRLF) != 0) {
      free(resp->encodedData[resp->vecLen].iov_base);
    }

    --resp->vecLen;
  }
  free(resp->encodedData);

  free(resp);
  return 0;
}