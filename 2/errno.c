#include<stdio.h>
#include<errno.h>
#include <fcntl.h>
/**
 * 内核通过全剧变量errno来指明错误类型，每个程序都可以访问errno
 * 
 */ 
int sample(){
    int fd;
    fd = open("file", O_RDONLY);
    if(fd == -1){
        printf("Cannot open file: ");
        if(errno == ENOENT){
            printf("There is no such file");
        }
        else if(errno == EINTR){
            printf("Interrupted while opening file.");
        }
        else if(errno == EACCES){
            printf("You do not have permission to open file");
        }
        else if(errno == EIO){
            printf("etc.......");
        }
    }

    return;
}