/**
 * 演示如何创建从一个进程到另一个进程的管道
 * 需要两个参数，每个参数一个命令，并将*av[1]的输出连接到av[2]的输入*
 * 用法：pipe command1 command2
 * 效果：command1 | command2
 * 限制：命令不带参数
 * 使用execlp（），因为参数的个数已知
 * 注意：交换父子进程并观看乐趣
 */
#include <stdio.h>
#include <unistd.h>

#define oops(m, x) \
    {              \
        perror(m); \
        exit(x);   \
    }

main(int ac, char **av)
{

    int thepipe[2], /* two file descriptors	*/
        // newfd,      /* useful for pipes	*/
        pid;        /* and the pid		*/

    // 命令个数不正确
    if (ac != 3)
    {
        fprintf(stderr, "usage: pipe cmd1 cmd2\n");
        exit(1);
    }
    // get a pipe
    if (pipe(thepipe) == -1)
        oops("Cannot get a pipe", 1);

    /* ------------------------------------------------------------ */
    /*	now we have a pipe, now let's get two processes		*/
    // get a process
    if ((pid = fork()) == -1)
        oops("Cannot fork", 2);

    /* ------------------------------------------------------------ */
    /* 	Right Here, there are two processes			*/
    /*             parent will read from pipe			*/
    // parent will exec av[2]
    if (pid > 0)
    {
        // 父进程才进入
        // parent doesn't write to pipe
        close(thepipe[1]);

        // 关闭原本的0(stdin)，复制thepipe[0]到0
        if (dup2(thepipe[0], 0) == -1)
        {
            oops("Could not redirect stdin", 3);
        }
        // stdin is duped, close pipe
        close(thepipe[0]);
        execlp(av[2], av[2], NULL);
        oops(av[2], 4);
    }

    /*	 child execs av[1] and writes into pipe			*/
    // child doesn't read from pipe
    close(thepipe[0]);
    // 关闭原本的1(stdout)，复制thepipe[1]到1
    if (dup2(thepipe[1], 1) == -1)
    {
        oops("Could not redirect stdout", 4);
    }
    // stdout is duped, close pipe
    close(thepipe[1]);
    execlp(av[1], av[1], NULL);
    oops(av[1], 5);

    return 0;
}