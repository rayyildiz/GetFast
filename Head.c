#include "Head.h"
#include "Defs.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>

long head_get_file_length(struct url_info* url){
	long file_length=0L;
	char buffer[BUFFER_SIZE],tmp[BUF_SIZE];
	char req[REQUEST_SIZE];
	struct sockaddr_in sin;
	struct hostent *h;
	int sd,rc,i;
	
	#ifdef DEBUG
	printf("\t[Head.c   ] Start head_get_file_length\n");
	#endif
	
	if (!url){
		#ifdef DEBUG 
		printf("\t[Head.c   ] * The url is not defined yet \n");
		#endif
		return -1L;
	}
	
	#ifdef DEBUG 
	printf("\t[Head.c   ] url->identifer : %s \n",url->identifer);
	#endif
	
	sprintf(req,HEADREQ,url->identifer,url->host);
	
	#ifdef DEBUG
	printf(">>The request:\n%s\n",req);
	printf("\t[Head.c   ] Getting host name information \n");
	#endif
	if((h=gethostbyname(url->host))==NULL){
		#ifdef DEBUG
		printf("\t[Head.c   ] * There are some while getting host information\n");
		#endif
		return -1L;
	}
	
	sin.sin_family=h->h_addrtype;
	sin.sin_port=htons(80);
	memcpy((char*)&sin.sin_addr.s_addr,h->h_addr_list[0],h->h_length);
	
	#ifdef DEBUG 
	printf("\t[Head.c   ] Socket is creating \n");
	#endif
	if ((sd=socket(AF_INET, SOCK_STREAM, 0))<0){
		#ifdef DEBUG
		printf("\t[Head.c   ] * Some problem was occured while creating socket \n");
		#endif
		return -1L;
	}
	
	#ifdef DEBUG
	printf("\t[Head.c   ] Connecting to socket \n");
	#endif
	if ((rc = connect(sd,(struct sockaddr*)&sin,sizeof(sin)))<0){
		#ifdef DEBUG1 
		printf("\t[Head.c   ] * Can not connecting to socket \n");
		#endif
		return -1L;
	}
	
	#ifdef DEBUG
	printf("\t[Head.c   ] Writing request to server \n");
	#endif
	if ((rc = write(sd,req,sizeof(req)))<0){
		#ifdef DEBUG
		printf("\t[Head.c   ] * Can not writing data to server \n");
		#endif
		return -1L;
	}
	
	do{
		memset(buffer,0,BUFFER_SIZE);
		rc=read(sd,buffer,BUFFER_SIZE);
		if (rc > 0){
			char *r,*s='\0',*t;
			int i=0;
			#ifdef DEBUG
			printf("Response [%d] : \n%s\n",strlen(buffer),buffer);
			#endif
			t=buffer;
			while (TRUE){
				s=strtok(t,"\r\n");
				if (s==NULL) break;
				i++;
				t =t+strlen(s)+2;
				if (strncmp(s,"Content-Length: ",16)==0){
					char p[10];
					strcpy(p,s+16);
					file_length=atol(p);
					break;		
				}
				
			}
		}
	}while(rc>0);
	close(sd);
	#ifdef DEBUG
	printf("\t[Head.c   ] Finish head_get_file_length\n");
	#endif
	return file_length;
}
