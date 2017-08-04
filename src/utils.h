#pragma once

/* find out the null line */
char* endOfHeader(const char* srcStr);

/* trim front and back space char */
char* trim(char* str);

/////////////////////////
// growData with malloc()
struct growData {
  size_t size;
  char* data;
};

struct growData* newgrowData(size_t size);

int appendGrowData(struct growData* rd, const char* data, size_t n);

int cleanGrowData(struct growData* rd);
////////////////////////