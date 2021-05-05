#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
long total_size = 0;
long open_read(char *dir){
    char tmp[1024];
    DIR* pdir = opendir(dir);
    if(pdir == NULL){
        return 0;
    }
    struct dirent* pEntry = NULL;
    long local_size = 0;
    while(((pEntry = readdir(pdir)) != NULL)){
        struct stat* buffer = malloc(sizeof(struct stat)) ;
        sprintf(tmp, "%s/%s", dir, pEntry->d_name);
        if (strcmp(pEntry->d_name, "..") == 0 || strcmp(pEntry->d_name, ".") == 0)
            continue;
        if(strstr(tmp, "/..") == NULL){
            if(pEntry->d_type == DT_DIR){
                lstat(tmp, buffer);
                local_size += buffer->st_size;
                long ls = open_read(tmp);
                local_size += ls;
                printf("%ld\t",buffer->st_size+ls);
                printf("(DIR) \t");
                printf("%s\n",tmp);
            }else if(pEntry->d_type == DT_LNK){
                lstat(tmp, buffer);
                printf("%ld\t",buffer->st_size);
                printf("(Link file) \t");
                printf("%s\n", tmp);
                local_size += buffer->st_size;
            }else if(pEntry->d_type == DT_REG){
                lstat(tmp, buffer);
                printf("%ld\t",buffer->st_size);
                printf("(Regular file) \t");
                printf("%s\n", tmp);
                local_size += buffer->st_size;
            }
            else{
                lstat(tmp, buffer);
                printf("%ld\t",buffer->st_size);
                printf("(Unknown) \t");
                printf("%s\n", tmp);
                local_size += buffer->st_size;
            }
        }
        free(buffer);
    }
    return local_size;
}
int main(int argc, char** args){
    long size = 0;
    char* dir = argc < 2 ? "." : args[1];
    size = open_read(dir);

    struct stat* buffer=malloc(sizeof(struct stat));
    lstat(dir, buffer);
    size += buffer->st_size;

    printf("Total size = %ld\n", size);
    return 0;
}