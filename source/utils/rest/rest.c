/*
 * rest.c
 *
 *  Created on: 19 de set de 2017
 *      Author: dkrepsky
 */

#include "rest.h"
#include "lwip/api.h"
#include "lwip/netbuf.h"
#include "tasks/net_status.h"
#include "utils/base64/base64.h"
#include "math.h"

char *stringFromRestType(RestType n) {

	char *str[] = { "GET", "POST", "PUT", "PATCH", "DELETE", "COPY", "HEAD",
			"OPTIONS", "LINK", "UNLINK", "PURGE", "LOCK", "UNLOCK", "PROPFIND",
			"VIEW" };

	return str[n];

}

char *stringFromContType(ContType n) {

	char *str[] = { "application/json", "application/xml", "text/plain" };

	return str[n];

}

char *stringFromCacheControl(CacheControl n) {

	char *str[] = { "no-cache", "no-store", "no-transform", "only-if-cached" };

	return str[n];

}

char *stringFromAuthType(AuthType n) {

	char *str[] = { "Basic", "Bearer", "Digest", "HOBA", "Mutual",
			"AWS4-HMAC-SHA256" };

	return str[n];

}

RestErr rest(Request *req, Response *resp) {

	//
	RestErr error = REST_UNKNOWN;


	if (getIsWebConnected()) {

		printf("conectado\n");
		ip_addr_t ip;

		//return the server IP
		if (netconn_gethostbyname(req->host, &ip) == ERR_OK) {
			printf("pegou o nome do host\n");
			struct netconn *sock = NULL;

			//create a new netconn connection TCPIPv4
			if ((sock = netconn_new(NETCONN_TCP)) != NULL) {
				printf("sock criado\n");
				//set the receiver timeout
				netconn_set_recvtimeout(sock, 5000);

				//Bind a netconn to a specific local IP address and port
				netconn_bind(sock, IP_ADDR_ANY, req->lport);
				printf("sock bindado\n");

				err_t e = netconn_connect(sock, &ip, req->port);

				//Connect a netconn to a specific remote IP address and port.
				if (e == ERR_OK) {
					int urllen = strlen(req->url);
					char *path = req->url + 8;
					while ((*path != '/') && (urllen > 0)) {
						path++;
						urllen--;
					}
					if (urllen == 0) {
						printf("Invalid URL\n");
						error = REST_PARAM;
					} else {
						int headerLen = strlen(
								stringFromRestType(req->restType)) + 1
								+ +strlen(path) + 1 + strlen("HTTP/1.1") + 4
								+ strlen("HOST: ") + strlen(req->host) + 4
								+ strlen("Content-Type: ")
								+ strlen(stringFromContType(req->contType)) + 4
								+ strlen("Cache-Control: ")
								+ strlen(stringFromCacheControl(req->cacheCtrl))
								+ 4 + strlen("Content-Length: ")
								+ sizeof(req->contLength) + 8;

						int bodyLen = 0;
						if (req->body != NULL) {
							bodyLen = strlen(req->body);
						}
						char *header = malloc(headerLen + bodyLen);

						if (req->authType == AUTH_NONE) {
							sprintf(header,
									"%s %s HTTP/1.1\r\nHost: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\nCache-Control: %s\r\n\r\n",
									stringFromRestType(req->restType), path,
									req->host,
									stringFromContType(req->contType),
									req->contLength,
									stringFromCacheControl(req->cacheCtrl));
						} else {

							size_t input_len = strlen(
									((AuthBasic *) req->auth)->user)
									+ strlen(":")
									+ strlen(((AuthBasic *) req->auth)->passwd);
							char *strb64 = malloc(ceil(input_len / 3) * 4);
							char *buffb64 = malloc(input_len);
							sprintf(buffb64, "%s:%s",
									((AuthBasic *) req->auth)->user,
									((AuthBasic *) req->auth)->passwd);
							strb64 = base64_encode(buffb64, input_len, NULL);

							sprintf(header,
									"%s %s HTTP/1.1\r\nHost: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\nAuthorization: %s %s\r\nCache-Control: %s\r\n\r\n",
									stringFromRestType(req->restType), path,
									req->host,
									stringFromContType(req->contType),
									req->contLength,
									stringFromAuthType(req->authType), buffb64,
									stringFromCacheControl(req->cacheCtrl));
							free(buffb64);
							free(strb64);

						}

						if (req->body != NULL) {
							strcat(header, req->body);
						}

						e = netconn_write(sock, (void * ) header,
								strlen(header), NETCONN_COPY);
						if (e != ERR_OK) {

							printf("[ERROR] Failed to send request: %d\n", e);
							error = REST_UNKNOWN;

						} else {

							free(header);
							struct netbuf *rb = NULL;

							//Receive data (in form of a netbuf containing a packet buffer) from a netconn
							e = netconn_recv(sock, &rb);
							if (e != ERR_OK) {

								printf("[ERROR] Failed to get response: %d\n",
										e);
								error = REST_UNKNOWN;

							} else {

								char *data;
								u16_t len;

								e = netbuf_data(rb, (void **) &data, &len);
								if (e != ERR_OK) {

									printf(
											"[ERROR] Failed to retrieved the information: %d\n",
											e);
									error = REST_UNKNOWN;

								} else {

									char *endptr;
									resp->status = strtol(&data[9], &endptr,
											10);

									switch (resp->status) {

									case (200):

										printf("OK");
										resp->body = strstr(data, "\r\n\r\n")
												+ 4;
										error = REST_OK;

										break;
									case (403):

										printf("403\nForbidden\n");
										break;
									case (404):

										printf("404\nNot Found");
										break;
									default:

										error = REST_UNKNOWN;
										printf("unkown error:%d\n",
												resp->status);
									}

								}
							}

						}
					}

					///////////////
					//Close a TCP netconn (doesn't delete it).
					netconn_close(sock);
					//Close a netconn 'connection' and free its resources
					netconn_delete(sock);
				}
			}
		}
	}
	return error;
}

