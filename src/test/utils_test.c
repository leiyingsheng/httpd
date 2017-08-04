#include <CUnit/Basic.h>
#include <stdio.h>

#include "../utils.h"

/*
* test for utils.c
*/

void test_trim() {
  char str0[] = "  test string  ";
  CU_ASSERT_STRING_EQUAL(trim(str0), "test string");

  char str1[] = "test string  ";
  CU_ASSERT_STRING_EQUAL(trim(str1), "test string");

  char str2[] = "  test string";
  CU_ASSERT_STRING_EQUAL(trim(str2), "test string");
}

void test_endOfHeader() {
  char str0[] = "";
  CU_ASSERT(endOfHeader(str0) == NULL);

  char str1[] = "\r\n\r\n";  // TODO
  CU_ASSERT(endOfHeader(str1) == str1 + 1);

  char str2[] =
      "test\r\n"
      "\r\n"
      "test\r\n";
  CU_ASSERT(endOfHeader(str2) == str2 + 7);
}

int main(void) {
  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("utils-Suite", NULL, NULL);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if (NULL == CU_add_test(pSuite, "trim", test_trim) ||
      NULL == CU_add_test(pSuite, "endOfHeader", test_endOfHeader)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}