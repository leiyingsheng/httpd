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

struct respone* newRespone(int statusCode) {
  struct respone* resp = (struct respone*)malloc(sizeof(struct respone));

  resp->statusCode = statusCode;
  resp->header = newMap();
  resp->body = newGrowData(BODY_INIT_SIZE);

  resp->encodedData = calloc(DATA_VECTOR_CAP, sizeof(struct iovec));
  resp->vecCap = DATA_VECTOR_CAP;
  resp->vecLen = 0;

  return resp;
}

// dump all respone data into encodeedData vector
int encodeRespone(struct respone* resp) {
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

// dump header into respone's encodedData vector
int encodeHeader(struct respone* resp) {
  char buf[BUFFER_SIZE];

  int i, pairLen;
  struct entry* p;
  struct map* m = resp->header;

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

      sprintf(buf, "%s: %s" CRLF, p->key, p->val);

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

int sendRespone(struct respone* resp, int clientFd) {
  int n;
  n = writev(clientFd, resp->encodedData, resp->vecLen);
  if (n <= 0) {
    perror("send respone");
  }

  return n;
}

int cleanRespone(struct respone* resp) {
  cleanMap(resp->header);
  cleanGrowData(resp->body);

  --resp->vecLen;
  while (resp->vecLen >= 0) {
    // to skip `CRLF`
    if (strcmp(resp->encodedData[resp->vecLen].iov_base, CRLF) == 0)
      continue;

    free(resp->encodedData[resp->vecLen].iov_base);
    --resp->vecLen;
  }
  free(resp->encodedData);

  free(resp);
  return 0;
}