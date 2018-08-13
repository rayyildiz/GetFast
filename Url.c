#include "Url.h"
#include "Download.h"
#include <time.h>

struct link_data{
	char host[HOST_SIZE];
	char identifer[HOST_SIZE];
	char file[FILE_SIZE];
};

struct link_data contents[MAX_LINK_URL];
int length=0;


BOOL download_url(struct url_info* url){
	int sd,rc,f,i,noof=0,finish_file=0;
	struct hostent* h;
	time_t start_time,finish_time;
	struct thread_data mthread;//Main thread
	struct thread_data* wthreads;//Worker
	char tmpDir[FILE_SIZE],tmp[FILE_SIZE];
	
	#ifdef DEBUG
	printf("\t[Url.c    ] Start of download_url \n");
	#endif
	
	if (!url){
		#ifdef DEBUG
		fprintf(stderr,"The url is not defined yet \n");
		#endif
		return FALSE;
	}

	if ((h=gethostbyname(url->host))==NULL){
		#ifdef DEBUG
		printf("The hostname is invalid\n");
		#endif
		return FALSE;
	}
	
	#ifdef DEBUG
	printf("\t[Url.c    ] Got hostname infos\n");
	#endif
	
	mthread.sin.sin_family = h->h_addrtype;
	memcpy((char *) &mthread.sin.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
	mthread.sin.sin_port = htons(80);
	
	sprintf(mthread.request,GETREQ2,url->identifer,url->host);

	#ifdef DEBUG
	printf("\t[Url.c    ] Creating main thread\n");
	#endif
	mthread.id=0;
	strcpy(mthread.file,url->file);
	strcpy(mthread.dir,"\0");
	strcpy(mthread.identifer,url->identifer);
	mthread.status=STATUS_STARTED;
	mthread.type=TYPE_LINKS;
	time(&start_time);
	printf("\tTrying to downloa main page [%s]\n",mthread.file);
	
	#ifdef DEBUG
	printf("\t[Url.c    ] pthread_create\n");
	#endif
	
	pthread_create(&(mthread.thread),NULL,downloader,(void*)&mthread);
	
	#ifdef DEBUG
	printf("\t[Url.c    ] pthread_join\n");
	#endif
	
	pthread_join(mthread.thread,NULL);
	time(&finish_time);
	printf("\tMain page is downloaded in %d sec[s]\n",(finish_time-start_time));
	
	
	if (mthread.status!=STATUS_SUCCESS){
		printf("\tConnection failed while downloading main page\n");
		return FALSE;
	}
	
	strncpy(tmpDir,url->file,3);
	strcat(tmpDir,"_files");
	
	strcpy(tmp,"mkdir ");
	strcat(tmp,tmpDir);
	system(tmp);//Create a new directory	
	
	#ifdef DEBUG
	printf("\t[Url.c    ] url->file: %s\n",url->file);
	#endif
	noof=parse_file(url);
	
	if (noof<0){
		printf("\tThere are some problem while parsing links\n");
		return FALSE;
	}else if(i==0){
		printf("\tThere are no link");
		return TRUE;
	}
	
	wthreads=(struct thread_data*)malloc(i*sizeof(struct thread_data));
	if (!wthreads){
		#ifdef DEBUG
		printf("\tThe memory is full\n");
		#endif
		return FALSE;
	}
	
	#ifdef DEBUG
		printf("\t >>>>> The links : total: %d \n",length);
		for(i=0;i<length;i++){
			printf("\t%d >> Host :%s -- File :%s\n",i,contents[i].host,contents[i].file);
		}
		sleep(3);
	#endif
	
	i=0;
	time(&start_time);//Get start time
	while(i<length){
		progress_bar((float)i+1.0,(float)noof);
		wthreads[i].id=i+1;
		memcpy(&(wthreads[i].sin),&(mthread.sin),sizeof(mthread.sin));
		sprintf((wthreads[i].request),GETREQ2,contents[length].identifer,contents[length].host);
		strcpy(wthreads[i].dir,tmpDir);
		wthreads[i].status=STATUS_STARTED;
		wthreads[i].type=TYPE_LINKS;
		strcpy(wthreads[i].file,contents[length].file);
		pthread_create(&(wthreads[i].thread),NULL,downloader,(void*)&(wthreads[i]));
		i++;
	}
	time(&finish_time);//Get finish time
	
	for(i=0;i<noof;i++){
		pthread_join(wthreads[i].thread,NULL);
		//if (wthreads[i].status==STATUS_SUCCESS) finish_file++;
	}
	printf("\tAll download finished in %d sec[s] \n",(finish_time-start_time));
	
	return TRUE;
}


int parse_file(struct url_info* url){
	FILE *in;
	BOOL isOK;
	int ch,state=0,i=0;
	char buf[BUF_SIZE];
	int no=0;
	
	#ifdef DEBUG
	printf("\t[Url.c    ] parse_file start File:%s\n",url->file);
	#endif
	
	if (!url) return -1;
	in=fopen(url->file,"rb+");
	if (!in){
		#ifdef DEBUG
		fprintf(stderr,"\t[Url.c    ] * File is not exist\n");
		#endif
		return -1;
	}
	
	while((ch=fgetc(in))!= EOF){
		char * retCh=0;
		isOK=FALSE;
		switch(ch){
			case '<':
				i=0;
				state=1;
				break;
			case '>':
				isOK=parse_string(retCh,buf);
				state=0;
				i=0;
				break;
			default:
				if (state==1) {
					buf[i++]=(char)ch;
					buf[i]='\0';
				}
				break;
		};
		
		if (isOK){
			if (length >= MAX_LINK_URL) return no;
			no++;

			if (strncmp(retCh,"http://",7)==0) {
				char*s;
				s=strtok(retCh+7,"/");
				strcpy(contents[length].host,s);
				if (s==NULL) strcpy(contents[length].identifer,"/");
				else strcpy(contents[length].identifer,retCh+7+strlen(s));
				
				reverse_string(retCh);
				i=0;
				for (s=retCh;*s!='/';s++){
					if (*s=='\0') break;
					contents[length].file[i++]=*s;
					contents[length].file[i]='\0';
				}
				reverse_string(contents[length].file);
				reverse_string(retCh);
				
				#ifdef DEBUG
				printf("host:%s and identifer:%s",contents[length].host,contents[length].identifer);
				#endif
			}else{
				char*s;
				strcpy(contents[length].host,url->host);
				strcpy(contents[length].identifer,retCh);
				
				reverse_string(retCh);
				i=0;
				for (s=retCh;*s!='/';s++){
					if (*s=='\0') break;
					contents[length].file[i++]=*s;
					contents[length].file[i]='\0';
				}
				reverse_string(contents[length].file);
				reverse_string(retCh);
				
				#ifdef DEBUG
				printf("host:%s and identifer:%s",contents[length].host,contents[length].identifer);
				#endif
			}
			length++;
		}
	}
	fclose(in);
	
	return no;
}/*end of the parse_file */
