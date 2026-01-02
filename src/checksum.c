// checksum.c

#include "checksum.h"

uint16_t checksum(const void *data, uint16_t length) {
  uint16_t *buf = (uint16_t *)data;
  uint32_t sum = 0; // Use a 32‑bit accumulator since the preliminary sum can
                    // exceed 16 bits.

  for (; length > 1; length -= 2) {
    sum += *buf++;
  }
  if (length == 1) {
    sum += *(uint8_t *)buf;
  }

  sum = (sum >> 16) + (sum & 0xFFFF);
  sum += (sum >> 16);

  return (~sum);
}