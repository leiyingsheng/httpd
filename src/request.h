#pragma once

#include "context.h"

int readRequest(struct context* ctx);

int parseHeader(struct context* ctx);