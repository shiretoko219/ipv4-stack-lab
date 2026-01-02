// ipv4_output.c

#include "ipv4_output.h"
#include "ipv4.h"
#include "ipv4_general.h"
#include <stdlib.h>
#include <string.h>

// Not thread-safe;
static uint16_t ip_id = 0;

static uint16_t ip_next_id(void) { return ip_id++; }

int ipv4_output(const uint8_t *data, netif_t *n, size_t len, uint8_t protocol,
                uint32_t dst, bool df) {
  uint16_t ihl = IPV4_MIN_HEADER_LEN;
  uint16_t total = ihl + len;

  if (df == true && total > n->mtu) {
    return IPV4_SEND_TOO_LARGE;
  }

  if (df == false && total > n->mtu) {
    // Fragmentation handling is left here; a error is returned for now.
    return IPV4_SEND_ERR;
  }

  uint8_t *buf = malloc(total);
  if (buf == NULL)
    return IPV4_SEND_NO_MEMORY;

  struct ipv4_header *header = (struct ipv4_header *)buf;

  // The following code loads the header.
  header->ver_ihl = (IP_VERSION << 4) | (ihl / 4);
  header->tos = 0x0;
  header->total_length = htons(total);
  header->id = htons(ip_next_id());
  if (df) {
    header->frag_offset = htons(IPV4_STD_DF);
  } else {
    header->frag_offset = IPV4_STD_NO_FRAG;
  }
  header->ttl = IPV4_DEFAULT_TTL;
  header->protocol = protocol;
  header->checksum = 0x0;
  header->src_ip = htonl(n->ip);
  header->dest_ip = htonl(dst);
  uint16_t checksum_result = ipv4_checksum(header);
  header->checksum = checksum_result;

  memcpy(buf + ihl, data, len);

  n->l2_output(n, dst, buf, total);

  free(buf);
  return IPV4_SEND_OK;
}
