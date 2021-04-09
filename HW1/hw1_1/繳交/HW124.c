#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
int gettimeofday (struct timeval *tv, struct timezone *tz );
//srand((unsigned int)time(NULL));
int main(void){
	FILE *fp;
	char *buffer;
	int FILE_SIZE = 100;
	int piece = 2;
	buffer = malloc(2);
	struct timeval start;
	struct timeval end;
	unsigned long diff;
	srand(time(NULL));


	fp = open("file.txt",O_CREAT|O_RDWR);
	gettimeofday(&start,NULL);

	for(int i = 0;i < 500 ; i++){
		int offset = (rand()%FILE_SIZE)+1;
		offset = offset - (offset%piece);
		lseek(fp,offset,SEEK_SET);
		read(fp,buffer,2);
	}
	gettimeofday(&end,NULL);

	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("Random read took %ld (us)\n", diff);

    free(buffer);
   	close(fp);

   	return 0;
}