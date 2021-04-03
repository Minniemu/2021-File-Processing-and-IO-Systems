#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
int gettimeofday (struct timeval *tv, struct timezone *tz );

int main(void){
	FILE *fp;
	char *buffer;
	int FILE_SIZE = 100*1024*1024;
	buffer = malloc(4*1024);
	struct timeval start;
	struct timeval end;
	unsigned long diff;

	fp = fopen("file.txt","r");
	fseek(fp,SEEK_SET,0);
	gettimeofday(&start,NULL);
	for(int i = 0;i < FILE_SIZE/(4*1024) ; i++){
		fread(buffer,sizeof(char), 4*1024 / sizeof(char), fp);
	}
	gettimeofday(&end,NULL);

	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("the difference is %ld (us)\n", diff);

   	//printf("%s\n", buffer);
   	fclose(fp);

   	return 0;
}