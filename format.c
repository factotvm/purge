#include "format.h"
#include <string.h>

int elide(char *buffer, size_t buf_size, char *source)
{
  static const char ELLIPSIS[] = "...";
  static const int ELLIPSIS_LEN = sizeof(ELLIPSIS) - sizeof(char);

  size_t src_len = strlen(source);
  size_t len = buf_size - 1;
  if (src_len <= len) {
    for (size_t i = 0; i < len; i++) {
      buffer[i] = source[i] ? source[i] : '\0';
    }
  }
  else {
    size_t i;
    for (i = 0; i < len - ELLIPSIS_LEN; i++) {
      buffer[i] = source[i];
    }
    buffer[i] = '\0';
    strcat(buffer, ELLIPSIS);
  }
  buffer[buf_size] = '\0';

  return strlen(buffer);
}
