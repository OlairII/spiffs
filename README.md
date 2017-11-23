### Introduction

Almost a RESTful api

With this api you can create an HTTP client in ESP8266 MCU.

This api uses the LwIP V1.4.1. to create, configure and delete a network socket. The first thing that rest function do is verify if the mcu have a wifi connection( using the getIsWebConnected function). After that it uses the LwIP functions to create and configure a sock. Then the rest function has the task to format an http header, send it to the specified host and recive the host response. In the end, the sock is closed and deleted. 

You have to pass two parameters, Request and Response. Both parameters are struct.

The Request struct contains the following variables.

    -char *host;
    	Name of the host.
    -uint16_t lport;
    	Local port, it's up to you to manage this.
    -uint16_t port;
     	The port that the host server are using.
    -char *url;
	    The url must have this format: http://exampleurl.com/path
    -char *body;
	    If you want to send an empty body just declare it NULL.
    -AuthType authType;
	    This library just suport the basic authentication method(AUTH_BASIC). For no authentication use AUTH_NONE.
    -void *auth;
	    This void pointer is used to point to a struct that has username and password( in case you use basic auth). 
    -RestType restType;
	    An enumarete that list the http request type
    -ContType contType;
	    An enumerate that list the http content type
    -CacheControl cacheCtrl;
	    The cache control
    -uint16_t contLength;
	    Content Length. If you don't know the content lentgh declare it equals zero.

The Response struct contains two variables.
    
    -uint16_t status;
        The http status code.
    -char *body;
	    The body already formated without the header.



