/** 通知
 * 显示了父进程如何获取子进程的退出状态
 */ 
#include <stdio.h>
#define DELAY 5
main() {
    int newpid;
    void child_code(), parent_code();
    printf("before:mypid is %d\n", getpid());
    if((newpid = fork()) == -1)
        perror("fork");
    else if(newpid == 0)
        child_code(DELAY);
    else
        parent_code(newpid);
}
// 新进程睡眠一会就退出
void child_code(int delay) {
    printf("child %d here. will sleep for %d seconds\n", getpid(), delay);
        sleep(delay);
    printf("child done.about to exit\n");
    exit(17);
}
// 父进程等待子进程然后输出信息
void parent_code(int childpid) {
    int wait_rv;            // return value from wait()
    int child_status;
    int high_8,low_7,bit_7;
    
    wait_rv = wait(&child_status);          // 传入整型变量的地址，存储子进程的退出状态
    printf("done waiting for %d. Wait returned:%d\n",childpid,wait_rv);

    // Assembly: 取高8位(右移8位)，取低8位(与操作&0xFF)
    high_8 = child_status >> 8;         // result:0x高8位, 右移8位: 将高8位移到低8位，高位补0;
    low_7 = child_status & 0x7f;        // 0000 0000 0111 1111
    bit_7 = child_status & 0x80;        // 0000 0000 1000 0000
    printf("status:exit = %d, sig = %d, core = %d\n",high_8,low_7,bit_7);
}