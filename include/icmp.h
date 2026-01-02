// icmp.h
//This file contains the ICMP-related header definitions.

#ifndef ICMP_H
#define ICMP_H

#include "netif.h"
#include <stddef.h>
#include <stdint.h>

struct icmp_header {
  uint8_t type;
  uint8_t code;
  uint16_t checksum;
} __attribute__((packed));

struct icmp_echo {
  uint16_t id;
  uint16_t seq;
  uint8_t *data;
};

#define ICMP_UNUSED_SIZE 4;

typedef enum {
  ICMP_TYPE_ECHO_REPLY = 0,
  ICMP_TYPE_DST_UNREACHABLE = 3,
  ICMP_TYPE_ECHO_REQUEST = 8,
  ICMP_TYPE_TIME_EXCEEDED = 11
} icmp_type;

typedef enum {
  ICMP_NET_UNREACHABLE = 0,
  ICMP_HOST_UNREACHABLE = 1,
  ICMP_PROTOCOL_UNREACHABLE = 2,
  ICMP_PORT_UNREACHABLE = 3,
  ICMP_FRAG_NEED = 4
} icmp_unreachable_code;

typedef enum {
  ICMP_TTL_EXCEEDED = 0,
  ICMP_FRAG_EXCEEDED = 1
} icmp_time_exceeded_code;

void icmp_dispatcher(const uint8_t *icmp, const uint8_t *org, uint8_t type,
                     size_t icmp_len, uint32_t reply_dst, size_t ihl,
                     netif_t *n);

#endif // ICMP_H
