#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include<unistd.h>
int main()
{
   char *str;
   int fp;
   int FILE_SIZE = 512*1024*1024;

   /* Initial memory allocation */
   str = malloc(FILE_SIZE);
   for(int i = 0; i<FILE_SIZE/sizeof(char) ;i++){
         str[i] = '0';
   }
   
   fp = open("file.txt",O_CREAT|O_RDWR);
   write(fp,str,FILE_SIZE/sizeof(char)); 
   /* Deallocate allocated memory */
   close(fp);
   free(str);
   
   return(0);
}