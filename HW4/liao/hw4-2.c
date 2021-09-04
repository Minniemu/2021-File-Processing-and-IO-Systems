#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<linux/fs.h>
#include<errno.h>
#include<sys/time.h>
#include<time.h>
#include<string.h>

#define FILESIZE (512 * 1000 * 1000)	// 512 MB
#define DEFAULTDISK "/dev/sda4"
#define TEMPFILE "temp.txt"
#define STARS "***********************************************************************"

int blockSize;

void randomSeek(int fd){
	lseek(fd, (random() % (FILESIZE / blockSize)) * blockSize, SEEK_SET);
}

unsigned long int elapsed(struct timeval* start, struct timeval* end){
	return 1000000 * (end->tv_sec - start->tv_sec) + (end->tv_usec - start->tv_usec);
}

int main(int argc, char** args){
	// STEP0: set random seed
	srand(time(NULL));

	// STEP1: set disk path and print some information for user
	char* disk = DEFAULTDISK;

	if(argc > 1){
		disk = args[1];
	}
	printf("Disk path: %s\n", disk);
	if(argc == 1){
		printf("  - You can change this by passing path at first arguments.\n    ex: `%s /dev/sdb1`\n", args[0]);
		printf("  - If you do not know what is your disk path, check by `df -h`\n");
	}

	char cwd[1024];
	getcwd(cwd, 1024);
	printf("\n"STARS"\n Ensure that the path `%s` is in the disk `%s`\n"STARS"\n\n", cwd, disk);

	// STEP2: get read ahead size
	// g for open disk path
	int g = open(disk, O_RDWR);
	if(g == -1) goto openDiskErr;
	if(ioctl(g, BLKSSZGET, &blockSize)) goto ioctlErr;
	printf("%s - block size: %d\n", disk, blockSize);


	// STEP3: create a 500MB file
	// f for TEMPFILE
	int f = open(TEMPFILE, O_WRONLY | O_CREAT | O_TRUNC);
	if(f == -1) goto tempFileErr;

	int i, j;
	char* writer = malloc(FILESIZE / 1000);
	for(i=0; i < 1000; i++){
		write(f, writer, FILESIZE / 1000);
	}
	free(writer);
	fsync(f);
	close(f);

	// STEP4: set read ahead size iteratively
	// - clear cache by system();
	// - open disk
	//   - set read ahead
	// - close
	// - open temp file
	//   - start timer1
	//     - sequential read
	//   - end timer1
	//   - start timer2
	//     - random read
	//   - end timer2
	// - close
	// - record the result

	int sizeList[] = {0,16, 64};
	struct timeval start, end;
	unsigned long int diff1, diff2;
	char output[4096] = "";
	for(i=0; i<3; i++){
		// clean cache
		system("sudo sh -c \"echo 3 > /proc/sys/vm/drop_caches\"");

		// set read ahead size
		g = open(disk, O_RDWR);
		if(g == -1) goto openDiskErr;
		if(ioctl(g, BLKRASET, sizeList+i)) goto ioctlErr;
		close(g);

		f = open(TEMPFILE, O_RDONLY | O_NONBLOCK | O_SYNC);
		if(f == -1) goto tempFileErr;

		char* reader = malloc(blockSize);
		// sequential read
		gettimeofday(&start, NULL);
		for(j=0; j < FILESIZE / blockSize; j++){
			read(f, reader, blockSize);
		}
		gettimeofday(&end, NULL);
		diff1 = elapsed(&start, &end);

		// random read
		gettimeofday(&start, NULL);
		for(j=0; j < FILESIZE / blockSize; j++){
			randomSeek(f);
			read(f, reader, blockSize);
		}
		gettimeofday(&end, NULL);
		diff2 = elapsed(&start, &end);

		close(f);
		free(reader);

		char* res = malloc(sizeof(char) * 1024);
		sprintf(res, "set read ahead - %2d\n  - sequential read: %10.3f ms\n  - randomly read:   %10.3f ms\n", sizeList[i], diff1/1000.0, diff2/1000.0);
		printf("%s", res);
		strcat(output, res);
		free(res);
	}

	// create a file for recording result
	FILE* o = fopen("output.txt", "w+");
	if(!o){
		fprintf(stderr, "open(output.txt) error: %d\n", errno);
		exit(EXIT_FAILURE);
	}
	fprintf(o, "%s", output);
	fclose(o);
	exit(EXIT_SUCCESS);

ioctlErr:
	fprintf(stderr, "ioctl error: %d\n\ndf -h\n\n", errno);
	system("df -h");
	exit(EXIT_FAILURE);

openDiskErr:
	if(errno == 13){
		fprintf(stderr, "open(%s) permission dinied.\n", disk);
	}else{
		fprintf(stderr, "open(%s) error: %d\n\ndf -h\n\n", disk, errno);
		system("df -h");
	}
	exit(EXIT_FAILURE);

tempFileErr:
	fprintf(stderr, "open("TEMPFILE") error: %d", errno);
	exit(EXIT_FAILURE);
}
