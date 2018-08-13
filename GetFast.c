#include "GetFast.h"
#include "File.h"

int main(int argc,char* argv[]){
	struct url_info url;
	int i,tr=0;
	char host[HOST_SIZE];
	int noofthreads=DEFAULT_THREAD_NO;
	int dfile=1,dlink=0,define_thread=0;
	
	if (argc < 2){
		printf("Unkown command. Please type %s -h[--help] to get help \n",argv[0]);
		exit(FAIL);
	}
	
	for(i=1;i<argc;i++){
		if ((strcmp(argv[i],"-d")==0) || (strcmp(argv[i],"-D")==0)){
			dfile=1;
			tr++;
		}
		
		if ((strcmp(argv[i],"-u")==0) || (strcmp(argv[i],"-U")==0)){
			dfile=0;
			dlink=1;
			tr++;
		}
		
		if ((strcmp(argv[i],"-v")==0) || (strcmp(argv[i],"-V")==0)){
			printf("GetFast version %.2f\n",VERSION);
			tr++;
			exit(SUCCESS);
		}
		
		if ((strcmp(argv[i],"--help")==0) || (strcmp(argv[i],"-h")==0)){
			usage();
			exit(SUCCESS);
		}
		
		if ((strcmp(argv[i],"-n")==0) || (strcmp(argv[i],"-N")==0)){
			define_thread=1;
			tr++;
			tr++;
			if (i<(argc-1)) noofthreads=atoi(argv[i+1]);
		}
	}
	
	if (tr>=argc){
		printf("Array is shorter than you want\n");
		exit(FAIL);
	}
	
	strcpy(host,argv[tr+1]);
	
	if (dfile && dlink){
		printf("You can not define twice \n");
	}
	
	if (noofthreads<=0){
		printf("You can not define treads number under zero\n");
		exit(FAIL);
	}
	
	if (!parse_url(&url,host)){
		printf("There are some problem while parsing usrl\n");
		exit(FAIL);
	}
	
	#ifdef DEBUG
	printf("\t[GetFast.c] download_link ?\n");
	#endif
	if (dlink==1){
		#ifdef DEBUG
		printf("\t[GetFast.c] download_link OK\n");
		#endif
		if (download_url(&url)){
			#ifdef DEBUG
			printf("\t[GetFast.c] download_link is success\n");
			#endif
			printf("\tDownloading URL is success\n");
		}else{
			#ifdef DEBUG
			printf("\t[GetFast.c] download_link is not success\n");
			#endif
			printf("\tDownloading URL is failed\n");
		}
		return SUCCESS;
	}
	
	#ifdef DEBUG
	printf("\t[GetFast.c] download_file ?\n");
	#endif
	
	if (dfile==1){
		#ifdef DEBUG
		printf("\t[GetFast.c] download_file OK\n");
		#endif
		if (download_file(&url,noofthreads)==TRUE){
			#ifdef DEBUG
			printf("\t[GetFast.c] download_file is success\n");
			#endif
			printf("\tDownloading file is success\n");
		}else{
			#ifdef DEBUG
			printf("\t[GetFast.c] download_file is not success\n");
			#endif
			printf("\tDownloadýng file is failed\n");
		}
		return SUCCESS;
	}

	return SUCCESS;
}
