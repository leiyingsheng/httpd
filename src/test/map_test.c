#include <CUnit/Basic.h>
#include <stdio.h>
#include <string.h>

#include "../map.h"

/*
* test for map.c
*/

void basic_opera() {
  struct Map* mMap = newMap();
  CU_ASSERT(NULL != mMap);

  setMap(mMap, "test", strdup("testval"));
  CU_ASSERT_STRING_EQUAL((char*)getMap(mMap, "test"), "testval");
}

void reset_map() {
  struct Map* mMap = newMap();
  CU_ASSERT(NULL != mMap);

  setMap(mMap, "test1", strdup("testval1"));
  // printf("%s\n", (char*)getMap(mMap, "test1"));
  CU_ASSERT_STRING_EQUAL((char*)getMap(mMap, "test1"), "testval1");

  setMap(mMap, "test1", strdup("testval2"));
  // printf("%s\n", (char*)getMap(mMap, "test1"));
  CU_ASSERT_STRING_EQUAL((char*)getMap(mMap, "test1"), "testval2");
}

void test_cleanMap() {
  struct Map* mMap = newMap();
  CU_ASSERT(NULL != mMap);

  setMap(mMap, "test", strdup("testval"));
  CU_ASSERT_STRING_EQUAL((char*)getMap(mMap, "test"), "testval");

  cleanMap(mMap);
  CU_ASSERT(NULL == getMap(mMap, "test"));
}
// int init_suite() {
//   return 0;
// }

// int clean_suite() {
//   return 0;
// }

int main(void) {
  CU_pSuite pSuite = NULL;

  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry())
    return CU_get_error();

  /* add a suite to the registry */
  pSuite = CU_add_suite("map-Suite", NULL, NULL);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* add the tests to the suite */
  if (NULL == CU_add_test(pSuite, "basic operation", basic_opera) ||
      NULL == CU_add_test(pSuite, "reset map", reset_map) ||
      NULL == CU_add_test(pSuite, "clean operation", test_cleanMap)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}