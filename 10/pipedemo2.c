#include <stdio.h>

#define CHILD_MESS "I want a cookie\n"
#define PAR_MESS "testing..\n"
#define oops(m,x) {perror(m); exit(x);}

int main() {
    int pipefd[2];
    int len;
    char buf[BUFSIZ];
    int read_len;

    // get a pipe
    if(pipe(pipefd) == -1) {
        oops("cannot get a pipe", 1);
    }

    // fork
    switch(fork()) {
        case -1:
            oops("cannot fork", 2);

        // child writes to pipe every 5s
        case 0:
            len = strlen(CHILD_MESS);       // "I want a cookie\n"
            // 死循环
            while(1) {
                // 写入到管道，并沉睡5s
                if(write(pipefd[1], CHILD_MESS, len) != len) {
                    oops("write", 3);
                }
                sleep(5);
            }

        // parent reads from pipe and also writes to pipe
        default:
            len = strlen(PAR_MESS);
            while(1) {
                // 写入到管道，并沉睡1s
                if(write(pipefd[1], PAR_MESS, len) != len) {
                    oops("write", 4);
                }
                sleep(1);
                // 从管道读取
                read_len = read(pipefd[0], buf, BUFSIZ);
                if(read_len <= 0) {
                    break;
                }
                // 将从管道读取的字符串，输出到stdout
                write(1, buf, read_len);
            }
    }
    return 0;
}