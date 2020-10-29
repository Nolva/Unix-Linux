#include <stdio.h>
#include <fcntl.h>

/**
 * close-then-open:
 * 目的：演示如何通过将文件描述符0替换为文件的连接来重定向标准输入
 * 操作：从标准输入中读取三行，然后关闭fd0，
 * 打开一个磁盘文件，然后从标准输入中再读取三行
 */ 
main() {
    int fd;
    char line[100];

    // read and print three lines
    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);
    // 输出stdin读取到的字符串

    // redirect input
    close(0);

    fd = open("/etc/passwd", O_RDONLY);
    if(fd != 0) {
        fprintf(stderr, "Could not open data as fd0\n");
        exit(1);
    }

    // read and print three lines
    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);
    fgets(line, 100, stdin);printf("%s", line);
    // 输出从文件/etc/passwd读取到的前三行
    // root:x:0:0:root:/root:/bin/bash
    // daemon:x:1:1:daemon:/usr/sbin:/usr/sbin/nologin
    // bin:x:2:2:bin:/bin:/usr/sbin/nologin

    return 0;
}