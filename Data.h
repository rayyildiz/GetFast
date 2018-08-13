#ifndef DATA_H
#define DATA_H

#include "Defs.h"
#include <pthread.h>
#include <netinet/in.h>

struct url_info{
	char url[URL_SIZE];
	char host[HOST_SIZE];
	char identifer[URL_SIZE];
	char file[FILE_SIZE];
	char ip[IP_SIZE];
	
	int port;
};


struct thread_data{
	struct sockaddr_in sin;
	pthread_t thread;
	int id;	/* 1, 2, 3, ...*/
	char request[REQUEST_SIZE];

	char file[FILE_SIZE];
	char identifer[URL_SIZE];
	char host[HOST_SIZE];
	char dir[FILE_SIZE];
	
	char status;
	char type;
};

#endif
