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

START_TEST(test_basic_quotes)
{
  char *output;
  const char *input = "Hello \"world\"";
  ck_assert_int_eq(cocanb_encode(input, &output, 1), 0);
  ck_assert_str_eq(output, "hell\"worlnonde\"nonoe");
}
END_TEST

START_TEST(test_2_nested_quotes)
{
  char *output;
  const char *input = "Hello \"foo \'bar\'\"";
  ck_assert_int_eq(cocanb_encode(input, &output, 2), 0);
  ck_assert_str_eq(output, "hell\"fo\'banonrc\'nonoc\"nonoe");
}
END_TEST

START_TEST(test_3_nested_quotes)
{
  char *output;
  const char *input = "Hello \"foo \'bar \"baz\"\'\"";
  ck_assert_int_eq(cocanb_encode(input, &output, 3), 0);
  ck_assert_str_eq(output, "hell\"fo\'ba\"banonzc\"nonrc\'nonoc\"nonoe");
}
END_TEST

START_TEST(test_quotes_with_punctuation)
{
  char *output;
  const char *input = "Hello \"foo. bar.\"";
  ck_assert_int_eq(cocanb_encode(input, &output, 1), 0);
  ck_assert_str_eq(output, "hell\"fononoc.banonrc.\"nonoe");
}
END_TEST

START_TEST(test_quote_overflow)
{
  char *output;
  const char *input = "Hello \"foo \'bar \"baz\"\'\"!";
  ck_assert_int_eq(cocanb_encode(input, &output, 1), 1);
}
END_TEST

START_TEST(test_unclosed_quote)
{
  char *output;
  const char *input = "Hello \"foo";
  ck_assert_int_eq(cocanb_encode(input, &output, 1), 1);
}
END_TEST

int main()
{
  int n_failed;
  Suite *s;
  TCase *tc;
  SRunner *sr;

  s = suite_create("Cocánb Quote Tests");
  tc = tcase_create("Quotes");
  tcase_add_test(tc, test_basic_quotes);
  tcase_add_test(tc, test_2_nested_quotes);
  tcase_add_test(tc, test_3_nested_quotes);
  tcase_add_test(tc, test_quotes_with_punctuation);
  tcase_add_test(tc, test_quote_overflow);
  tcase_add_test(tc, test_unclosed_quote);
  suite_add_tcase(s, tc);

  sr = srunner_create(s);

  srunner_run_all(sr, CK_NORMAL);
  n_failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (n_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
