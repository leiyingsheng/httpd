#pragma once

#include "request.h"
#include "respone.h"

// typedef struct Respone* (*handleFunc)(struct Context* ctx);
typedef struct Respone* HandleFunc(struct Context* ctx);
typedef HandleFunc* HandleFuncPtr;
typedef HandleFuncPtr* HandleFuncPtrPtr;

int setupListener();

void* handleRequest(void* clientFd);

struct Respone* staticFile(struct Context* ctx);
// HandleFunc staticFile;