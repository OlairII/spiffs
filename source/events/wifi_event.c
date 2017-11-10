/*
 * wifi_event.c
 *
 *  Created on: 2 de ago de 2017
 *      Author: dkrepsky
 */

#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"

#include "tasks/net_status.h"

void CALLBACK_Wifi(System_Event_t *pEvent) {
  switch (pEvent->event_id) {
  case EVENT_STAMODE_CONNECTED:
    os_printf("Event: EVENT_STAMODE_CONNECTED\n");
    break;
  case EVENT_STAMODE_DISCONNECTED:
    os_printf("Event: EVENT_STAMODE_DISCONNECTED\n");
    if (getIsLanConnected()) {
      netStatusUpdate();
    }
    break;
  case EVENT_STAMODE_AUTHMODE_CHANGE:
    os_printf("Event: EVENT_STAMODE_AUTHMODE_CHANGE\n");
    break;
  case EVENT_STAMODE_GOT_IP:
    os_printf("[Event] EVENT_STAMODE_GOT_IP\n");
    if (!getIsLanConnected())
      netStatusUpdate();
    break;
  case EVENT_SOFTAPMODE_STACONNECTED:
    os_printf("Event: EVENT_SOFTAPMODE_STACONNECTED\n");
    break;
  case EVENT_SOFTAPMODE_STADISCONNECTED:
    os_printf("Event: EVENT_SOFTAPMODE_STADISCONNECTED\n");
    break;
  default:
    os_printf("Unexpected event: %d\n", pEvent->event_id);
    break;
  }
}
