// ipv4_handlers.c

#include "ipv4_handlers.h"
#include "icmp.h"
#include "ipv4.h"
#include "ipv4_general.h"
#include "netif.h"
#include <string.h>

static ipv4_l4_handler_fn handlers[256];

void ipv4_init(void) { memset(handlers, 0, sizeof(handlers)); }

int ipv4_register_protocol(uint8_t protocol, ipv4_l4_handler_fn handler) {
  handlers[protocol] = handler;
  return 0;
}

int ipv4_up(netif_t *n, const uint8_t *ip_pkt) {
  struct ipv4_header *h = (struct ipv4_header *)ip_pkt;
  uint8_t protocol = h->protocol;
  size_t ihl = (h->ver_ihl & 0x0F) * 4;
  uint16_t total = ntohs(h->total_length);
  const uint8_t *payload = ip_pkt;
  size_t payload_len = (size_t)total - ihl;
  payload = payload + ihl;
  uint32_t src_ip = ntohl(h->src_ip);
  uint32_t des_ip = ntohl(h->dest_ip);

  if (protocol == IP_PROTOCOL_ICMP) {
    icmp_dispatcher(payload, ip_pkt, ICMP_TYPE_ECHO_REQUEST, payload_len,
                    src_ip, ihl, n);
    return IPV4_OK;
  }

  if (handlers[protocol] != NULL && protocol != IP_PROTOCOL_ICMP) {
    return handlers[protocol](n, payload, payload_len, des_ip, src_ip);
  }

  return IPV4_NO_SUCH_PROTOCOL;
}
