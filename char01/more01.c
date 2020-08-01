/**
 * more01.c - version 0.1 of more
 * read and print 24 lines then pause for a few special commands
 * Usage:
 * 1. more filename
 * 2. more
 * Process: 
 * 1.首先调用fopen() 函数打开文件。
 * 2.然后是调用自己写的函数 do_more() ，使用fgets() 与 fputs() 一行一行进行读入和输出。
 * 3.首先输出整个屏幕的文件（假设一个屏幕24行），如果文件超过24行，
 * 那么接着调用see_more()， 根据用户输入，选择输出一行，一页，或者退出。
 * 输出之后接着给用户选择。依次循环，直到出错或者文件读完。
 * 4.最后关闭文件。
 */

#include <stdio.h>
#include<stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *); // more的功能实现
int see_more(); // 询问用户是否继续读文件

// more的逻辑
int main(int ac, char *av[])
{

    FILE *fp;

    // 只有一个参数，即只有more命令，“从标准输入得到要分页显示的内容”
    if (ac == 1)
    {
        do_more(stdin);
    }
    // 多个参数，即参数中有文件名。分页显示此文件
    else
    {
        while (--ac)
        {
            // 如果下一个参数指令所指向的文件能够以只读的方式打开
            if ((fp = fopen(*++av, "r")) != NULL)
            {
                // 进行more0.1
                do_more(fp);
                // 关闭文件
                fclose(fp);
            }

            else
            {
                printf("打开文件错误！\n");
                exit(1);
            }
        }
    }
    return 0;
}

// read PAGELEN lines, then call see_more() for further instructions
void do_more(FILE *fp)
{

    char line[PAGELEN];
    int num_of_lines = 0;
    int see_more(), reply;

    /** 从参数fp所指的文件内读入字符并存到参数line所指的内存空间，
      * 直到出现换行字符、读到文件尾或是已读了LINELEN-1个字符为止，
      * 最后会加上NULL作为字符串结束
      */
    while (fgets(line, LINELEN, fp))
    {

        // 如果屏幕满了，即文件内容超过24行（shell只能显示24行）
        if (num_of_lines == PAGELEN)
        {
            // 输出判断用户是否要继续读文件的格式
            reply = see_more();

            // 对应于more的q指令，reply的值为0，直接退出
            if (reply == 0)
            {
                break;
            }
            // 另外的PAGELEN或1
            num_of_lines -= reply; // 重置计数器num_of_lines，即count
        }

        // fputs输出到stdout标准输出，直到End Of File 文档最后
        if (fputs(line, stdout) == EOF)
        {
            printf("读文档完毕");
            exit(1);
        }
        num_of_lines++; // 读一行，count++，判断是否有24行然后进入循环
    }
}

/**
 * print message, wait for response, return # of lines to advance
 * 1. 'q' means 0, 
 * 2. SPACE means PAGELEN, 
 * 3. '\n' means one line.
 */
int see_more()
{
    int c;
    printf("\033[7m more? \033[m"); // 转换终端显示的颜色，反白字体

    // 当文件没有读到最后， 获取用户的意愿
    while ((c = getchar()) != EOF)
    {
        switch (c)
        {
        case 'q': // 'q' => quit
            return 0;

        case ' ': // SPACE => next page
            return PAGELEN; // 计算有多少行要显示

        case '\n':
            return 1; // Enter => 1line

        default:
            break;
        }
    }
}
