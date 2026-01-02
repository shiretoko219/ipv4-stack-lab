// test_io.h

#ifndef TEST_IO_H
#define TEST_IO_H

#include <stdint.h>
#include <stddef.h>

int read_file(const char *path, uint8_t **out_buf, size_t *out_len);

#endif // TEST_IO_H
