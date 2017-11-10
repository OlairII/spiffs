/*
 * getwforecast.c
 *
 *  Created on: Oct 25, 2017
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

#include "getwforecast.h"

#define WFSERVER_ADDR	"api.apixu.com"

//LOCAL cJSON *root = NULL;

//LOCAL cJSON *wdata = NULL;
LOCAL const char *header =
		"GET /v1/current.json?key=46712e1bd82748309f9183131172710&q=Londrina HTTP/1.1\r\nHost: api.apixu.com\r\nContent-Type: application/json\r\nCache-Control: no-cache\r\n\r\n";

void taskGetwforecast(void * args) {

	while (1) {

		printf("esperando conexao\n");
		if (getIsWebConnected()) {

			printf("conectado\n");
			ip_addr_t ip;

			//return the server IP
			if (netconn_gethostbyname(WFSERVER_ADDR, &ip) == ERR_OK) {
				struct netconn *sock = NULL;

				//create a new netconn connection TCPIPv4
				if ((sock = netconn_new(NETCONN_TCP)) != NULL) {
					//set the receiver timeout
					netconn_set_recvtimeout(sock, 5000);

					//Bind a netconn to a specific local IP address and port
					netconn_bind(sock, IP_ADDR_ANY, 8080);

					printf("tentando conectar\n");
					//Connect a netconn to a specific remote IP address and port.
					if (netconn_connect(sock, &ip, 80) == ERR_OK) {

						printf("conectou\n");
						err_t e;

						printf("enviando header\n");
						//Send data over a TCP netconn.
						e = netconn_write(sock, (void * ) header,
								strlen(header), NETCONN_COPY);

//						printf("header enviado\n");
						if (e != ERR_OK) {
							printf("[ERROR] Failed to send request: %d\n", e);
						}

						printf("sem erro\n");
						struct netbuf *rb = NULL;

						printf("recebendo dado\n");
						//Receive data (in form of a netbuf containing a packet buffer) from a netconn
						e = netconn_recv(sock, &rb);

						printf("dado recebido\n");
						//passar o dado recebido para uma var JSON
						//JSON_parse  ???
						//wdata = cJSON_Parse(rb);

						if (e != ERR_OK) {
							printf("[ERROR] Failed to get response: %d\n", e);
						} else {
							printf("recebeu sem erros\n");

							char *data;
							u16_t len;

							netbuf_data(rb, (void **) &data, &len);

							while ((*data != '{') && (len > 0)) {
								data++;
								len--;
							}

							if (len == 0) {
								printf("no json received\n");

							} else {
								cJSON *response = cJSON_Parse(data);
								char *wfdata = cJSON_Print(response);
								printf("%s", wfdata);
							}
						}

						//desaloca o rb
						netbuf_delete(rb);
					}

					printf("fechando conexao\n");
					//Close a TCP netconn (doesn't delete it).
					netconn_close(sock);
					//Close a netconn 'connection' and free its resources
					netconn_delete(sock);
				}
			}

			vTaskDelay(pdMS_TO_TICKS(600000));

		}

		vTaskDelay(pdMS_TO_TICKS(300));
	}

}
