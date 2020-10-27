/**
 * utmp记录当前正在使用系统的用户信息；wtmp记录所有的登录和注销的用户信息，涉及命令：login、init、ac
 * 若文件销毁，则功能失效。
 * /var/adm/utmp  保存了结构数组，数组的元素是utmp类型的结构
 * /var/include/utmp.h  包含了utmp类型的定义
 * utmp.h中
 * ut_user数组保存登录名
 * ut_line数组保存设备名，用户的终端类型
 * ut_time保存登录时间
 * ut_host保存用户用于登录的远程计算机的名字
 * 
 * 由man who和utmp以及utmp.h可推断who的工作原理：
 * who通过读utmp文件来获取重要的信息，而每个登录的用户在utmp文件中都有对应的记录。
 * 数据流：
 * 1. 打开utmp
 * 2. 读取记录
 * 3. 显示记录
 * 4. 关闭utmp
 * 我们要做的
 *  - 从文件中读取数据结构
 *  - 从结构中的信息以合适的形式形式
 *
 * open、read、close  3个系统调用
 * open在进程和文件之间建立一条连接，连接称为文件描述符，像一条由进程通向内核的管道。
 * 1. open 以how的方式打开文件名为name的文件
 *  头文件 #include<fcntl.h>
 *  函数原型 int fd = open(char *name,int how)->open ? -> how(2) to open whcih(1) file ?
 *  参数 name 文件名
 *       how O_RDONLY,O_WRONLY,O_RDWR
 *  返回值　　-1 遇到错误
 *  　　　　　int 文件描述符，用来唯一标识这个连接，可通过文件描述符对文件进行操作
 * 
 * 2. read 请求内核从fd所指向的文件读取qty字节的数据，存放到buf所指向的内存空间(何种形式的数据结构)
 *  头文件 #include<unistd.h>
 *  函数原型 ssize_t numread = read(int fd,void *buf,size_t qty)->put which(1) file to which(2) buffer(缓冲区), and how much(3) ?
 *  参数 fd 文件描述符
 *  buf 存放数据的目的缓存区
 *  qty 要读取的字节数
 *  返回值　　-1 遇到错误
 *  　　　　　numread 成功读取，返回所读取的字节数目
 *          * 0 读取的数据没有所要求的多，读到文末在继续就变为0
 * 
 * 3. close 请求内核关闭fd所指向的文件
 *  头文件 #include<unistd.h>
 *  函数原型 int result = close(int fd)
 *  参数 fd 文件描述符
 *  返回值　　-1 遇到错误
 *  　　　　　0 成功关闭
 * 
 */
// versioin 1 -- open, read UTMP file, and show results, then close file.
#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<utmp.h>
#include<unistd.h>
#include<time.h>

#define SHOWHOST  // 包括了远程主机名
void showtime(long);
void show_info(struct utmp *);

int main(){
    // 读取数据放到utmp数据结构中
    struct utmp utbuf;
    // 从这个文件描述符读取文件
    int utmpfd;
    // UTMP_FILE位于utmp.h,只读的方式打开UTMP_FILE,然后utmpfd接受该文件描述符,并作异常处理
    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
        // perror处理系统错误
        perror(UTMP_FILE);
        exit(1);
    }

    // 每次读取一个utmp数据结构的数据
    int reclean = sizeof(utbuf);

    
    // 在该循环逐条读取reclean大小的数据,存放在utmp数据结构的缓冲区,
    while(read(utmpfd, &utbuf, reclean) == reclean){
        show_info(&utbuf);
    }
    close(utmpfd);
    return 0;
}

void show_info(struct utmp *utbufp){
    if(utbufp->ut_type != USER_PROCESS){
        return;
    }
    printf("% -8.8s", utbufp->ut_user);/*登陆名*/
    printf(" ");
    printf("% -12.12s", utbufp->ut_line);/*终端名*/
    printf(" ");
    showtime(utbufp->ut_time);/*登陆时间*/
    printf(" ");
    #ifdef SHOWHOST
        if(utbufp->ut_host[0] != '\0')
            printf("(%s)", utbufp->ut_host);
    #endif
    printf("\n");
}

// use ctime to build a string then picks parts out of it
void showtime(long timeval){
    char *cp;
    cp = ctime(&timeval);
    printf("%25.25s", cp);
}