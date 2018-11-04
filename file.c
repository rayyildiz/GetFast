#include "file.h"
#include "download.h"

#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

BOOL download_file(struct url_info* url,int noofthreads){
	struct thread_data* wthreads;
	int rc,sd,i;
	struct hostent *h;
	struct sockaddr_in servAddr;
	long rate;
	/*FILE* fd;*/
	int fd;
	long start_file,finish_file,total_file;
	time_t start_time,finish_time;
	char tmp[BUF_SIZE];
	char buffer[BUFFER_SIZE];

	printf("\tThe downloading file startted\n");
	#ifdef DEBUG
	printf("\t[File.c   ] download_file basi\n");
	#endif
	if(!url){
		#ifdef DEBUG
		printf("\t[File.c   ] The url is not defined yet \n");
		#endif
		return FALSE;
	}
	
	if (noofthreads<1) return FALSE;
	#ifdef DEBUG
	printf("\t[File.c   ] No of threads:%d\n",noofthreads);
	#endif
	wthreads=(struct thread_data*)malloc(noofthreads*sizeof(struct thread_data));
	if (!wthreads){
		#ifdef DEBUG
		printf("\t[File.c   ] The memory is full \n");
		#endif
		return FALSE;
	}
	
	#ifdef DEBUG
	printf("\t[File.c   ] Wthreads was created\n");
	#endif
	
	if ((h=gethostbyname(url->host))==NULL){
		#ifdef DEBUG
		printf("\t[File.c   ] The hostname is invalid\n");
		printf(">> The hostname : \n",url->host);
		#endif
		return FALSE;
	}

	#ifdef DEBUG
	printf("\t[File.c   ] gethostbyname\n");
	#endif
	servAddr.sin_family = h->h_addrtype;
	memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	servAddr.sin_port = htons(80);
	
	total_file=head_get_file_length(url);
	if (total_file<0){
		#ifdef DEBUG
		printf("\t[File.c   ]Some problem while getting file length\n");
		#endif
		return FALSE;
	}
	
	#ifdef DEBUG
	printf("\t[File.c   ] start threads File-Length:%ld \n\n",total_file);
	#endif
	
	time(&start_time);
	rate=total_file/noofthreads;
	finish_file=0L;

	for(i=0;i<noofthreads;i++){
		start_file=finish_file+1L;
		if (i==(noofthreads-1)) finish_file=total_file;
		else finish_file=(i+1)*rate;
		
		
		#ifdef DEBUG
		printf("\t[File.c   ] url->identifer : %s --- url->host : %s\n",url->identifer,url->host);
		#endif
		
		sprintf(wthreads[i].request,GETREQ1,url->identifer,url->host,start_file,finish_file);
		memcpy(&(wthreads[i].sin),&servAddr,sizeof(wthreads[i].sin));
		wthreads[i].id= i+1;
		strcpy(wthreads[i].dir,"\0");
		strcpy(wthreads[i].host,url->host);
		strcpy(wthreads[i].identifer,url->identifer);
		wthreads[i].type=TYPE_FILES;
		wthreads[i].status=STATUS_STARTED;
		strncpy(tmp,url->file,3);
		sprintf(wthreads[i].file,"%s_%d",tmp,wthreads[i].id);
		
		#ifdef DEBUG
		printf("\t[File.c   ] Getting file info >>> \n");
		printf(">> The request : %s\n",wthreads[i].request);
		printf(">> The ID      : %d\n",wthreads[i].id);
		printf(">> The host    : %s\n",wthreads[i].host);
		printf(">> The dir     : %s\n",wthreads[i].dir);
		printf(">> The file    : %s\n",wthreads[i].file);
		printf(">> The Rate    : %d\n",rate);
		printf(">> The start_fi: %ld\n",start_file);
		printf(">> The finish_f: %ld\n\n",finish_file);
		sleep(1);
		#endif
		
		progress_bar((float)finish_file,(float)total_file);
		
		pthread_create(&(wthreads[i].thread),NULL,downloader,(void*)&wthreads[i]);
	}
	
	for(i=0;i<noofthreads;i++){
		pthread_join(wthreads[i].thread,NULL);
	} 
	
	time(&finish_time);
	printf("\tAll downloaded finished in %ld secs \n",((int)finish_time-start_time));
	printf("\tFile is orginazing \n");
	if (strlen(url->file)<1) strcpy(url->file,"down.tmp");
	
	time(&start_time);
	rc=0;
	
	/*fd = fopen(url->file,"w+b");*/
	fd = open(url->file,O_WRONLY | O_CREAT | O_TRUNC, 0x777);
	
	for(i=0;i<noofthreads;i++){
	
		sd=open(wthreads[i].file,O_RDONLY);
		
		if (sd < 0) break;
		do{
		
			memset(buffer,0x0,BUFFER_SIZE);
			rc=read(sd,buffer,BUFFER_SIZE);
			#ifdef DEBUGX
			printf("The buffer : %s\n",buffer);
			#endif
			if (rc>0){
				write(fd,buffer,rc);
			}
			
		}while(rc>0);
		
		close(sd);
		remove(wthreads[i].file);
	}
	close(fd);
	
	time(&finish_time);
	printf("\tFile orginazing finished in %ld sec\n",(finish_time-start_time));
	#ifdef DEBUG
	printf("\t[File.c   ] file connection\n");
	#endif
	
	return TRUE;
}
