#include<stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>

int main(){
    int fd, n_chars;
    if((fd = open("file1", O_RDWR)) == -1){
        perror("Can't open");
        return -1;
    }
    // test1
    lseek(fd, 100, SEEK_END);
    char buf[21];
    n_chars = read(fd, buf, 20);
    printf("%d\n", n_chars);
    printf("%s\n", buf);

    // test2
    char str[] = "test lseek...";
    n_chars = write(fd, str, strlen(str));
    printf("write1: %d\n", n_chars);
    printf("%s\n", str);

    // test3
    lseek(fd, 20000, SEEK_END);
    n_chars = write(fd, str, strlen(str));
    printf("write2: %d\n", n_chars);
    printf("%s\n", str);

    // test4
    char buffer[21];
    n_chars = read(fd, buffer, 20);
    printf("%d\n", n_chars);
    printf("%s\n", buffer);

    return 0;
}