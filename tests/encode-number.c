/**
 * libcocanb - Encodes texts into Cocánb
 * Copyright (C) 2024 salade malade
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "cocanb.h"
#include <check.h>
#include <stdlib.h>

START_TEST(test_number_separate)
{
  char *output;
  const char *input = "I have 5 apples.";
  ck_assert_int_eq(cocanb_encode(input, &output, 1), 0);
  ck_assert_str_eq(output, "hav5applenoniaedsf.");
}
END_TEST

START_TEST(test_number_embedded)
{
  char *output;
  const char *input = "he110";
  ck_assert_int_eq(cocanb_encode(input, &output, 1), 0);
  ck_assert_str_eq(output, "henon110c");
}
END_TEST

int main()
{
  int n_failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("Cocánb Number Tests");
  tc = tcase_create("Number");
  tcase_add_test(tc, test_number_separate);
  tcase_add_test(tc, test_number_embedded);
  suite_add_tcase(s, tc);

  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
