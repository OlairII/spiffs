### Introduction

Almost a RESTful api

With this api you can create an HTTP client in ESP8266 MCU.

This api uses the LwIP V1.4.1. to create, configure and delete a network socket. The first thing that rest function do is verify if the mcu have a wifi connection( using the getIsWebConnected function). After that it uses the LwIP functions to create and configure a sock. Then the rest function has the task to format an http header, send it to the specified host and recive the host response. You have to pass two parameters, Request and Response. Both parameters are struct.

The Request struct contains the following variables.

	-char *host;
	-uint16_t lport;
	-uint16_t port;
	-char *url;
        the url must have this format: http://exampleurl.com/path
	-char *body;
        if you don't want to send a empty body just declare it NULL.
	-AuthType authType;
        this library just suport the basic authentication method.
	-void *auth;
        this void pointer is used to point to a struct that have username and password. 
	-RestType restType;
        An enumarete lists the http request type
	-ContType contType;
        An enumerate lists the http content type
	-CacheControl cacheCtrl;
        the cache control
	-uint16_t contLength;
        if you don't know the content lentgh declare it equals zero.

The Response struct contains two variables.
    
    -uint16_t status;
        the http status code
    -char *body;
	the body already formated without the header.



