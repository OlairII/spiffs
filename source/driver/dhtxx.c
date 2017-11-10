/*
 * dhtxx.c
 *
 *  Created on: 6 de ago de 2017
 *      Author: dkrepsky
 */

#include "esp_common.h"

#include "FreeRTOS.h"
#include "task.h"

#include "gpio.h"

#include "dhtxx.h"

extern void system_soft_wdt_feed();

LOCAL uint16_t *raw = NULL;
LOCAL uint8_t *buffer = NULL;

LOCAL bool isCrcValid(uint8_t *buffer) {
  uint32_t sum = 0;
  uint8_t cnt;

  for (cnt = 0; cnt < 4; cnt++) {
    sum += (uint32_t) buffer[cnt];
  }

  sum &= 0xFF;

  return (sum == buffer[4]) ? true : false;
}

void DHT_Init(GPIO_Pin pin) {
  GPIO_Init(pin);
  GPIO_SetPinType(pin, GPIO_TYPE_INPUT);
  GPIO_PullUpDisable(pin);

  if (raw == NULL) {
    raw = malloc(40 * sizeof(uint16_t));
  }

  if (buffer == NULL) {
    buffer = malloc(5 * sizeof(uint8_t));
  }

  vTaskDelay(2000 / portTICK_RATE_MS);
}

DHT_Error DHT_Read(GPIO_Pin pin, DHT_Data *measurement) {

  if ((raw == NULL) || (buffer == NULL)) {
    return DHT_ERR_UNKNOWN;
  }

  for (uint8_t i = 0; i < 44; i++) {
    raw[i] = 0;
  }

  for (uint8_t i = 0; i < 5; i++) {
    buffer[i] = 0;
  }

  vPortEnterCritical();
  system_soft_wdt_feed();

// Hold line down for 20ms.
  GPIO_SetPinType(pin, GPIO_TYPE_OUTPUT);
  GPIO_SetPinLow(pin);

  os_delay_us(1000);
  system_soft_wdt_feed();

// Release the line.
  GPIO_SetPinType(pin, GPIO_TYPE_INPUT);

  os_delay_us(40);

// Waits until dht puts the line low.
  uint32_t timeout = 0;
  while (GPIO_GetPin(pin)) {
    timeout++;
    system_soft_wdt_feed();

    if (timeout > 200) {
      vPortExitCritical();
      return DHT_ERR_NO_CON;
    }
  }

  os_delay_us(80);

  timeout = 0;
  while (GPIO_GetPin(pin)) {
    timeout++;
    system_soft_wdt_feed();

    if (timeout > 200) {
      vPortExitCritical();
      return DHT_ERR_TIMEOUT;
    }
  }

  uint32_t bits_read = 0;
  GPIO_Pin laststate = GPIO_GetPin(pin);
  timeout = 0;
  while ((bits_read < 40)) {

    timeout = 0;
    while (GPIO_GetPin(pin) == laststate) {
      os_delay_us(1);
      timeout++;
      system_soft_wdt_feed();

      if (timeout > 100) {
        vPortExitCritical();
        return DHT_ERR_TIMEOUT;
      }
    }

    laststate = GPIO_GetPin(pin);

    if (laststate == 0) {
      raw[bits_read] = timeout;
      bits_read++;
    }
  }

  vPortExitCritical();

  for (uint32_t i = 0; i < 5; i++) {
    for (uint32_t j = 0; j < 8; j++) {
      buffer[i] <<= 1;
      if (raw[8 * i + j] > 20) {
        buffer[i] |= 1;
      }
    }
  }

  if (!isCrcValid(buffer)) {
    return DHT_ERR_CHECKSUM;
  }

  measurement->humidity = buffer[0];
  measurement->humidity = measurement->humidity * 256 + buffer[1];

  measurement->temperature = buffer[2] & 0x7F;
  measurement->temperature = measurement->temperature * 256 + buffer[3];

  if (buffer[2] & 0x80) {
    measurement->temperature *= -1;
  }

  return DHT_ERR_OK;
}
