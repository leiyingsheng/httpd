#pragma once
#include "context.h"

extern int errno;

int handle_request(int clientFd);

int read_all(struct context* ctx);

int parse_header(struct context* ctx);