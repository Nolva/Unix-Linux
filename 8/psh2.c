#include <stdio.h>
#include <signal.h>
#include <string.h>

#define MAXARGS 20
#define ARGLEN 100

int main() {
    char *arglist[MAXARGS + 1];             // 指针数组
    int numargs;                            // 数组索引
    char argbuf[ARGLEN];                    // 将数据读到这里
    char* makestring();                     // malloc etc
    numargs = 0;                            // 参数个数

    while(numargs < MAXARGS) 
    {
        printf("Arg[%d]? ", numargs);
        if(fgets(argbuf, ARGLEN, stdin) && *argbuf != '\n')
            arglist[numargs++] = makestring(argbuf);

        else
        {
            if(numargs > 0)                 // 有参数的话
            {
                arglist[numargs] = NULL;    // 关闭参数数组，加上末尾的NULL
                execute(arglist);           // excute
                numargs = 0;                // reset
            }
        }
    }
    return 0 ;
}

// 用execvp执行
int execute(char* arglist[])
{
    int pid,exitstatus;             // 子程序的ID和退出状态
    pid = fork();                   // 新建进程
    switch(pid) {
        case -1:
            // 创建子进程错误
            perror("fork failed");
            exit(1);
        case 0:
            // 子进程分支
            execvp(arglist[0], arglist);    // 真正执行的函数
            perror("execvp failed");
            exit(1);
        default:
            // 父进程分支，死循环直到wait返回子进程pid
            while(wait(&exitstatus) != pid)
                ;
            printf("child exited with status %d, %d\n", exitstatus>>8, exitstatus&0377);
    }
}

// 修剪换行符并为字符串创建存储
char* makestring(char* buf)
{
    char *cp;
    // 让字符串最后一位为'\0' (本来是换行符)
    buf[strlen(buf) - 1] = '\0';
    // 申请字符串空间+1，为复制作准备
    cp = malloc(strlen(buf) + 1);
    if(cp == NULL)                      // or die
    {
        fprintf(stderr, "no memory\n");
        exit(1);
    }
    strcpy(cp, buf);
    return cp;
}