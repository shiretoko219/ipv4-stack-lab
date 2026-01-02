// netif.h
/* In this project, our L3 layer does not implement ARP; it only retains
 * the ability to notify the lower layer of the IP datagram and the IP address.
 * In the test code, we introduce the handler to dump the .bin file.
 * This header defines an l2_output_fn, which serves as a
 * mocked lower-layer interface and is not suitable for real-world use.
 */

#ifndef NETIF_H
#define NETIF_H

#include <stddef.h>
#include <stdint.h>

typedef struct netif netif_t;

typedef enum { ETH_IPV4 = 0x0800, ETH_ARP = 0x0806 } ehthernet_protocol;

typedef void (*l2_input_fn)(netif_t *n, uint16_t ethertype,
                            const uint8_t *payload, size_t len);

typedef void (*l2_output_fn)(netif_t *n, uint32_t next_hop_ip,
                             const uint8_t *ip_pkt, size_t ip_len);

struct netif {
  uint8_t mac[6];
  uint32_t ip;
  uint32_t netmask;
  uint32_t gateway;
  uint16_t mtu;

  l2_input_fn l2_input;
  l2_output_fn l2_output;
};

void netif_init(netif_t *n);

#endif // NETIF_H
