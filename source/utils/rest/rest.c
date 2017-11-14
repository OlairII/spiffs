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
#include "base64/base64.h"


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

void rest(Request *req, Response *resp) {

	//
	RestErr error = 0;

	printf("tentando conectar\n");
	if (getIsWebConnected()) {

		printf("Conectado\n");
		ip_addr_t ip;

		//return the server IP
		if (netconn_gethostbyname(req->host, &ip) == ERR_OK) {
			printf("pegou o ip\n");
			struct netconn *sock = NULL;

			//create a new netconn connection TCPIPv4
			if ((sock = netconn_new(NETCONN_TCP)) != NULL) {
				//set the receiver timeout
				netconn_set_recvtimeout(sock, 5000);

				//Bind a netconn to a specific local IP address and port
				netconn_bind(sock, IP_ADDR_ANY, 8080);

				//Connect a netconn to a specific remote IP address and port.
				if (netconn_connect(sock, &ip, req->port) == ERR_OK) {

					printf("sock criado\n");
					err_t e;

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

						printf("%s\n", path);

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

						printf("\n\n\nheaderLen:%d\n\n\n", headerLen);

						int bodyLen = 0;
						if (req->body != NULL) {
							bodyLen = strlen(req->body);
							printf("bodyLen = %d\n", bodyLen);
						}
						char *header = malloc(headerLen + bodyLen);
//						char header[200];
						printf("malloc\n");
						printf("%d\n", req->contLength);

						// restType  path  host  contType  cachectrl

						if (req->authType == AUTH_NONE) {
							sprintf(header,
									"%s %s HTTP/1.1\r\nHost: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\nCache-Control: %s\r\n\r\n",
									stringFromRestType(req->restType), path,
									req->host,
									stringFromContType(req->contType),
									req->contLength,
									stringFromCacheControl(req->cacheCtrl));
						} else {

							char *base64buffer;
							size_t input_len = strlen(req->auth.AuthBasic.user) +strlen(req->auth.AuthBasic.passwd);
							size_t *output_len;

							base64_encode(){

							}

							sprintf(header,
									"%s %s HTTP/1.1\r\nHost: %s\r\nContent-Type: %s\r\nContent-Length: %d\r\nAuthorization: %s %s\r\nCache-Control: %s\r\n\r\n",
									stringFromRestType(req->restType), path,
									req->host,
									stringFromContType(req->contType),
									req->contLength, stringFromAuthType(req->authType), req->auth,
									stringFromCacheControl(req->cacheCtrl));
						}
						printf("header formatado\n");
						printf("%s\n", header);

//						if (req->contlength != 0) {
//							sprintf(header,
//									("Content-Length: %hu", req->contLength));
//						printf("header formatado com contLength\n");
//						}

						if (req->body != NULL) {
							strcat(header, req->body);
							printf("header formatado com body\n");
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

								//verificar o tamanho ???
								e = netbuf_data(rb, (void **) &data, &len);
								if (e != ERR_OK) {

									printf(
											"[ERROR] Failed to retrieved the information: %d\n",
											e);
									error = REST_UNKNOWN;

								} else {

//								char *stcode = malloc(4 * sizeof(char));
//								strncpy(stcode, data + 9, 3);
//								resp->status = atoi(stcode);
//								free(stcode);

									char *endptr;
									resp->status = strtol(&data[9], &endptr,
											10);

									//analisa o status code
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
										printf("unkown error\n");
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
//	return error;
}
//mod18:16
