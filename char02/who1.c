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
 */
#include<stdio.h>
#include<fcntl.h>
#include<utmp.h>
#include<unistd.h>
#include<time.h>
#define SHOWHOST
//优化1.消除空白记录
//优化2.以可读方式显示登录时间
int main(){
    struct utmp utbuf;
    int utmpfd;

    /*打开utmp文件，UTMP_FILE在utmp.h中，指向utmp文件的存放目录*/
    if((utmpfd = open(UTMP_FILE, O_RDONLY)) == -1){
        perror(UTMP_FILE);//报错
        exit(1);//错误退出
    }

    /*读取UTMP_FILE的内容放到current_record中  一次读取一个完整的信息，直到读完为止*/
    while(read(utmpfd, &utbuf, sizeof(utbuf)) == sizeof(utbuf))
        /*将current_record中的内容有序的显示出来*/
        show_info(&utbuf);
    close(utmpfd);/*关闭UTMP_FILE*/
    return 0;
}

show_info(struct utmp* utbufp){
    if(utbufp->ut_type != USER_PROCESS){//1.消除空白记录：筛选出已经登录的
        return;
    }
    printf("% -8.8s", utbufp->ut_name);/*登陆名*/
    printf(" ");
    printf("%-8.8s", utbufp->ut_line);/*终端名*/
    printf(" ");
    showtime(utbufp->ut_time);/*登陆时间*/
    printf(" ");
    #ifdef SHOWHOST
    if(utbufp->ut_host[0] != '\0'){
        printf("(%s)", utbufp->ut_host);
    }
    #endif
    printf("\n");
}
void showtime(long timeval){
    char* cp;
    cp = ctime(&timeval);

    printf("%12.12s", cp+4);
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