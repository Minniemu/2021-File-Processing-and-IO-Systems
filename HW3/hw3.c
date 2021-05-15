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

void open_read(char *dir){
    char tmp[1024];
    DIR* pdir = opendir(dir);
    if(pdir == NULL){
        return;
    }
    struct dirent* pEntry = NULL;
    int file_num = 0;
    while(((pEntry = readdir(pdir)) != NULL)){
        file_num ++;
        struct stat* buffer = malloc(sizeof(struct stat)) ;
        sprintf(tmp, "%s" ,pEntry->d_name);
        // sprintf(tmp, "%s/%s", dir, pEntry->d_name);
        lstat(tmp, buffer);
        printf("%s \t",pEntry->d_name);
        if(file_num % 3 == 0){
            printf("\n");
        }
        free(buffer);
    }
    printf("\n");
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
        sprintf(tmp, "%s/%s" ,dir, pEntry->d_name);
        lstat(tmp, buffer);
        //file format
        if(pEntry->d_type == DT_DIR )
            printf("d");
        else if(pEntry->d_type == DT_LNK )
            printf("l");
        else
            printf("-");
        //Permission-owners
        if((buffer->st_mode & S_IRUSR) == S_IRUSR)
            printf("r");
        else
            printf("-");
        if((buffer->st_mode & S_IWUSR) == S_IWUSR)
            printf("w");
        else
            printf("-");
        if((buffer->st_mode & S_IXUSR) == S_IXUSR)
            printf("x");
        else
            printf("-");
        //Permission-group
        if((buffer->st_mode & S_IRGRP) == S_IRGRP)
            printf("r");
        else
            printf("-");
        if((buffer->st_mode & S_IWGRP) == S_IWGRP)
            printf("w");
        else
            printf("-");
        if((buffer->st_mode & S_IXGRP) == S_IXGRP)
            printf("x");
        else
            printf("-");
        //Permission-others
        if((buffer->st_mode & S_IROTH) == S_IROTH)
            printf("r");
        else
            printf("-");
        if((buffer->st_mode & S_IWOTH) == S_IWOTH)
            printf("w");
        else
            printf("-");
        if((buffer->st_mode & S_IXOTH) == S_IXOTH)
            printf("x");
        else
            printf("-");
        printf(" ");

        
        //owner
        struct passwd *user = getpwuid(buffer->st_uid);
        printf("%s ",user->pw_name);
        
        //group
        struct group *grp = getgrgid(buffer->st_gid);
        printf("%s ",grp->gr_name);

        //size
        printf("%7ld ",buffer->st_size);

        //time
        time_t rawtime = buffer->st_mtime;
        struct tm *ptr_time = localtime(&rawtime);
        char str[50];
        strftime(str, sizeof(str), "%x %H:%M", ptr_time );
 	    printf("%s ", str );

        //link
        printf("%s", pEntry->d_name);
        if(pEntry->d_type == DT_LNK ){
            char *p = calloc(2048, sizeof(char));
            readlink(tmp, p, 2048);
            printf(" -> %s ",p);
            free(p);
        }
        printf("\n");
        free(buffer);
    }
}
int main(int argc, char** args){
    long size = 0;
    char* dir;
    int flag = 0;
    for(int i = 0;i < argc ; i++){
        if (!strcmp(args[i],"-l")){
            flag = 1;
        }
    }
    if(flag){
        if(argc < 3)
            dir = ".";
        else
            dir = args[1];
        list(dir);
    }
    else{
        if(argc < 2)
            dir = ".";
        else
            dir = args[1];
        open_read(dir);
    }
   
    return 0;
}