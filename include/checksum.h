//checksum.h
// This function only computes a checksum; it is not IPv4-specific.

#ifndef CHECKSUM_H
#define CHECKSUM_H

#include <stdint.h>

uint16_t checksum(const void *data, uint16_t length);

#endif // CHECKSUM_H
