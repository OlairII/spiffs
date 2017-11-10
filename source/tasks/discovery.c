/*
 * discovery.c
 *
 *  Created on: 3 de set de 2017
 *      Author: dkrepsky
 */

#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/api.h"

#include "discovery.h"

void taskDiscovery(void *args) {
//  struct netconn *sock = netconn_new(NETCONN_UDP);
//  struct netbuf *buf;
//
//  netconn_bind(sock, IP_ADDR_ANY, 7636);
//
//  while (true) {
//
//    if (netconn_recv(sock, &buf) != ERR_OK) {
//      continue;
//    }
//
//    uint8_t *data;
//    uint16_t len;
//
//    netbuf_data(buf, (void **) &data, &len);
//
//    if (data[0] == LOCAL_REQ_HEADER) {
//      switch (data[1]) {
//      case LOCAL_REQ_KEEP_ALIVE:
//        keep_alive(sock, buf);
//        break;
//      default:
//        break;
//      }
//    }
//
//    netbuf_delete(buf);
//  }
}

