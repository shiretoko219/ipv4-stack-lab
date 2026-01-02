// ipv4_input.c

#include "ipv4_input.h"
#include "icmp.h"
#include "ipv4_general.h"
#include "ipv4_handlers.h"
#include "netif.h"

int ipv4_check(const uint8_t *pkt, size_t len) {
  // Check if the length is valid
  if (len < IPV4_MIN_HEADER_LEN) {
    return IPV4_TOO_SMALL; // if it is too small, report an error
  }

  if (len > IPV4_MAX_LEN) {
    return IPV4_TOO_BIG; // if it is too big, report an error
  }

  // Check if the version number is valid
  struct ipv4_header *header = (struct ipv4_header *)pkt;

  if (header->ver_ihl >> 4 != IP_VERSION) {
    return IPV4_ERROR_INVALID_VERSION;
  }

  // Check if header_lenth is valid
  uint16_t header_length = (header->ver_ihl & 0x0F) * 4;
  ;

  if (header_length < IPV4_MIN_HEADER_LEN ||
      header_length > IPV4_MAX_HEADER_LEN) {
    return IPV4_ERROR_INVALID_IHL;
  }

  if (len < header_length)
    return IPV4_ERROR_INVALID_IHL;

  // Check the checksum
  if (ipv4_checksum(header) != 0) {
    return IPV4_ERROR_INVALID_CHECKSUM;
  }

  // Check if the protocol number is in the processing range, handler are not
  // called
  uint8_t protocol = header->protocol;

  switch (protocol) {
  case IP_PROTOCOL_ICMP:
    break;
  case IP_PROTOCOL_TCP:
    break;
  case IP_PROTOCOL_UDP:
    break;
  default:
    return IPV4_ERROR_UNKNOWN_PROTOCOL;
  }

  // Since the legality of len was checked earlier, here we only check if it
  // matches len. Endian conversion is performed here because our calculations
  // are done in little-endian format.
  uint16_t total = ntohs(header->total_length);

  if (total < header_length) {
    return IPV4_ERROR_INVALID_TOTAL_LENGTH;
  }

  if (len < total) {
    return IPV4_ERROR_INVALID_TOTAL_LENGTH;
  }

  if (header->ttl <= 1) {
    return IPV4_ERROR_TIME_EXCEEDED;
  }

  return IPV4_OK;
}

void ipv4_process(const uint8_t *pkt, netif_t *n, size_t len) {
  if (n == NULL || n->l2_output == NULL) {
    return;
  }

  int report = ipv4_check(pkt, len);
  struct ipv4_header *h = (struct ipv4_header *)pkt;
  size_t ihl = (h->ver_ihl & 0x0F) * 4;
  uint32_t dst = ntohl(h->dest_ip);
  
  if (report != IPV4_OK) {
    if (dst != n->ip && dst != IPV4_BROADCAST) {
      return;
    }

    uint32_t reply_dst = ntohl(h->src_ip);
    const uint8_t *icmp = pkt + ihl;

    switch (report) {
    case IPV4_ERROR_UNKNOWN_PROTOCOL:
      icmp_dispatcher(icmp, pkt, ICMP_PROTOCOL_UNREACHABLE, len - ihl,
                      reply_dst, ihl, n);
      return;
    case IPV4_ERROR_TIME_EXCEEDED:
      icmp_dispatcher(icmp, pkt, ICMP_TTL_EXCEEDED, len - ihl, reply_dst, ihl,
                      n);
      return;
    default:
      // Other errors, drop the packet.
      return;
    }
  }
  ipv4_up(n, pkt);
}
