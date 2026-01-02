//

#include "netstack_init.h"
#include "../test/fake_tcp_udp.h"
#include "ipv4_handlers.h"

int netstack_init(void) {
  ipv4_init();
  fake_tcp_udp_init();
  return 0;
}
