// ipv4_general.c
// Tools box for all other func.

#include "ipv4_general.h"
#include "checksum.h"
#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

uint16_t ipv4_checksum(const struct ipv4_header *header) {
  uint16_t header_length = (uint16_t)((header->ver_ihl & 0x0F) * 4);
  return checksum(header, header_length);
}

uint16_t ntohs(uint16_t data) { return (data >> 8) | (data << 8); }

uint16_t htons(uint16_t data) { return (data << 8) | (data >> 8); }

uint32_t ntohl(uint32_t data) {
  return ((data & 0x000000FF) << 24) | ((data & 0x0000FF00) << 8) |
         ((data & 0x00FF0000) >> 8) | ((data & 0xFF000000) >> 24);
}

uint32_t htonl(uint32_t data) {
  return ((data & 0x000000FF) << 24) | ((data & 0x0000FF00) << 8) |
         ((data & 0x00FF0000) >> 8) | ((data & 0xFF000000) >> 24);
}

uint32_t inet_addr(const char *c) {
  uint32_t addr = 0;
  uint32_t part = 0;
  int dots = 0;

  while (*c) {
    if (*c >= '0' && *c <= '9') {
      part = part * 10 + (*c - '0');
      if (part > 255)
        return 0xFFFFFFFF; // INADDR_NONE
    } else if (*c == '.') {
      addr = (addr << 8) | part;
      part = 0;
      dots++;
      if (dots > 3)
        return 0xFFFFFFFF;
    } else {
      return 0xFFFFFFFF;
    }
    c++;
  }

  if (dots != 3)
    return 0xFFFFFFFF;

  addr = (addr << 8) | part;

  return htonl(addr);
}

char *inet_ntoa(uint32_t ip, char *buf, size_t buflen) {
  if (buf == NULL || buflen < 16) {
    return NULL;
  }

  unsigned char b1 = (ip >> 24) & 0xFF;
  unsigned char b2 = (ip >> 16) & 0xFF;
  unsigned char b3 = (ip >> 8) & 0xFF;
  unsigned char b4 = ip & 0xFF;

  snprintf(buf, buflen, "%u.%u.%u.%u", b1, b2, b3, b4);
  return buf;
}
