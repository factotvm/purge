#include "../format.h"
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
