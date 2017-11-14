/*
 * restcomm.c
 *
 *  Created on: Nov 10, 2017
 *      Author: houz03
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
#include "tasks/net_status.h"
#include "utils/rest/rest.h"
#include "restcomm.h"

void taskRestComm(void *args) {

	printf("\n\nentrou na taskRestComm\n\n");
	Request req;
	req.host = "api.apixu.com";
	req.port = 80;
	req.url =
			"http://api.apixu.com/v1/current.json?key=46712e1bd82748309f9183131172710&q=Londrina";
	req.body = NULL;
	req.cacheCtrl = NO_CACHE;
	req.contLength = 0;
	req.contType = CONTENT_JSON;
	req.restType = GET;
	req.authType = AUTH_NONE;


	Response resp;
	printf("criou as structs\n");
	printf("\n\n%s\n\n", req.host);

	while (1) {

		printf("entrando na func rest\n");
		rest(&req, &resp);
		printf("saiu da rest\n");
		cJSON *response = cJSON_Parse(resp.body);
		char *wfdata = cJSON_Print(response);
		printf("%s", wfdata);
		vTaskDelay(pdMS_TO_TICKS(60000));

	}

}
