#ifndef DEFS_H
#define DEFS_H

#define VERSION 0.3

#define DEFAULT_THREAD_NO 5

#define DEBUG_X

#define BUFFER_SIZE     1024
#define BUF_SIZE        100
#define URL_SIZE 	    32
#define HOST_SIZE 	    64
#define FILE_SIZE 	    32
#define IP_SIZE		    20
#define REQUEST_SIZE    256

#define MAX_LINK_URL    20

#define BOOL 	int
#define TRUE 	1
#define FALSE 	0

#define SUCCESS 0
#define FAIL 	1

#define TYPE_FILES 1
#define TYPE_LINKS 2

#define STATUS_STARTED		0
#define STATUS_DOWNLOADING 	1
#define STATUS_SUCCESS		2
#define STATUS_FAILED		3

#define NORMAL_MODE	0
#define DEBUG_MODE	1

#define HEADREQ  "HEAD %s HTTP/1.1\r\nHost: %s\r\n\r\n"
#define GETREQ1  "GET %s HTTP/1.1\r\nHOST: %s\r\nRange: bytes=%ld-%ld\r\n\r\n"
#define GETREQ2  "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n"

#endif
