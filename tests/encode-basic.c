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

START_TEST(test_basic)
{
  char *output;
  const char *input = "Hello world";
  ck_assert_int_eq(cocanb_encode(input, &output, 0), 0);
  ck_assert_str_eq(output, "hellworlnonoede");
  cocanb_free(output);
}
END_TEST

START_TEST(test_punctuation)
{
  char *output;
  const char *input = "Hello world!";
  ck_assert_int_eq(cocanb_encode(input, &output, 0), 0);
  ck_assert_str_eq(output, "hellworlnonoede!");
  cocanb_free(output);
}
END_TEST

START_TEST(test_space_punctuation)
{
  char *output;
  const char *input = "Hello world !";
  ck_assert_int_eq(cocanb_encode(input, &output, 0), 0);
  ck_assert_str_eq(output, "hellworlnonoede!");
  cocanb_free(output);
}
END_TEST

START_TEST(test_multi_sentence)
{
  char *output;
  const char *input = "Hello! World?";
  ck_assert_int_eq(cocanb_encode(input, &output, 0), 0);
  ck_assert_str_eq(output, "hellnonoe!worlnonde?");
  cocanb_free(output);
}
END_TEST

int main()
{
  int n_failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("Cocánb Basic Tests");
  tc = tcase_create("Basic");
  tcase_add_test(tc, test_basic);
  tcase_add_test(tc, test_punctuation);
  tcase_add_test(tc, test_space_punctuation);
  tcase_add_test(tc, test_multi_sentence);
  suite_add_tcase(s, tc);

  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
