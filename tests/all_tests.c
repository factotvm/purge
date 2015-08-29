#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void elide_handles_short_strings();
void elide_handles_long_strings();
void test_format_bytes();

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(elide_handles_short_strings),
    cmocka_unit_test(elide_handles_long_strings),
    cmocka_unit_test(test_format_bytes)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
