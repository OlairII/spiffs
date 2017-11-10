/*
 * dhtxx.h
 *
 *  Created on: 6 de ago de 2017
 *      Author: dkrepsky
 */

#ifndef SOURCE_DRIVER_DHTXX_H_
#define SOURCE_DRIVER_DHTXX_H_

typedef enum {
  DHT_ERR_OK, /// No error.
  DHT_ERR_CHECKSUM, /// Data Checksum failed.
  DHT_ERR_NO_CON, /// No answer from the sensor.
  DHT_ERR_TIMEOUT, /// Sensor stoped responding.
  DHT_ERR_UNKNOWN, /// Something weird happened.
} DHT_Error;

typedef struct {
  int16_t humidity;
  int16_t temperature;
} DHT_Data;

void DHT_Init(GPIO_Pin pin);

DHT_Error DHT_Read(GPIO_Pin pin, DHT_Data *measurement);

#endif /* SOURCE_DRIVER_DHTXX_H_ */
