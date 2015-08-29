#include "format.h"
#include <stdio.h>
#include <stdlib.h>
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

int format_bytes(char *buffer, size_t buf_size, uint64_t bytes)
{
  struct Unit {
    char suffix[2];
    uint64_t divisor;
  };

  static struct Unit units[5] = {
    {"B", 1},
    {"KB", 1000L},
    {"MB", 1000*1000L},
    {"GB", 1000*1000*1000L},
    {"TB", 1000*1000*1000*1000L}
  };

  // set the buffer to the zero case
  strcpy(buffer, "0B\0");
  // must use int because size_t can't be -1 and that's needed
  for (int i = sizeof(units)/sizeof(struct Unit) - 1; i >= 0; i--) {
    double value = (double)bytes / units[i].divisor;
    if (value >= 1) {
      snprintf(buffer, buf_size, "%.2f%s", value, units[i].suffix);
      break;
    }
  }

  return strlen(buffer);
}
