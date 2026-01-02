//main.c
//Once all settings are complete, run main.c.

#include "ipv4.h"
#include "ipv4_output.h"
#include "netif.h"
#include "test_env.h"
#include "test_io.h"
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_IP 0x00000000  //At your wish

static void run_tests_dir(netif_t *n, const char *dir) {
  DIR *d = opendir(dir);
  struct dirent *e;

  while ((e = readdir(d)) != NULL) {
    if (e->d_name[0] == '.') {
      continue;
    }
    if (strstr(e->d_name, ".bin") == NULL) {
      continue;
    }

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dir, e->d_name);

    uint8_t *buf = NULL;
    size_t len = 0;

    if (read_file(path, &buf, &len) != 0) {
      continue;
    }

    printf("input: %s (%zu bytes)\n", path, len);
    // n->l2_input(n, ETH_IPV4, buf, len);
    ipv4_output(buf, n, len, IP_PROTOCOL_TCP, TEST_IP, true);
    free(buf);
  }

  closedir(d);
}

int main(void) {
  netif_t n;
  test_env(&n);
  run_tests_dir(&n, "./tests");
  return 0;
}
