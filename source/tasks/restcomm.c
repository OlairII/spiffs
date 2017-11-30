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
#include "utils/base64/base64.h"
#include "math.h"

void taskRestComm(void *args) {

	printf("\n\nentrou na taskRestComm\n\n");
	Request req;
	req.host = "api.apixu.com";
	req.port = 8080;
	req.lport = 8000;
	req.url =
			"http://api.apixu.com/v1/current.json?key=46712e1bd82748309f9183131172710&q=Londrina";
	req.body = NULL;
	req.cacheCtrl = NO_CACHE;
	req.contLength = 0;
	req.contType = CONTENT_JSON;
	req.restType = GET;
	req.authType = AUTH_NONE;

	/*
	AuthBasic authbasic;
	authbasic.user = "olair";
	authbasic.passwd = "JR";

	req.auth = &authbasic;*/

	Response resp;

	printf("criou as structs\n");
	printf("\n\n%s\n\n", req.host);

	/*
	size_t input_len = strlen(((AuthBasic *) req.auth)->user) + strlen(":")
			+ strlen(((AuthBasic *) req.auth)->passwd);
	char *strb64 = malloc(ceil(input_len / 3) * 4);
	char *buffb64 = malloc(input_len);
	sprintf(buffb64, "%s:%s", ((AuthBasic *) req.auth)->user,
			((AuthBasic *) req.auth)->passwd);
	strb64 = base64_encode(buffb64, input_len, NULL);
	printf("\n\n%s\n\n",strb64);*/



	/*
	Request req;
	req.host = "httpbin.org";
	req.port = 80;
	req.lport = 8080;
	req.url = "http://httpbin.org/post";
	req.body = "POST test";
	req.cacheCtrl = NO_CACHE;
	req.contLength = strlen(req.body);
	req.contType = CONTENT_RAW;
	req.restType = POST;
	req.authType = AUTH_NONE;*/



	while (1) {

		printf("entrando na func rest\n");
		if (rest(&req, &resp) == REST_OK) {
			printf("saiu da rest\n");
			printf("%s\n", resp.body);
			cJSON *response = cJSON_Parse(resp.body);
			char *wfdata = cJSON_Print(response);
			printf("%s", wfdata);
		}
		vTaskDelay(pdMS_TO_TICKS(60000));
	}
}
