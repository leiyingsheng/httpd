#pragma once

#include "context.h"

extern int errno;

int readRequest(struct context* ctx);

int parseHeader(struct context* ctx);