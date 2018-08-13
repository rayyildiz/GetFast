#include "Misc.h"

char PROGVERSION[]="\tGetFast v%.2f by Ramazan AYYILDIZ and Gazi KESKIN\n";


void reverse_string(char* str){
	char *p, *s;
	int i;
	int size;

	if ((size = strlen(str)) == 0)
		return;
	p = (char *)calloc(size, sizeof(char));
	s = p;
	for (i = size; i >= 0; i--, s++)
		*s = *(str + i - 1);
	*s = '\0';
	memset(str, 0, size);
	strncpy(str, p, size);
}


void usage(){
	printf("\n\t-----------------------------------------------------------\n");
	printf(PROGVERSION,VERSION);
	printf("\n\tUsage : \n\t-------\n\n");
	printf("\t  GetFast -[d/u] [-n PART_NUMBER] [-v] [-h/--help] url \n\n");
	printf("\t  -d     : Downlaod a file \n");
	printf("\t  -u     : Downlaod remote url and first order link to disk\n");
	printf("\t  -n     : Split the remote file into parts PART_NUMBER while\n"); 
	printf("\t           downloading the file. Default=5 parts \n");
	printf("\t  -v     : Display version of GetFast\n");
	printf("\t  -h     : Get help\n");
	printf("\t  --help : Get help\n");
	printf("\t  url    : Remote url. Url must be HTTP protocol\n");
	printf("\t-----------------------------------------------------------\n\n");
}/*end of the usage */


BOOL parse_string(char* out,char* atr){
	char*s='\0';
	char *tok;
	int a_link=0,img_link=0;
	
	#ifdef DEBUG
	printf("\t[Misc.c   ] Parse String start \n");
	#endif
	
	if (strlen(atr)==0){
		out=NULL;
		return FALSE;
	}
	
	if (atr[0]=='/') {
		out=NULL;
		return FALSE;
	}
	
	if (atr[0]=='a' || atr[0]=='A')
		a_link=1;
	else if (strncmp(atr,"img",3)==0)
		img_link=1;
	else{
		out=NULL;
		return FALSE;
	}
	
	s=atr;
	while(TRUE){
		tok=strtok(s," ");
		if (tok==NULL) break;
		if (a_link && (strncmp(tok,"href=\"",6))==0){
			char* t=strtok(tok+6,"\"");
			strcpy(out,t);
			return TRUE;
		}else if (img_link && (strncmp(tok,"src=\"",5))==0){
			char* t=strtok(tok+5,"\"");
			strcpy(out,t);
			return TRUE;
		}
		s=s+strlen(tok)+1;
	}
	#ifdef DEBUG
	printf("\t[Misc.c   ] Parse String finish \n");
	#endif
	return FALSE;
}


void progress_bar(float cur,float tot){
	static float last= 0.0;
	float rate = (cur/tot)*100.0;
	if (rate==0) rate=1.0;
	int i=0,dot=0;
	
	dot=(int)(rate/2.0);
	if (rate < last + 5.0 || rate > 100) return;
	for (i=0;i<dot;i++){
		printf(".");
	}
	for (i=0;i<(52-dot);i++){
		printf(" ");
	} 
	printf("%.3f %\n",(float)rate);
	
	last=rate;
}


BOOL parse_url(struct url_info* u,char* h){
	char *s;
	char buf[FILE_SIZE];
	int i;
	
	#ifdef DEBUG
	printf("\t[Misc.c   ] Parse URL start \n");
	#endif
	
	if (!u){
		u=(struct url_info*)malloc(sizeof(struct url_info));
		if (!u){
			#ifdef DEBUG
			fprintf(stderr,"\t[Misc.c   ]Memory is full\n");
			#endif
			return FALSE;
		}
	}/*if url has no area at memory */
	
	if (strncmp(h,"http://",7)!=0){
		fprintf(stderr,"The url must start with >> http://... \n");
		usage();
		u=NULL;
		exit((int)FAIL);
	}
	 
	strcpy(u->url,h);/*Copy all arg[1] to url */
	#ifdef DEBUG
	printf("\t[Misc.c   ] strcpy(u->url,h)  Now url : %s \n",u->url);
	#endif
	reverse_string(u->url);
	
	s=u->url;
	i=0;
	for(s=u->url;*s!='/';s++){
		buf[i++]=*s;
		buf[i]='\0';
	}
	
	reverse_string(buf);
	strcpy(u->file,buf);
	reverse_string(u->url);
	
	s=u->url + 7;
	i=0;
	for(;*s!='\0';s++){
		if (*s=='/') break;
		buf[i++]=*s;
		buf[i]='\0';
	}
	
	strcpy(u->host,buf);
	
	#ifdef DEBUG
	printf("\t[Misc.c   ] Hostname: %s\n",buf);
	#endif
	
	u->port=80;
	
	#ifdef DEBUG
	printf("\t[Misc.c   ] After file Now url->port:%d url->file:%s \n",u->port,u->file);
	#endif
	
	i=0;
	s=h+7+strlen(u->host);

	for( ;*s!='\0';s++){
		if(*s=='\0') break;
		buf[i++]=*s;
		buf[i]='\0';
	}
	strcpy(u->identifer,buf);

	#ifdef DEBUG
	printf("\t[Misc.c   ] Identifer: %s \n",u->identifer);
	#endif	
	
	#ifdef DEBUG
	printf("\t[Misc.c   ] Parse URL finish\n");
	#endif
	return TRUE;
}/*end of the parse_url */
