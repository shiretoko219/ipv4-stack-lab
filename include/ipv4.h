//ipv4.h
/* Copyright (c) 2024 Francesco Cozzuto
* This code is licensed under the MIT License. See LICENSE file for details.
* The ip_protocol_t part was taken from 'cozis/microtcp' project and is licensed
* under MIT
* link: https://github.com/cozis/microtcp
* You should refer to the 'cozis/microtcp' project for those specific lines of
code.

* This header file only defines the IPv4 header structure.
* All IP address constants are expressed in host byte order
* Using human-readable big-endian values
* Packet fields must be converted with ntohl() before comparison.
*/

#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>

struct ipv4_header {
  uint8_t ver_ihl;
  uint8_t tos;
  uint16_t total_length;
  uint16_t id;
  uint16_t frag_offset;
  uint8_t ttl;
  uint8_t protocol;
  uint16_t checksum;
  uint32_t src_ip;
  uint32_t dest_ip;
} __attribute__((packed));

#define IP_VERSION 4
#define IPV4_MIN_HEADER_LEN 20
#define IPV4_MAX_LEN 0xFFFF
#define IPV4_MAX_HEADER_LEN 60
#define IPV4_DEFAULT_TTL 64
#define IPV4_STD_NO_FRAG 0x0000
#define IPV4_STD_DF 0x4000

typedef enum {
  IPV4_LOOPBACK_NETID = 0x7F000000,
  IPV4_LOOPBACK_NETMASK = 0xFF000000,
  IPV4_BROADCAST = 0xFFFFFFFF,
  IPV4_A_LOCAL_NETID = 0x0A000000,
  IPV4_A_NETMASK = 0xFF000000,
  IPV4_B_LOCAL_NETID = 0xAC100000,
  IPV4_B_LOCAL_NETMASK = 0xFFF00000,
  IPV4_C_LOCAL_NETID = 0xC0A80000,
  IPV4_C_LOCAL_NETMASK = 0xFFFF0000,
  IPV4_RSV_ADDR = 0x00000000
} ip_useful_addr;

typedef enum {
  IP_PROTOCOL_ICMP = 1,
  IP_PROTOCOL_TCP = 6,
  IP_PROTOCOL_UDP = 17,
} ip_protocol_t;

typedef enum {
  IPV4_OK = 0,
  IPV4_TOO_SMALL = -1,
  IPV4_TOO_BIG = -2,
  IPV4_ERROR_INVALID_VERSION = -3,
  IPV4_ERROR_INVALID_IHL = -4,
  IPV4_ERROR_INVALID_CHECKSUM = -5,
  IPV4_ERROR_UNKNOWN_PROTOCOL = -6,
  IPV4_ERROR_INVALID_TOTAL_LENGTH = -7,
  IPV4_ERROR_TIME_EXCEEDED = -8,
} ip_rcv_error;

typedef enum {
  IPV4_SEND_OK = 0,
  IPV4_SEND_TOO_LARGE = -1,
  IPV4_SEND_NO_MEMORY = -2,
  IPV4_SEND_ERR = -3,
  IPV4_NO_SUCH_PROTOCOL = -4
} ip_internal_error;

typedef enum {
  IP_STAT_LOOPBACK = 1,
  IP_STAT_LOCAL = 2,
  IP_STAT_BRDCAST = 3
} ip_stat;

#endif // IPV4_H

