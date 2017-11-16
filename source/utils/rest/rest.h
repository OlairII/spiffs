/*
 * rest.h
 *
 *  Created on: 19 de set de 2017
 *      Author: dkrepsky
 */

#ifndef SOURCE_UTILS_REST_REST_H_
#define SOURCE_UTILS_REST_REST_H_

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	REST_OK,        //
	REST_MEM,       //
	REST_PARAM,      //
	REST_UNKNOWN,   //
} RestErr;

typedef enum {
	GET,      //
	POST,     //
	PUT,      //
	PATCH,    //
	DELETE,   //
	COPY,     //
	HEAD,     //
	OPTIONS,  //
	LINK,     //
	UNLINK,   //
	PURGE,    //
	LOCK,     //
	UNLOCK,   //
	PROPFIND, //
	VIEW,     //
} RestType;

typedef enum {
	AUTH_NONE,    //
	AUTH_BASIC,	  //
	AUTH_BEARER,  //
	AUTH_DIGEST,  //
	AUTH_HOBA,    //
	AUTH_MUTUAL,  //
	AUTH_AWS4,    //
} AuthType;

typedef enum {
	CONTENT_JSON, //
	CONTENT_XML,  //
	CONTENT_RAW,  //
} ContType;

typedef enum {
	NO_CACHE,	    //
	NO_STORE,	    //
	NO_TRANSFORM,	//
	ONLY_IF_CACHED,	//
} CacheControl;

typedef struct {
	char *user;
	char *passwd;
} AuthBasic;

typedef struct {
	char *host;
	uint16_t port;
	uint16_t lport;
	char *url;
	char *param;
	char *body;
	AuthType authType;
	AuthBasic *auth;
	RestType restType;
	ContType contType;
	CacheControl cacheCtrl;
	uint16_t contLength;
} Request;

typedef struct {
	uint16_t status;
	char *body;
} Response;

RestErr rest(Request *req, Response *resp);

#endif /* SOURCE_UTILS_REST_REST_H_ */
