#include<stdio.h>
#include<errno.h>
#include<fcntl.h>

/**
 * perror(string) 自己查找错误代码，在标准错误输出string后，显示相应的错误信息
 * 
 */ 
int sample(){
    int fd;
    fd = open("file", O_RDONLY);
    if(fd == -1){
        perror("Cannot open file");
        return ;
    }
}