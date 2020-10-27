/**
 * version 2 for selfmade more
 * 读入并输出24行然后等待输入特殊指令
 * version 2的特点：从/dev/tty读入到命令行
 * 未解决的问题：
 * 0. 用户输入后需要回车程序才会动作，而且输入的字符会显示出来。
 * Answer: 利用/dev/tty从键盘读入用户输入后，还经过字符处理单元。
 * 利用字符处理单元对输入进一步处理，可以**调整终端参数**使得用户输入的字符被立即送到程序，并让字符不回显。
 * 
 * 1. 如何知道文件中显示的百分比？百分比->文件大小->合适的系统调用
 * (与终端类型相关的问题，若全都宏定义，程序将失去灵活性)
 * 2. 如何反白显示文字？
 * 3. 如何确定每一页的行数？
 * 4. 如何使程序能够处理各种类型的终端？->学会如何控制和调整终端参数
 */
#include <stdio.h>
#include<stdlib.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *); // more的功能实现
// int see_more(); 
int see_more(FILE *);  // 询问用户是否继续读文件

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
            // 如果下一个参数指令所指向的文件能够以只读的方式打开，就继续，否则报错退出
            if ((fp = fopen(*++av, "r")) != NULL)
            {
                // 进行分页显示该文件
                do_more(fp);
                // 关闭文件
                fclose(fp);
            }
            else
            {
                // printf("打开文件错误！\n");
                exit(1);
            }
        }
    }
    return 0;
}
/**
 * read PAGELEN lines, then call see_more() for further instructions
 * more将实现从标准输入读数据，从键盘(/dev/tty)读用户的输入
 */
void do_more(FILE *fp)
{

    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(FILE *), reply;
    FILE *fp_tty;
    
    fp_tty = fopen("/dev/tty", "r");
    if(fp_tty == NULL){
        exit(1);
    }

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
            reply = see_more(fp_tty);

            // 对应于more的q指令，reply的值为0，直接退出读文件操作
            if (reply == 0)
            {
                break;
            }
            // 另外的PAGELEN或1
            num_of_lines -= reply; // 重置计数器num_of_lines，即count
        }

        // fputs输出到stdout标准输出，直到End Of File文档最后
        if (fputs(line, stdout) == EOF)
        {
            // printf("读文档完毕");
            exit(1);
        }
        num_of_lines++; // 读一行，count++，判断是否有24行然后进入循环
    }
}

/**
 * print message, wait for response, return # of lines to advance
 * 1. 'q' means 0 ==> quit, 
 * 2. SPACE means PAGELEN ==> 24 lines, 
 * 3. '\n' means one line ==> 1 line.
 * NEW: accepts arg
 */
int see_more(FILE *cmd)
{
    int c;
    printf("\033[7m more? \033[m"); // 转换终端显示的颜色，反白字体

    // 当文件没有读到最后， 获取用户的意愿
    while ((c = getc(cmd)) != EOF)
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
