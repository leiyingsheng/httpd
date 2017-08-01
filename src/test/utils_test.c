#include "../utils.h"
#include <CUnit/Basic.h>
#include <stdio.h>

/*
* test for main.c
*/

void test_main() {
  printf("I'm testing  main\n");
  mLog();
  CU_ASSERT(1);
}

int RunTest() { test_main(); }

int main(void) { return RunTest(); }