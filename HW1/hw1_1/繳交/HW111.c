#include <stdio.h>
#include <stdlib.h>
//#define FILE_SIZE 100000000
int main()
{
   char *str;
   FILE *fp;
   int FILE_SIZE = 100*1024*1024;

   /* Initial memory allocation */
   str = malloc(FILE_SIZE);
   for(int i = 0; i<FILE_SIZE/sizeof(char) ;i++){
   		str[i] = '0';
   }
   //strcpy(str, "yiibai");
   //printf("String = %s", str);
   fp = fopen("file.txt","w");
   fwrite(str, sizeof(char),FILE_SIZE/sizeof(char), fp); 
   /* Deallocate allocated memory */
   fclose(fp);
   free(str);
   
   return(0);
}
