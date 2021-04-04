#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int gettimeofday (struct timeval *tv, struct timezone *tz );

int main(void){
	FILE *fp;
	char *buffer;
	int FILE_SIZE = 100*1024*1024;
	buffer = malloc(4*1024);
	struct timeval start;
	struct timeval end;
	unsigned long diff;

	fp = open("file.txt",O_CREAT|O_RDWR);
	lseek(fp,0,SEEK_SET);
	gettimeofday(&start,NULL);
	for(int i = 0;i < FILE_SIZE/(4*1024); i++){
		read(fp,buffer, (4*1024) / sizeof(char));
		//printf("%s\n",buffer );
	}
	gettimeofday(&end,NULL);

	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("Sequential Read took %ld (us)\n", diff);

   	//printf("%s\n", buffer);
   	close(fp);

   	return 0;
}