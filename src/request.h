#pragma once

#include "context.h"

/* https://www.w3.org/Protocols/rfc2616/rfc2616-sec5.html#sec5 */
#define MTD_OPTIONS "OPTIONS"
#define MTD_GET "GET"
#define MTD_HEAD "HEAD"
#define MTD_POST "POST"
#define MTD_PUT "PUT"
#define MTD_DELETE "DELETE"
// ...

int readRequest(struct context* ctx);

int parseHeader(struct context* ctx);