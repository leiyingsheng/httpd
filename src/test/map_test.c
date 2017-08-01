#include <CUnit/Basic.h>
#include <stdio.h>

#include "../map.h"

/*
* test for map.c
*/

void basic_opera() {
  struct map* mMap = newMap();
  CU_ASSERT(NULL != mMap);

  map_set(mMap, "test", "testval");
  CU_ASSERT_STRING_EQUAL((char*)map_get(mMap, "test"), "testval");
}

void reset_map() {
  struct map* mMap = newMap();
  CU_ASSERT(NULL != mMap);

  map_set(mMap, "test1", "testval1");
  // printf("%s\n", (char*)map_get(mMap, "test1"));
  CU_ASSERT_STRING_EQUAL((char*)map_get(mMap, "test1"), "testval1");

  map_set(mMap, "test1", "testval2");
  // printf("%s\n", (char*)map_get(mMap, "test1"));
  CU_ASSERT_STRING_EQUAL((char*)map_get(mMap, "test1"), "testval2");
}

void clean_map() {
  struct map* mMap = newMap();
  CU_ASSERT(NULL != mMap);

  map_set(mMap, "test", "testval");
  CU_ASSERT_STRING_EQUAL((char*)map_get(mMap, "test"), "testval");

  map_clean(mMap);
  CU_ASSERT(NULL == map_get(mMap, "test"));
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
      NULL == CU_add_test(pSuite, "clean operation", clean_map)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  /* Run all tests using the CUnit Basic interface */
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  CU_cleanup_registry();
  return CU_get_error();
}