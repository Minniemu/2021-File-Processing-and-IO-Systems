#include<dirent.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

long total_size = 0;
long open_read(char *dir){
    char tmp[1024];
    DIR* pdir = opendir(dir);
    if(pdir == NULL){
        return 0;
    }
    struct dirent* pEntry = NULL;
    long local_size = 0;
    int file_num = 0;
    while(((pEntry = readdir(pdir)) != NULL)){
        file_num ++;
        struct stat* buffer = malloc(sizeof(struct stat)) ;
        sprintf(tmp, "%s" ,pEntry->d_name);
        //sprintf(tmp, "%s/%s", dir, pEntry->d_name);
        lstat(tmp, buffer);
        printf("%s \t",tmp);
        if(file_num % 3 == 0){
            printf("\n");
        }
        free(buffer);
    }
    printf("\n");
    return local_size;
}

void list(char *dir){
    char tmp[1024];
    DIR* pdir = opendir(dir);
    if(pdir == NULL){
        return ;
    }
    struct dirent* pEntry = NULL;
    while(((pEntry = readdir(pdir)) != NULL)){
        struct stat* buffer = malloc(sizeof(struct stat)) ;
        sprintf(tmp, "%s" ,pEntry->d_name);
        lstat(tmp, buffer);
        //file format
        if(pEntry->d_type == DT_DIR )
            printf("d");
        else if(pEntry->d_type == DT_LNK )
            printf("l");
        else
            printf("-");
        //Permission-owners
        if(buffer->st_mode & S_IRUSR)
            printf("r");
        else
            printf("-");
        if(buffer->st_mode & S_IWUSR)
            printf("w");
        else
            printf("-");
        if(buffer->st_mode & S_IXUSR)
            printf("x");
        else
            printf("-");
        //Permission-group
        if(buffer->st_mode & S_IRGRP)
            printf("r");
        else
            printf("-");
        if(buffer->st_mode & S_IWGRP)
            printf("w");
        else
            printf("-");
        if(buffer->st_mode & S_IXGRP)
            printf("x");
        else
            printf("-");
        //Permission-others
        if(buffer->st_mode & S_IROTH)
            printf("r");
        else
            printf("-");
        if(buffer->st_mode & S_IWOTH)
            printf("w");
        else
            printf("-");
        if(buffer->st_mode & S_IXOTH)
            printf("x");
        else
            printf("-");
        printf(" ");

        //link
        char *p = calloc(1000, sizeof(char));
        //char *buf = malloc(123);
        readlink(tmp, p, sizeof(p));
        printf("%s ",p);
        free(p);
        
        //owner
        struct passwd *user = getpwuid(buffer->st_uid);
        printf("%s ",user->pw_name);
        
        //group
        struct group *grp = getgrgid(buffer->st_gid);
        printf("%s ",grp->gr_name);

        //size
        printf("%ld ",buffer->st_size);

        //time
        time_t rawtime = buffer->st_mtime;
        struct tm *ptr_time = localtime(&rawtime);
        char str[50];
        strftime(str, sizeof(str), "%x %H:%M", ptr_time );
 	    printf("%s ", str );

        printf("%s \t",tmp);
        printf("\n");
        free(buffer);
    }
}
int main(int argc, char** args){
    long size = 0;
    char* dir;
    if(argc < 2){
        dir = ".";
        size = open_read(dir);
    }
    else if(argc == 2){
        dir = args[1];
        size = open_read(dir);
    }
    else if (argc == 3){
        dir = args[1];
        list(dir);
        //printf("%s\n",args[1]);
    }
    return 0;
}