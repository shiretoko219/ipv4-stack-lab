//ipv4_input.h
// This header file only serves ipv4_input.h.

#ifndef IPV4_INPUT_H
#define IPV4_INPUT_H

#include <netif.h>
#include <stdio.h>

int ipv4_check(const uint8_t *pkt, size_t len);
void ipv4_process(const uint8_t *pkt, netif_t *n, size_t len);

#endif // IPV4_INPUT_H
