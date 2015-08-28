#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

void elide_handles_short_strings();
void elide_handles_long_strings();

int main(void)
{
  const struct CMUnitTest tests[] = {
    cmocka_unit_test(elide_handles_short_strings),
    cmocka_unit_test(elide_handles_long_strings)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
