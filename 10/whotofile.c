#include <stdio.h>

/**
 * 目的：演示如何重定向另一个程序的输出
 * 想法：fork()，然后在子程序中，重定向输出，再执行
 */ 
main() {
    int pid;
    int fd;
    printf("About to run who into a file\n");

    if((pid = fork()) == -1) {
        perror("fork");exit(1);
    }

    if(pid == 0) {
        close(1);
        fd = creat("userlist", 0644);
        execlp("who", "who", NULL);
        perror("execlp");
        exit(1);
    }

    if(pid != 0) {
        wait(NULL);
        printf("Done running who. results in userlist.\n");
    }
    return 0;
}