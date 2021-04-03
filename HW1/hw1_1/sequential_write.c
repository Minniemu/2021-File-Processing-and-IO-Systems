#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
int gettimeofday (struct timeval *tv, struct timezone *tz );

int main(void){
	char *str;
   	FILE *fp;
   	int FILE_SIZE = 100*1024*1024;
	char *buffer;
	buffer = malloc(4*1024);
	struct timeval start;
	struct timeval end;
	unsigned long diff;

	str = malloc(FILE_SIZE);
   	for(int i = 0; i<FILE_SIZE/sizeof(char) ;i++){
   		str[i] = '1';
   	}

	fp = fopen("file.txt","w+");
	fseek(fp,SEEK_SET,0);
	gettimeofday(&start,NULL);
	for(int i = 0;i < FILE_SIZE/(2*1024) ; i++){
		fwrite(str,sizeof(char), 2*1024 / sizeof(char), fp);
	}
	gettimeofday(&end,NULL);

	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("the difference is %ld (us)\n", diff);

   	//printf("%s\n", buffer);
   	fclose(fp);

   	return 0;
}