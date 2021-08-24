#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/fs.h>
#include <time.h> 
int gettimeofday (struct timeval *tv, struct timezone *tz );
#define BLKRASET _IO(0x12,98)
#define BLKRAGET _IO(0x12,99)

int main(int argc, char **argv)
{
	unsigned long readahead_arr[3] = {0,16,64};
	unsigned long readahead = 0,get_readahead = 0;
	int fd = open("/dev/sda4",O_RDWR);
	char *str;
	int fp;
	char *buffer;
	int FILE_SIZE = 512*1000*1000;
	buffer = malloc(2*1000);
	str = malloc(FILE_SIZE);
   	for(int i = 0; i<FILE_SIZE/sizeof(char) ;i++){
        str[i] = '0';
   	}
   
    fp = open("file.txt",O_CREAT|O_RDWR);
    write(fp,str,FILE_SIZE/sizeof(char)); 
	for (int i = 0;i < 3;i++){
		//Set readahead size
		ioctl(fd, BLKRASET, readahead_arr[i]);
		ioctl(fd, BLKRAGET, &get_readahead);
		printf("readahead : %lu\n",get_readahead );
		//Read Sequentially
		int fp;
		char *buffer;
		int FILE_SIZE = 512*1000*1000;
		buffer = malloc(2*1000);
		struct timeval start;
		struct timeval end;
		unsigned long diff;
		fp = open("file.txt",O_CREAT|O_RDWR);
		lseek(fp,0,SEEK_SET);
		gettimeofday(&start,NULL);
		for(int i = 0;i < FILE_SIZE / 2000 ; i++){
			read(fp,buffer, (2*1000) / sizeof(char));
		}
		gettimeofday(&end,NULL);

		diff = 1000000 * (end.tv_sec-start.tv_sec)+ end.tv_usec-start.tv_usec;
	    printf("Sequential Read took %ld (us)\n", diff);
	   	//Random Read
	   	struct timeval start2;
		struct timeval end2;
		unsigned long diff2;
	   	int piece = 2*1000;
	   	gettimeofday(&start2,NULL);
		for(int i = 0;i < FILE_SIZE / 2000 ; i++){
			int offset = (rand()%FILE_SIZE)+1;
			offset = offset - (offset%piece);
			lseek(fp,offset,SEEK_SET);
			read(fp,buffer,2000);
		}
		gettimeofday(&end2,NULL);

		diff2 = 1000000 * (end2.tv_sec-start2.tv_sec)+ end2.tv_usec-start2.tv_usec;
	    printf("Random read took %ld (us)\n", diff2);
	    printf("\n");
	}
	

}
