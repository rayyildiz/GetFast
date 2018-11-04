#include "download.h"
#include <fcntl.h>
#include <sys/types.h>


void* downloader(void* arg){
	char file_name[FILE_SIZE];
	char buffer[BUFFER_SIZE],buf[BUFFER_SIZE],*s;
	int rc,sd,i,fd,isFirst=0;
	struct thread_data* thr;
	thr=(struct thread_data*)arg;
	char *t,*header,*a;
	

	#ifdef DEBUG
	printf("\t[Download ] Start function \n");
	#endif
	
	if (thr==NULL){
		#ifdef DEBUG
		printf("\t[Download ]The thr is empty\n");
		#endif
		return NULL;
	}
	
	#ifdef DEBUG
	printf("\t[Download ] --> %d. thread was created\n",thr->id);
	#endif
	
	if (strlen(thr->dir)>0) {
		strcpy(file_name,thr->dir);
		strcat(file_name,"/");
		strcat(file_name,thr->file);
	}else{
		strcpy(file_name,thr->file);
	}
	
	
	if ((sd = socket(AF_INET, SOCK_STREAM, 0))<0){
		#ifdef DEBUG
		printf("\t[Download ]The socket was not created \n");
		#endif
		exit(FAIL);
	}
	
	if ((rc= connect(sd, (struct sockaddr*) &(thr->sin), sizeof(thr->sin)))<0){
		#ifdef DEBUG
		printf("\t[Download ]The connectioan failed\n");
		#endif
		exit(FAIL);
	}
	
	if ((rc=write(sd,thr->request,strlen(thr->request)))<0){
		#ifdef DEBUG
		printf("\t[Download ]The writing of request is failed \n");
		#endif
		exit(FAIL);
	}
	
	#ifdef DEBUG
	printf("\t[Download ] Getting file info >>> \n");
	printf(">> The request : %s\n",thr->request);
	sleep(1);
	#endif
	
	if (strlen(file_name)<1) strcpy(file_name,"down.tmp");
	rc=0;
	fd=open(file_name, O_CREAT | O_WRONLY ,0x777);
	isFirst=0;
	do{
		memset(buffer,0x0,BUFFER_SIZE);
		rc=read(sd,buffer,BUFFER_SIZE);
		
		#ifdef DEBUGX
		printf("\t[Download ] The buffer :%s \n",buffer);
		sleep(1);
		#endif
		
		if (rc>0){
			if (isFirst==0){
				a=buffer;
				header=strstr(buffer,"\r\n\r\n");
				t=header+4;
				#ifdef DEBUGY
				printf(".....---> The buffer : %s \n",buffer);
				#endif
				write(fd,t,(rc -(int)(t-a)));
			}else{
				write(fd,buffer,rc);
			}
			
			isFirst=1;
		}
	}while(rc>0);
	
	close(fd);
	close(sd);
	
	thr->status=STATUS_SUCCESS;
	
	#ifdef DEBUG
	printf("\t[Download ] -- >%d. thread was success\n",thr->id);
	#endif
	pthread_exit((void*)0);
}
