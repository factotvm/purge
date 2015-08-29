#include "../format.h"
#include <math.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void elide_handles_short_strings()
{
  size_t buf_size = 10;
  char buffer[buf_size];
  char *source = "A";
  int result = elide(buffer, buf_size, source);

  assert_string_equal(source, buffer);
  assert_int_equal(1, result);
}

void elide_handles_long_strings()
{
  size_t buf_size = 10;
  char buffer[buf_size];
  char *source = "A really long string";
  int result = elide(buffer, buf_size, source);

  assert_string_equal("A real...", buffer);
  assert_int_equal(buf_size - 1, result);
}

void test_format_bytes()
{
  size_t size = 14;
  char actual[size];

  format_bytes(actual, size, 0);
  assert_string_equal("0B", actual);
  format_bytes(actual, size, 1);
  assert_string_equal("1.00B", actual);
  format_bytes(actual, size, 12);
  assert_string_equal("12.00B", actual);
  format_bytes(actual, size, 1000);
  assert_string_equal("1.00KB", actual);
  format_bytes(actual, size, 1234);
  assert_string_equal("1.23KB", actual);
  format_bytes(actual, size, pow(1000, 2));
  assert_string_equal("1.00MB", actual);
  format_bytes(actual, size, pow(1000, 3));
  assert_string_equal("1.00GB", actual);
  format_bytes(actual, size, pow(1000, 4));
  assert_string_equal("1.00TB", actual);
  format_bytes(actual, size, pow(1000, 4) * 123.45);
  assert_string_equal("123.45TB", actual);
  format_bytes(actual, size, UINT64_MAX);
  assert_string_equal("18446744.07TB", actual);
}
