// 自制的shell--执行命令的C文件代码
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

/**
 * 使用fork、execvp和wait来运行一个命令。
 * 调用malloc来分配内存以接受任意长度的命令行。
 * 碰到文件结束符EOF，返回NULL
 */ 
int execute(char *argv[]) {
    int pid;
    int child_info = -1;

    // 命令行为空则返回0
    if(argv[0] == NULL) 
        return 0;

    // 执行fork()，出错报错
    if((pid = fork()) == -1) 
        perror("fork");
    // 子进程分支
    else if(pid == 0) {
        // CTRL+C 或者是DELETE 无法中断
        signal(SIGINT, SIG_DFL);
        // CTRL+\ 无法中断
        signal(SIGQUIT, SIG_DFL);
        // 执行命令
        execvp(argv[0], argv);
        // 出错则退出
        perror("cannot execute command");
        exit(1);
    }
    // 父进程分支
    else{
        // 接收的fork_return_value有错
        if(wait(&child_info) == -1)
            perror("wait");
        return child_info;
    }
}