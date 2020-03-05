#include<stdio.h>
#include<utmp.h>
#include<fcntl.h>
#include<unistd.h>

#define SHOWHOST

int main(){
    struct utmp current_record;
    int utmpfd;
    int reclen = sizeof(current_record);

    /*打开utmp文件，UTMP_FILE在utmp.h中，指向utmp文件的存放目录*/
    if((utmpfd = open(URMP_FILE, O_RDONLY)) == -1){
        perror(UTMP_FILE);//报错
        exit(1);//错误退出
    }

    /*读取UTMP_FILE的内容放到current_record中  一次读取一个完整的信息，直到读完为止*/
    while(read(utmp, &current_record, reclen) == reclen)
        /*将current_record中的内容有序的显示出来*/
        show_info(&current_record);
    close(utmpfd);/*关闭UTMP_FILE*/
    return 0;
}

show_info(struct utmp* utbufp){
    printf("% -8.8s", utbufp->ut_name);/*登陆名*/
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);/*终端名*/
    printf(" ");
    printf("%101d", utbufp->ut_time);/*登陆时间*/
    printf(" ");
    #ifdef SHOWHOST
    printf("(%s)", utbufp->ut_host);/*host*/
    #endif
    printf("\n");
}

/**
 * 1 open
    头文件 #include<fcntl.h>
    函数原型 int fd = open(char *name,int how)
    参数 name 文件名
    how O_RDONLY,O_WRONLY,O_RDWR
    返回值　　-1 遇到错误
    　　　　　int 文件描述符，用来唯一标识这个连接，可通过文件描述符对文件进行操作
 
    2 read
    将数据读到缓冲区
    头文件 #include<unistd.h>
    函数原型 ssize_t numread = read(int fd,void *buf,size_t qty)
    参数 fd 文件描述符
    buf 存放数据的目的缓存区
    qty 要读取的字节数
    返回值　　-1 遇到错误
    　　　　　numread 成功读取，返回所读取的字节数目
    
    3 close
    关闭一个文件
    头文件 #include<unistd.h>
    函数原型 int result = close(int fd)
    参数 fd 文件描述符
    返回值　　-1 遇到错误
    　　　　　0 成功关闭
 * 
 * 
 */ 