// fake_tcp_udp.c

#include "ipv4_handlers.h"
#include "netif.h"
#include <stdint.h>
#include <stdio.h>

#ifndef IP_PROTOCOL_TCP
#define IP_PROTOCOL_TCP 6
#endif

#ifndef IP_PROTOCOL_UDP
#define IP_PROTOCOL_UDP 17
#endif

static int write_bin(const char *path, const uint8_t *buf, size_t n) {
  FILE *file = fopen(path, "wb");
  if (file == NULL) {
    perror("fopen");
    return -1;
  }

  size_t w = fwrite(buf, 1, n, file);
  fclose(file);

  if (w != n) {
    perror("fwrite");
    return -1;
  }
  return 0;
}

static int fake_tcp_input(netif_t *n, const uint8_t *payload, size_t len,
                          uint32_t dst_ip, uint32_t src_ip) {
  (void)n;
  (void)dst_ip;
  (void)src_ip;

  static unsigned int tcp_seq = 0;
  tcp_seq++;

  char filename[64];
  snprintf(filename, sizeof(filename), "./targets/tcp_%03u.bin", tcp_seq);

  printf("[fake_tcp] #%03u  len=%zu bytes\n", tcp_seq, len);

  return write_bin(filename, payload, len);
}

static int fake_udp_input(netif_t *n, const uint8_t *payload, size_t len,
                          uint32_t dst_ip, uint32_t src_ip) {
  (void)n;
  (void)dst_ip;
  (void)src_ip;

  static unsigned int udp_seq = 0;
  udp_seq++;

  char filename[64];
  snprintf(filename, sizeof(filename), "./targets/udp_%03u.bin", udp_seq);

  printf("[fake_udp] #%03u  len=%zu bytes\n", udp_seq, len);

  return write_bin(filename, payload, len);
}

int fake_tcp_udp_init(void) {
  int r1 = ipv4_register_protocol(IP_PROTOCOL_TCP, fake_tcp_input);
  int r2 = ipv4_register_protocol(IP_PROTOCOL_UDP, fake_udp_input);
  return (r1 != 0) ? r1 : r2;
}
