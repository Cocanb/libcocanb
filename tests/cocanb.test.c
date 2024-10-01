#include "cocanb.h"
#include <check.h>
#include <stdlib.h>

START_TEST(test_simple_cocanb)
{
  const char *input = "Hello world!";
  ck_assert_str_eq(cocanb_encode(input), "hellworlnonoede!");
}
END_TEST

int main()
{
  int n_failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("Cocánb");
  tc = tcase_create("Core");
  tcase_add_test(tc, test_simple_cocanb);
  suite_add_tcase(s, tc);

  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
