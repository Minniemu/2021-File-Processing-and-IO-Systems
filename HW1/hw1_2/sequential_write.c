#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int gettimeofday (struct timeval *tv, struct timezone *tz );

int main(void){
	char *str;
   	FILE *fp;
   	int FILE_SIZE = 100*1024*1024;
	
	struct timeval start;
	struct timeval end;
	unsigned long diff;

	str = malloc(2*1024);
   	for(int i = 0; i < 2*1024 ;i++){
   		str[i] = '1';
   	}

	fp = open("file.txt",O_CREAT|O_RDWR);
	lseek(fp,0,SEEK_SET);
	gettimeofday(&start,NULL);
	for(int i = 0;i < FILE_SIZE/2048 ; i++){
		write(fp, str, 2048);
		//fsync(fileno(fp));
	}
	gettimeofday(&end,NULL);
	//printf("%d\n",sizeof(str) );
	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("the difference is %ld (us)\n", diff);

   	//printf("%s\n", buffer);
   	close(fp);

   	return 0;
}