//icmp.c
// This part handles ICMP.

#include "icmp.h"
#include "checksum.h"
#include "ipv4.h"
#include "ipv4_handlers.h"
#include "ipv4_output.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static void icmp_echo(const uint8_t *icmp, size_t icmp_len, uint32_t dst,
                      netif_t *n) {
  uint8_t *reply_buf = (uint8_t *)malloc(icmp_len);

  if (reply_buf == NULL) {
    return;
  }

  memcpy(reply_buf, icmp, icmp_len);

  struct icmp_header *reply = (struct icmp_header *)reply_buf;
  reply->type = ICMP_TYPE_ECHO_REPLY;
  reply->code = 0x0; // The code field is always 0 for echo messages.
  reply->checksum = 0x0;
  uint16_t new_csm = checksum(reply_buf, icmp_len);
  reply->checksum = new_csm;

  ipv4_output(reply_buf, n, icmp_len, IP_PROTOCOL_ICMP, dst, false);
  free(reply_buf);
}

static void icmp_pro_unreach(const uint8_t *org, size_t ihl, uint32_t dst,
                             netif_t *n) {
  if ((dst & IPV4_LOOPBACK_NETMASK) == IPV4_LOOPBACK_NETID) {
    return;
  }

  if (dst == 0) {
    return;
  }

  size_t reply_size = sizeof(struct icmp_header) + ihl +
                      12; // 12 bytes include unused and original data section
  struct icmp_header *reply = (struct icmp_header *)calloc(1, reply_size);
  // Fields not explicitly used are zero-initialized by calloc.
  if (reply == NULL) {
    return;
  }

  reply->type = ICMP_TYPE_DST_UNREACHABLE;
  reply->code = ICMP_PROTOCOL_UNREACHABLE;
  reply->checksum = 0x0;
  uint8_t *data =
      (uint8_t *)reply + sizeof(struct icmp_header) + ICMP_UNUSED_SIZE;

  memcpy(data, org, ihl + 8);
  uint16_t csm = checksum(reply, reply_size);
  reply->checksum = csm;

  const uint8_t *payload = (uint8_t *)reply;
  ipv4_output(payload, n, reply_size, IP_PROTOCOL_ICMP, dst, false);

  free(reply);
}

static void icmp_ttl_exceed(const uint8_t *org, size_t ihl, uint32_t dst,
                            netif_t *n) {
  if ((dst & IPV4_LOOPBACK_NETMASK) == IPV4_LOOPBACK_NETID) {
    return;
  }

  if (dst == 0) {
    return;
  }

  size_t reply_size = sizeof(struct icmp_header) + ihl +
                      12; // 12 bytes include unused and original data section
  struct icmp_header *reply = (struct icmp_header *)calloc(1, reply_size);
  // Fields not explicitly used are zero-initialized by calloc.
  if (reply == NULL) {
    return;
  }

  reply->type = ICMP_TYPE_TIME_EXCEEDED;
  reply->code = ICMP_TTL_EXCEEDED;
  reply->checksum = 0x0;
  uint8_t *data =
      (uint8_t *)reply + sizeof(struct icmp_header) + ICMP_UNUSED_SIZE;

  memcpy(data, org, ihl + 8);
  uint16_t csm = checksum(reply, reply_size);
  reply->checksum = csm;

  const uint8_t *payload = (uint8_t *)reply;
  ipv4_output(payload, n, reply_size, IP_PROTOCOL_ICMP, dst, false);

  free(reply);
}

static void icmp_frag_needed(const uint8_t *org, size_t ihl, uint32_t dst,
                             netif_t *n) {
  if ((dst & IPV4_LOOPBACK_NETMASK) == IPV4_LOOPBACK_NETID) {
    return;
  }

  if (dst == 0) {
    return;
  }

  size_t reply_size = sizeof(struct icmp_header) + ihl +
                      12; // 12 bytes include unused and original data section
  struct icmp_header *reply = (struct icmp_header *)calloc(1, reply_size);
  // Fields not explicitly used are zero-initialized by calloc.
  if (reply == NULL) {
    return;
  }

  reply->type = ICMP_TYPE_DST_UNREACHABLE;
  reply->code = ICMP_FRAG_NEED;
  reply->checksum = 0x0;
  uint8_t *data =
      (uint8_t *)reply + sizeof(struct icmp_header) + ICMP_UNUSED_SIZE;

  memcpy(data, org, ihl + 8);
  uint16_t csm = checksum(reply, reply_size);
  reply->checksum = csm;

  const uint8_t *payload = (uint8_t *)reply;
  ipv4_output(payload, n, reply_size, IP_PROTOCOL_ICMP, dst, false);

  free(reply);
}

void icmp_dispatcher(const uint8_t *icmp, const uint8_t *org, uint8_t type,
                     size_t icmp_len, uint32_t reply_dst, size_t ihl,
                     netif_t *n) {
  const struct icmp_header *icmp_h = (const struct icmp_header *)icmp;

  if (icmp_h->type == ICMP_TYPE_ECHO_REQUEST) {
    type = ICMP_TYPE_ECHO_REQUEST;
  }

  switch (type) {
  case ICMP_TYPE_ECHO_REQUEST:
    icmp_echo(icmp, icmp_len, reply_dst, n);
    break;
  case ICMP_PROTOCOL_UNREACHABLE:
    icmp_pro_unreach(org, ihl, reply_dst, n);
    break;
  case ICMP_TTL_EXCEEDED:
    icmp_ttl_exceed(org, ihl, reply_dst, n);
    break;
  case ICMP_FRAG_NEED:
    icmp_frag_needed(org, ihl, reply_dst, n);
    break;
  default:
    return;
  }
}