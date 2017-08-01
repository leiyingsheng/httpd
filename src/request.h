#pragma once
#include "context.h"

extern int errno;

int handle_request(int clientFd);

int parse_header(struct context* ctx);