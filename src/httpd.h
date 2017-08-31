#pragma once

#include "request.h"
#include "respone.h"

typedef struct respone* (*handleFunc)(struct context* ctx);

int setupListener();

void* handleRequest(void* clientFd);