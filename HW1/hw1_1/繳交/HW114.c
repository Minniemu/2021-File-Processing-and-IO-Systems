#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
int gettimeofday (struct timeval *tv, struct timezone *tz );
//srand((unsigned int)time(NULL));
int main(void){
	FILE *fp;
	char *buffer;
	int FILE_SIZE = 100*1024*1024;
	int piece = 4*1024;
	buffer = malloc(4*1024);
	struct timeval start;
	struct timeval end;
	unsigned long diff;
	srand(time(NULL));


	fp = fopen("file.txt","r");
	gettimeofday(&start,NULL);

	for(int i = 0;i < 50000 ; i++){
		int offset = (rand()%FILE_SIZE)+1;
		offset = offset - (offset%piece);
		fseek(fp,offset,SEEK_SET);
		fread(buffer,sizeof(char), 4*1024 / sizeof(char), fp);
	}
	gettimeofday(&end,NULL);

	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("Random read took %ld (us)\n", diff);

    free(buffer);
   	fclose(fp);

   	return 0;
}