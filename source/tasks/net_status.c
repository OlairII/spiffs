/*
 * net_status.c
 *
 *  Created on: 20 de ago de 2017
 *      Author: dkrepsky
 */

#include <stdint.h>
#include <stdbool.h>
#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

#include "lwip/api.h"

#include "dev_config.h"
#include "driver/gpio.h"
#include "utils/ping/ping.h"

LOCAL const uint8_t LED_NOT_CONNECTED = 0x01;
LOCAL const uint8_t LED_LAN_CONNECTED = 0x02;
LOCAL const uint8_t LED_WEB_CONNECTED = 0x03;
LOCAL const uint8_t LED_SERVER_CONNECTED = 0x04;

LOCAL uint8_t ledStatus = 0;
LOCAL bool isLanConnected = false;
LOCAL bool isWebConnected = false;
LOCAL bool isServerConnected = false;
LOCAL TaskHandle_t taskHandler = NULL;
LOCAL TimerHandle_t ledTimer = NULL;

LOCAL bool checkLan() {
  bool isConnected = (wifi_station_get_connect_status() == STATION_GOT_IP);
  return isConnected;
}

LOCAL bool checkWeb() {
  int32_t ping_time;

  ip_addr_t web_ip;
  IP4_ADDR(&web_ip, 8, 8, 8, 8);

  ping_time = ping(&web_ip);

  for (uint8_t i = 0; i < 5; i++) {
    if (ping_time >= 0) {
      return true;
    }
  }

  return false;
}

LOCAL bool checkServer() {
  ///TODO: Check server status implementation.

  ip_addr_t ip;

  if (netconn_gethostbyname("www.google.com", &ip) != ERR_OK) {
    return false;
  }

  struct netconn *sock = NULL;

  if ((sock = netconn_new(NETCONN_TCP)) == NULL) {
    return false;
  }

  bool success = false;

  if (netconn_connect(sock, &ip, 80) == ERR_OK) {
    netconn_disconnect(sock);
    success = true;
  }

  netconn_close(sock);
  netconn_delete(sock);
  return success;
}

LOCAL void ledTimerCallBack(TimerHandle_t xTimer) {
  GPIO_SetPinToggle(PIN_NETWORK_LED);
}

LOCAL void ledInit() {
  GPIO_Init(PIN_NETWORK_LED);
  GPIO_SetPinType(PIN_NETWORK_LED, GPIO_TYPE_OUTPUT);
  GPIO_SetPinLow(PIN_NETWORK_LED);

  if (ledTimer != NULL) {
    printf("[WARNING] Network led timer is not NULL!\n");
    return;
  }

  ledTimer = xTimerCreate("Network Led", pdMS_TO_TICKS(1000), pdTRUE, NULL, ledTimerCallBack);

  if (ledTimer == NULL) {
    printf("[ERROR] Network led timer not created!");
    return;
  }

  ledStatus = LED_NOT_CONNECTED;
}

LOCAL void ledNotConnected() {
  if (ledStatus != LED_NOT_CONNECTED) {
    xTimerStop(ledTimer, 500);
    GPIO_SetPinHigh(PIN_NETWORK_LED);
    ledStatus = LED_NOT_CONNECTED;
  }
}

LOCAL void ledLanConnected() {
  if (ledStatus != LED_LAN_CONNECTED) {
    xTimerChangePeriod(ledTimer, pdMS_TO_TICKS(500), 500);
    xTimerStart(ledTimer, 500);
    ledStatus = LED_LAN_CONNECTED;
  }
}

LOCAL void ledWebConnected() {
  if (ledStatus != LED_WEB_CONNECTED) {
    xTimerChangePeriod(ledTimer, pdMS_TO_TICKS(200), 500);
    xTimerStart(ledTimer, 500);
    ledStatus = LED_WEB_CONNECTED;
  }
}

LOCAL void ledServerConnected() {
  if (ledStatus != LED_SERVER_CONNECTED) {
    xTimerStop(ledTimer, 500);
    GPIO_SetPinLow(PIN_NETWORK_LED);
    ledStatus = LED_SERVER_CONNECTED;
  }
}

bool getIsLanConnected() {
  return isLanConnected;
}

bool getIsWebConnected() {
  return isWebConnected;
}

bool getIsServerConnected() {
  return isServerConnected;
}

void netStatusUpdate() {
  if (taskHandler != NULL)
    xTaskNotifyGive(taskHandler);
}

void netStatusUpdate_ISR() {
  if (taskHandler != NULL)
    vTaskNotifyGiveFromISR(taskHandler, NULL);
}

void taskNetStatus(void *args) {

  taskHandler = xTaskGetCurrentTaskHandle();

  ledInit();

  while (true) {
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5000));

    isLanConnected = checkLan();

    if (!isLanConnected) {
      isWebConnected = false;
      isServerConnected = false;
      ledNotConnected();
      continue;
    }

    isWebConnected = checkWeb();

    if (!isWebConnected) {
      isServerConnected = false;
      ledLanConnected();
      continue;
    }

    isServerConnected = checkServer();

    if (!isServerConnected) {
      ledWebConnected();
    } else {
      ledServerConnected();
    }
  }
}

