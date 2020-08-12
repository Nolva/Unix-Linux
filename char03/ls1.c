#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

void do_ls(char[]);

int main(int ac, char *av[]){

    if(ac == 1){
        do_ls(".");
    }
    else{
        while(--ac){
            printf("%s:\n", *++av);
            do_ls(*av);
        }
    }
    return 0;
}
/**
 * list files in directory called dirname
 * struct dirent{
 *      ino_t d_ino;
 *      off_t d_off;
 *      unsigned shorted d_reclen;
 *      char d_name[1];   (how can he do this ?)
 * }
 */ 
void do_ls(char direname[]){

    // 目录指针: opendir(char*)返回DIR*，我们可以据此判断是否存在direname目录
    DIR *dir_ptr;
    // 目录下的每一个入口
    struct dirent *direntptr;

    if((dir_ptr = opendir(direname)) == NULL){
        fprintf(stderr, "ls1: cannot open %s\n", direname);
    }else{
        while((direntptr = readdir(dir_ptr)) != NULL){
            printf("%s\n", direntptr->d_name);
        }
        closedir(dir_ptr);
    }
}