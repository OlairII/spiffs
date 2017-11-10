/*
 * measure.c
 *
 *  Created on: 27 de ago de 2017
 *      Author: dkrepsky
 */

#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cJSON.h"
#include "lwip/apps/sntp.h"
#include "lwip/apps/sntp_time.h"
#include "lwip/api.h"
#include "lwip/netbuf.h"

#include "dev_config.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/dhtxx.h"
#include "tasks/net_status.h"
#include "tasks/restcomm.h"

#include "measure.h"

LOCAL cJSON *root = NULL;

LOCAL uint32_t drop_count = 0;

LOCAL const char *header =
    "POST /dados HTTP/1.1\r\nHost: koppenclima.tk:8080\r\nContent-Type: application/json\r\nContent-Length: %d\r\nAuthorization: Basic a29wcGVuOjlmZGZkMDYwNjQzOTk3NTYzNTNhMDljNTllOTM1YmQ4NDA5MGJhZDQ=\r\nCache-Control: no-cache\r\n\r\n";
void incDropCounter() {
// TODO: Make this thread safe.
  drop_count++;
}

char *lastMeasureJSON() {
// TODO: Make this thread safe.
  return cJSON_Print(root);
}

void taskMeasure(void *args) {

// Initialize Water Drop pin.
  GPIO_Init(PIN_WATER_DROP);
  GPIO_SetPinType(PIN_WATER_DROP, GPIO_TYPE_INPUT);
  GPIO_PullUpDisable(PIN_WATER_DROP);
  GPIO_IntSetType(PIN_WATER_DROP, GPIO_INT_FALLING_EDGE);

// Create JSON object.
  root = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "estacao", cJSON_CreateString(DEVICE_ID));

  cJSON *measurements = cJSON_CreateObject();
  cJSON_AddItemToObject(root, "medidas", measurements);

  cJSON *temperature = cJSON_CreateNumber(0);
  cJSON_AddItemToObject(measurements, "temperatura", temperature);

  cJSON *humidity = cJSON_CreateNumber(0);
  cJSON_AddItemToObject(measurements, "humidade", humidity);

  cJSON *water_vol = cJSON_CreateNumber(0);
  cJSON_AddItemToObject(measurements, "volumeDeAgua", water_vol);

//  cJSON *time_stamp = cJSON_CreateNumber(0);
//  cJSON_AddItemToObject(measurements, "time_stamp", time_stamp);

  DHT_Init(PIN_DHT);
  DHT_Data dht22;

  while (true) {

// Sample temperature and humidity.
    DHT_Error e = DHT_Read(PIN_DHT, &dht22);

    if (e == DHT_ERR_OK) {
      cJSON_SetNumberValue(temperature, ((double ) dht22.temperature) / 10.0);
      cJSON_SetNumberValue(humidity, ((double ) dht22.humidity) / 10.0);
    } else {
      cJSON_SetNumberValue(temperature, 0xFFFF);
      cJSON_SetNumberValue(humidity, 0xFFFF);
    }

// Sample water volume.
    uint32_t drops = drop_count;
    drop_count = 0;

    double wvol = 0.008374042611977 * ((double) drops) - 0.013764823756088;

    if (wvol < 0) {
      wvol = 0.0;
    }

    cJSON_SetNumberValue(water_vol, wvol);

// Sample time.
    uint32_t current_stamp;
    current_stamp = sntp_get_current_timestamp();
//    cJSON_SetNumberValue(time_stamp, current_stamp);
    printf("[INFO] New sample at %s", sntp_get_real_time(current_stamp));

    char * meas = lastMeasureJSON();

    if (getIsServerConnected()) {
      ip_addr_t ip;

      if (netconn_gethostbyname(SERVER_ADDR, &ip) == ERR_OK) {
        struct netconn *sock = NULL;

        if ((sock = netconn_new(NETCONN_TCP)) != NULL) {
          netconn_set_recvtimeout(sock, 5000);

          netconn_bind(sock, IP_ADDR_ANY, 8080);

          if (netconn_connect(sock, &ip, SERVER_PORT) == ERR_OK) {

            int headerLen = strlen(header);
            int bodyLen = strlen(meas);

            int len = headerLen + bodyLen + 10;

            char *buffer = malloc(len);

            sprintf(buffer, header, bodyLen);
            strcat(buffer, meas);

            err_t e;

            e = netconn_write(sock, (void * ) buffer, strlen(buffer), NETCONN_COPY);

            if (e != ERR_OK) {
              printf("[ERROR] Failed to send request: %d\n", e);
            }

            free(buffer);

            struct netbuf *rb = NULL;

            e = netconn_recv(sock, &rb);

            if (e != ERR_OK) {
              printf("[ERROR] Failed to get response: %d\n", e);
            }

            netbuf_delete(rb);
          }

          netconn_close(sock);
          netconn_delete(sock);
        }
      }
    }

    cJSON_free(meas);

    // Print debug info.
    printf("[INFO] Free Heap: %d.\n", system_get_free_heap_size());

    vTaskDelay(pdMS_TO_TICKS(SAMPLE_PERIOD_SEC*1000));
  }
}
