/** 通知
 * 显示了子进程调用exit是如何触发wait返回父进程的
 */ 
#include <stdio.h>
#define DELAY 2
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
    wait_rv = wait(NULL);
    printf("done waiting for %d. Wait returned:%d\n",childpid,wait_rv);
}