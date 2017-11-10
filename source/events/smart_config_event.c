/*
 * wifi_config.c
 *
 *  Created on: 2 de ago de 2017
 *      Author: dkrepsky
 */

#include  <stdbool.h>
#include "esp_common.h"

bool g_smartconfigFinished = false;

void CALLBACK_SmartConfig(sc_status status, void *pdata) {
  switch (status) {
  case SC_STATUS_WAIT:
    break;
  case SC_STATUS_FIND_CHANNEL:
    break;
  case SC_STATUS_GETTING_SSID_PSWD:
    break;
  case SC_STATUS_LINK:
    printf("[INFO] Received SSID and PSWD Info, testing WiFi.\n");

    struct station_config *sta_conf = pdata;
    wifi_station_set_config(sta_conf);
    wifi_station_disconnect();
    bool isConnected = wifi_station_connect();
    if (!isConnected) {
      printf("[ERROR] Wrong SSID info.");
    }

    break;

  case SC_STATUS_LINK_OVER:
    if (pdata != NULL) {
      printf("[INFO] Configuration received!");
      g_smartconfigFinished = true;
    }
    break;
  }
}
