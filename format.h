#ifndef FORMAT_H
#define FORMAT_H

#include <stddef.h>
#include <stdlib.h>

int elide(char *buffer, size_t buf_size, char *source);
int format_bytes(char *buffer, size_t buf_size, uint64_t bytes);

#endif
