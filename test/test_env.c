//test_env.c
//This section configures basic IP information of the machine.
//It should be configured manually.

#include <string.h>
#include "netif.h"
#include "eth_fake.h"
#include "test_env.h"
#include "ipv4_general.h"
#include "netstack_init.h"

void test_env(netif_t *n) {
  netif_init(n);
  uint8_t mac[6] = {0x02, 0x00, 0x00, 0x00, 0x00, 0x00};
  memcpy(n->mac, mac, 6);

  n->ip = htonl(inet_addr("0.0.0.0"));  //At your wish
  n->netmask = 0xffffffff;
  n->gateway = 0;
  netstack_init();
  eth_fake_attach(n);
}
