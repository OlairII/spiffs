/*
 * test_task.c
 *
 *  Created on: 2 de ago de 2017
 *      Author: dkrepsky
 */

#include "esp_common.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lwip/apps/sntp.h"
#include "lwip/apps/sntp_time.h"

#include "dev_config.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "tasks/net_status.h"
#include "tasks/measure.h"

#include "tasks/startup.h"
#include "tasks/local_udp.h"
//#include "tasks/getwforecast.h"
//#include "tasks/restcomm.h"
#include "tasks/spiffs_test.h"



extern bool g_smartconfigFinished;

extern void ISR_Gpio(void *arg);
extern void CALLBACK_Wifi(System_Event_t *pEvent);
extern void CALLBACK_SmartConfig(sc_status status, void *pdata);


void enableWifi() {

	// Initialize the SmartConfig pin to check for wifi configuration mode.
	wifi_set_event_handler_cb(CALLBACK_Wifi);

	wifi_set_opmode(STATION_MODE);

	char *SSID = "houz";
	char *PW = "houz.com.br";

	struct station_config cfg;
	strcpy(cfg.ssid, SSID);
	strcpy(cfg.password, PW);

	cfg.bssid_set = 0;

	wifi_station_set_config(&cfg);

	wifi_station_connect();

//  // Check for WiFi configuration mode.
//  bool isConfigPressed = GPIO_GetPin(PIN_WIFI_CONFIG);
//  bool isConfigReset = (system_get_rst_info()->reason == REASON_EXT_SYS_RST) ? true : false;
//
//  if (isConfigPressed && isConfigReset) {
//    printf("[INFO] Entering WiFi setup mode...\n");
//    wifi_set_opmode(STATION_MODE);
//
////    smartconfig_start(CALLBACK_SmartConfig);
//
//    printf("[INFO] Smartconfig started, waiting for SSID and PSWD.\n");
//
//    while (!g_smartconfigFinished) {
//      vTaskDelay(pdMS_TO_TICKS(100));
//    }
//
//    printf("[INFO] SmartConfig success!\n");
//    smartconfig_stop();
//  } else {
//    wifi_set_opmode_current(STATION_MODE);
//    wifi_station_connect();
//  }
}

void waitForConnection() {
	xTaskCreate(taskNetStatus, "NetStatus", 500, NULL, 3, NULL);

	while (getIsWebConnected() == false) {
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void startSntp() {
	sntp_setservername(0, "0.br.pool.ntp.org"); // set server 0 by domain name
	sntp_setservername(1, "1.br.pool.ntp.org"); // set server 1 by domain name
	sntp_setservername(2, "us.pool.ntp.org"); // set server 0 by domain name
	sntp_set_timezone(-3);
	sntp_init();

	while (sntp_get_current_timestamp() == 0) {
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void createTasks() {
//	xTaskCreate(taskRestComm, "Rest Communication", 500, NULL, 3, NULL);
	xTaskCreate(taskSpiffsTest, "SPIFFS test", 500, NULL, 3, NULL);
//	xTaskCreate(taskGetwforecast, "Weather Forecast", 500, NULL, 3, NULL);
//  xTaskCreate(taskMeasure, "Measurements", 500, NULL, 3, NULL);
//  xTaskCreate(taskLocalUdp, "Local UDP", 500, NULL, 3, NULL);
}

void taskStartup(void *pData) {

	printf("habilitando wifi\n");
	enableWifi();

	printf("esperando conexao\n");
	waitForConnection();

//	startSntp();

	printf("criando task\n");
	createTasks();

//	uint32 current_stamp;
//	current_stamp = sntp_get_current_timestamp();
//
//	printf("[INFO] Successful initialization at %s\n",
//			sntp_get_real_time(current_stamp));

	vTaskDelete(NULL);
}
