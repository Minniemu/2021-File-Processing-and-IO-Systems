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
   	int FILE_SIZE = 100;
	int piece = 4;
	str = malloc(4);
	struct timeval start;
	struct timeval end;
	unsigned long diff;

	str = malloc(2);
   	for(int i = 0; i<2 ;i++){
   		str[i] = '0';
   	}
	fp = open("file.txt",O_CREAT|O_RDWR);	
	gettimeofday(&start,NULL);

	for(int i = 0;i < 500 ; i++){
		int offset = (rand()%FILE_SIZE)+1;
		offset = offset - (offset%piece);
		lseek(fp,offset,SEEK_SET);
		write(fp, str,sizeof(str));
	}
	
	gettimeofday(&end,NULL);

	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("Random buffered write_1 took %ld (us)\n", diff);

    free(str);
   	close(fp);

   	return 0;
}