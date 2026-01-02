// This function only performs basic initialization.

#include "netif.h"
#include <string.h>

void netif_init(netif_t *n) {
  memset(n, 0, sizeof(*n));
  n->mtu = 1500;
}
