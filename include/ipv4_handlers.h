// ipv4_handlers.h
//This file defines the L4 registration interface and the L4 dispatch interface.

#ifndef IPV4_HANDLERS_H
#define IPV4_HANDLERS_H

#include "netif.h"

typedef int (*ipv4_l4_handler_fn)(netif_t *n, const uint8_t *payload,
                                  size_t len, uint32_t dst_ip, uint32_t src_ip);

void ipv4_init(void);
int ipv4_register_protocol(uint8_t protocol, ipv4_l4_handler_fn handler);
int ipv4_up(netif_t *n, const uint8_t *ip_pkt);

#endif // IPV4_HANDLERS_H
