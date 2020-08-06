#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<string.h>

int main(){
    int fd0 , fd1, fd2, n_char;
    if((fd0 = open("file0", O_RDONLY)) == -1){
        perror("Cannot open file:");
        return -1;
    }
    if((fd1 = open("file1", O_WRONLY)) == -1){
        perror("Cannot open file:");
        return -1;
    }
    if((fd2 = open("file2", O_RDWR)) == -1){
        perror("Cannot open file:");
        return -1;
    }
    char buf0[21];
    char buf2[21];
    n_char = read(fd0, buf0, 20);
    printf("file0: %d\n", n_char);
    printf("%s\n", buf0);

    char str[] = "testing 123...";
    n_char = write(fd1, str, strlen(str));
    printf("file1:%d\n", n_char);

    n_char = read(fd2, buf2, 20);
    printf("file2:%d\n", n_char);
    printf("%s\n", buf2);
    
    return 0;
}