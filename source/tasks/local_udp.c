/*
 * local_udp.c
 *
 *  Created on: 28 de ago de 2017
 *      Author: dkrepsky
 */

#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/mem.h"
#include "lwip/netif.h"
#include "lwip/sys.h"
#include "lwip/timers.h"
#include "lwip/inet_chksum.h"
#include "lwip/netifapi.h"
#include "lwip/api.h"
#include "lwip/inet.h"

#include "tasks/measure.h"

#include "local_udp.h"

void keep_alive(struct netconn *sock, struct netbuf *buf);
void get_measure(struct netconn *sock, struct netbuf *buf);

void taskLocalUdp(void *args) {

  struct netconn *sock = netconn_new(NETCONN_UDP);
  struct netbuf *buf;

  netconn_bind(sock, IP_ADDR_ANY, 7635);

  while (true) {

    if (netconn_recv(sock, &buf) != ERR_OK) {
      continue;
    }

    uint8_t *data;
    uint16_t len;

    netbuf_data(buf, (void **) &data, &len);

    if (data[0] == LOCAL_REQ_HEADER) {
      switch (data[1]) {
      case LOCAL_REQ_KEEP_ALIVE:
        keep_alive(sock, buf);
        break;
      default:
        break;
      }
    }

    netbuf_delete(buf);
  }
}

void keep_alive(struct netconn *sock, struct netbuf *buf) {
  uint8_t *data;
  uint16_t len;

  netbuf_data(buf, (void **) &data, &len);

  if (len >= 3) {
    data[0] = LOCAL_RES_HEADER;
    data[1] = LOCAL_RES_KEEP_ALIVE;
    data[2] = 0x00;
    netconn_send(sock, buf);
  }
}

void get_measure(struct netconn *sock, struct netbuf *buf) {
  uint8_t *data;
  uint16_t len;

  netbuf_data(buf, (void **) &data, &len);
}
