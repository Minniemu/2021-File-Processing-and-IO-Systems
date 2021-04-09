#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
int gettimeofday (struct timeval *tv, struct timezone *tz );

int main(void){
	char *str;
   	FILE *fp;
   	int FILE_SIZE = 100*1024*1024;
	int piece = 4*1024;
	str = malloc(2*1024);
	struct timeval start;
	struct timeval end;
	unsigned long diff;

	str = malloc(FILE_SIZE);
   	for(int i = 0; i<2*1024 ;i++){
   		str[i] = '0';
   	}
	fp = fopen("file.txt","w+");
	
	gettimeofday(&start,NULL);

	for(int i = 0;i < 50000 ; i++){
		int offset = (rand()%FILE_SIZE)+1;
		offset = offset - (offset%piece);
		fseek(fp,offset,SEEK_SET);
		fwrite(str,sizeof(char),sizeof(str), fp);
	}
	
	gettimeofday(&end,NULL);

	diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
    printf("Random buffered write_1 took %ld (us)\n", diff);

    free(str);
   	fclose(fp);

   	return 0;
}