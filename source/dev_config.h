/*
 * config.h
 *
 *  Created on: 2 de ago de 2017
 *      Author: dkrepsky
 */

#ifndef SOURCE_CONFIG_DEV_CONFIG_H_
#define SOURCE_CONFIG_DEV_CONFIG_H_

#include "esp_common.h"

#define PRODUCT_NAME "KoppenClima"
#define FIRMWARE_VERSION "0.1"
#define HARDWARE_VERSION "0.1"

#define SERVER_ADDR "koppenclima.tk"
#define SERVER_PORT 8080

#define DEVICE_ID "59bf6eb6b7e3f5148c0b7545"

#define PIN_DHT           GPIO_PIN_4
#define PIN_WATER_DROP    GPIO_PIN_5
#define PIN_WIFI_CONFIG   GPIO_PIN_13
#define PIN_NETWORK_LED   GPIO_PIN_2

#endif /* SOURCE_CONFIG_DEV_CONFIG_H_ */
