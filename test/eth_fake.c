// eth_fack.c
/* This section mocks a network interface and its implementation;
 *  it merely extracts binary input and does not represent a real NIC.
 */

#include "eth_fake.h"
#include "ipv4_general.h"
#include "ipv4_input.h"
#include "netif.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static int write_bin(const char *path, const uint8_t *buf, size_t n) {
  FILE *file = fopen(path, "wb");
  if (file == NULL) {
    perror("EOF!");
    return -1;
  }

  size_t w = fwrite(buf, 1, n, file);
  fclose(file);

  if (w != n) {
    perror("Bin file write Error!");
    return -1;
  }
  return 0;
}

static void fake_l2_input(netif_t *net_if, uint16_t ethertype,
                          const uint8_t *payload, size_t len) {
  // uint16_t type = ntohs(ethertype);  //Needed only in real world
  if (ethertype != ETH_IPV4) {
    return;
  }
  ipv4_process(payload, net_if, len);
}

static void fake_l2_output(netif_t *net_if, uint32_t next_hop_ip,
                           const uint8_t *ip_pkt, size_t ip_len) {
  (void)net_if;
  static unsigned seq = 0;

  char ipbuf[16];
  printf("L2 send: next hop = %s, len = %zu\n",
         inet_ntoa(next_hop_ip, ipbuf, sizeof(ipbuf)), ip_len);

  char outpath[256];
  snprintf(outpath, sizeof(outpath), "./targets/eth_%03u.bin", seq++);

  if (write_bin(outpath, ip_pkt, ip_len) == 0) {
    printf("IP write test OK! -> %s\n", outpath);
    return;
  } else {
    printf("Error on IP write test! -> %s\n", outpath);
    return;
  }
}

void eth_fake_attach(netif_t *n) {
  n->l2_input = fake_l2_input;
  n->l2_output = fake_l2_output;
}
