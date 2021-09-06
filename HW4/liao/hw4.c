#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/fs.h>
#include<errno.h>
#include<sys/time.h>
#include<time.h>

#define FILESIZE (512 * 1000 * 1000)	// 512 MB
#define BUFFERSIZE (2 * 1000)		// 2 KB

void randomSeek(int fd){
	lseek(fd, (random() % (FILESIZE / BUFFERSIZE)) * BUFFERSIZE, SEEK_SET);
}

unsigned long int elapsed(struct timeval* start, struct timeval* end){
	return 1000000 * (end->tv_sec - start->tv_sec) + (end->tv_usec - start->tv_usec);
}

int main(){
	// set random seed
	srand(time(NULL));

	// declare a buffer
	char* buffer = malloc(BUFFERSIZE);

	// create and open a file
	int f = open("tempFile", O_WRONLY | O_TRUNC | O_CREAT, 0666);
	int d = open("/dev/sda4", O_RDWR);

	if(f == -1){
		fprintf(stderr, "open() error: %d\n", errno);
		exit(EXIT_FAILURE);
	}

	int i;
	for(i=0; i < FILESIZE / BUFFERSIZE; i++){
		write(f, buffer, BUFFERSIZE);
	}

	// set read ahead size iteratively
	int j;
	int sizeList[] = {0, 16, 64};
	FILE* out = fopen("output.txt", "w+");
	for(i=0; i<3; i++){
		// set read ahead size
		if(ioctl(d, BLKRASET, sizeList+i)){ // if success return 0
			fprintf(stderr, "ioctl error: %d\n", errno);
			exit(EXIT_FAILURE);
		}

		struct timeval start, end;
		unsigned long int diff1, diff2;

		// sequential read
		gettimeofday(&start, NULL);
		for(j=0; j < FILESIZE / BUFFERSIZE; j++){
			read(f, buffer, BUFFERSIZE);
		}
		gettimeofday(&end, NULL);
		diff1 = elapsed(&start, &end);

		gettimeofday(&start, NULL);
		// radome read
		for(j=0; j < FILESIZE / BUFFERSIZE; j++){
			randomSeek(f);
			read(f, buffer, BUFFERSIZE);
		}
		gettimeofday(&end, NULL);
		diff2 = elapsed(&start, &end);

		printf("set read ahead - %2d\n  - sequential read: %lu us\n  - randomly read:   %lu us\n\n", sizeList[i], diff1, diff2);
		fprintf(out, "set read ahead - %2d\n  - sequential read: %lu us\n  - randomly read:   %lu us\n\n", sizeList[i], diff1, diff2);

	}
	close(f);
	close(d);
	fclose(out);
}
