#pragma once

/* find out the null line */
char* endOfHeader(const char* srcStr);

/* trim front and back space char & '\r' '\n' */
char* trim(char* str);

/////////////////////////
// growData with malloc()
struct growData {
  size_t size;
  char* data;
};

struct growData* newGrowData(size_t size);

int appendGrowData(struct growData* gd, const char* data, size_t n);

int cleanGrowData(struct growData* gd);
////////////////////////

int readFile(const char* filePath, struct growData* gd);