//test_io.c

#include "test_io.h"
#include <stdio.h>
#include <stdlib.h>

int read_file(const char *path, uint8_t **out_buf, size_t *out_len) {
  FILE *f = fopen(path, "rb");
  if (f == NULL) {
    perror("fopen");
    return 0;
  }
  fseek(f, 0, SEEK_END);
  long sz = ftell(f);
  fseek(f, 0, SEEK_SET);

  uint8_t *buf = malloc(sz);
  if (buf == NULL) {
    fclose(f);
    return -1;
  }

  fread(buf, 1, sz, f);
  fclose(f);

  *out_buf = buf;
  *out_len = (size_t)sz;
  return 0;
}
