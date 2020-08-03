/**
 *  1. creat 请求内核创建一个filename的文件,如果已经存在,就把它的内容清空,把文件的长度设为0.
 *         如果内核成功创建文件,文件的许可位(permission bits)设置为由第2个参数mode所指定的值.
 *  头文件 #include<fcntl.h>
 *  函数原型 int fd = creat(char *filename, mode_t mode)-> creat? -> how(2) to creat whcih(1) file ?
 *  参数 filename 文件名
 *       mode 访问模式 eg:rw-r--r--
 *  返回值　　-1 遇到错误
 *  　　　　　fd 创建成功.文件描述符，用来唯一标识这个连接，可通过文件描述符对文件进行操作
 * 
 * 2. write 
 * 头文件 #include<unistd.h>
 *  函数原型 ssize_t result = write(int fd, void *buf, size_t amt)-> write? -> from where(2) write to which file(1), and how much ?
 *  参数 fd 文件描述符
 *       buf 内存数据
 *       amt 要写入的字节数
 *  返回值　　-1 遇到错误
 *  　　　　　num written 成功写入的字节数
 * 实际写入可能少于所要求的.
 * 1. 有的系统会对文件的MAXSIZE有限制
 * 2. 磁盘空间满了
 * 因此调用之后必须进行检查并进行异常处理.
 * 
 * cp.c处理的数据流走向:
 * 1. 打开源文件进行读取
 * 2. 打开目标文件进行写入
 * 3. +->从源文件读取数据到buf缓冲区 -- EOF -+
 * 4. -->从buf缓冲区复制并写入到目标文件
 * 5. 关闭源文件
 * 6. 关闭目标文件
 * 
 * version 1 of cp - use read and write with tunable buffer size
 */ 
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644  // rw-r--r--

void oops(char *, char *);

int main(int ac, char *av[]){
    // n_chars缓存区读取in_fd文件的字节数
    int in_fd, out_fd, n_chars;
    char buf[BUFFERSIZE];

    // 检查指令:cp指令参数错误,进行错误处理
    if(ac != 3){
        fprintf(stderr, "usage: %s source destination\n", *av);
        _exit(1);
    }

    // 检测读取的源文件
    if((in_fd = open(av[1], O_RDONLY)) == -1){
        oops("Can not open ", av[1]);
    }

    // 检测写入的目标文件
    if((out_fd = creat(av[2], COPYMODE)) == -1){
        oops("Can not creat ", av[2]);
    }

    // 逐条数据从in_fd文件读取到buf数组,BUFFERSIZE个字节
    while((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0){
        // n_chars缓存区读取in_fd的字节数
        if(write(out_fd, buf, n_chars) != n_chars){
            oops("write error to ", av[2]);
        }
    }

    // read返回-1说明读取文件异常
    if(n_chars == -1){
        oops("read error from ", av[1]);
    }
    if(close(in_fd) == -1 || close(out_fd) == -1){
        oops("error closing files", " ");
    }
}
void oops(char *s1, char *s2){
    fprintf(stderr, "Error: %s", s1);
    perror(s2);
    _exit(1);
}