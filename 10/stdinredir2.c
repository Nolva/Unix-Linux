#include <stdio.h>
#include <fcntl.h>

/* #define CLOSE_DUP /* open,close,dup,close */
/* #define USE_DUP2  /* open,dup2,close */

// 显示了另外两种重定向标准输入的方法
// 使用#define设置一个，#else设置另一个
main() {
    int fd;
    int newfd;
    char line[100];

    // read and print three lines
    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);
    // 输出stdin读取到的字符串

    // 重定向输入
    fd = open("data", O_RDONLY);
#ifdef CLOSE_DUP
    close(0);
    newfd = dup(fd);
#else
    newfd = dup2(fd,0); // 等价于#ifdef 关闭0，复制fd到0
#endif
    if(newfd != 0) {
        fprintf(stderr, "Could not duplicate fd to 0\n");
        exit(1);
    }
    close(fd);          // 关闭原始的文件描述符fd

    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);

    return 0;

}