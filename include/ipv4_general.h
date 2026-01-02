//ipv4_general.h
// This file contains functions shared by both sending and receiving.

#ifndef IPV4_GENERAL_H
#define IPV4_GENERAL_H

#include "ipv4.h"
#include <stddef.h>

struct ipv4_header;

uint16_t ipv4_checksum(const struct ipv4_header *header);
uint16_t ntohs(uint16_t data);
uint16_t htons(uint16_t data);
uint32_t ntohl(uint32_t data);
uint32_t htonl(uint32_t data);
uint32_t inet_addr(const char *c);
char *inet_ntoa(uint32_t ip, char *buf, size_t buflen);

#endif // IPV4_GENERAL_H
