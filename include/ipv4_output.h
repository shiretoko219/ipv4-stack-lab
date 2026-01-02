//ipv4_output.h
// This header file only serves ipv4_output.h.

#ifndef IPV4_OUTPUT_H
#define IPV4_OUTPUT_H

#include "netif.h"
#include <stdbool.h>
#include <stdio.h>

int ipv4_output(const uint8_t *data, netif_t *n, size_t len, uint8_t protocol,
                uint32_t dst, bool df);

#endif // IPV4_OUTPUT_H
