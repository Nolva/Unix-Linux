/**
 * 注销过程 -- 文件读写
 * 1. 打开文件utmp --> fd = open(UTMP_FILE, O_RDWR);
 * 
 * 2. 从utmp中找到包含你所在终端的登录记录
 *      while(read(fd, rec, utmplen) == utmplen){ --get next reccord
 *          if((strcmp(rec.ut)line, myline) == 0) --myline
 *              revise_entry();                   --remove my name
 * }
 * 
 * 3. 对当前记录作修改(**)
 *      Q: 在文件操作中，如何改变一个文件的当前读/写位置？
 *      A: 使用系统调用lseek
 * 
 * 4. 关闭文件 -- > close(fd)
 * 
 * 1. lseek   使指针指向 -> fd指向文件中的(base+dist)的位置
 *  头文件 #incldue<unistd.h>              #include<sys/type.h> 
 *  函数原型 off_t oldpos = lseek(int fd,off_t dist, int base)-> lseek -> which(1) file, which location: which(2) dist of which(3) base 
 *  参数  fd 文件描述符
 *       dist 偏移的距离
 *       base SEEK_SET, SEEK_CUR, SEEK_END  分别：起始、当前、末尾位置
 *  返回值　　-1 遇到错误
 *  　　　　　oldpos 指针变化*前*的位置
 * 
 * eg: lseek(fd, -(sizeof(struct utmp)), SEEK_CUR) -- 把指针移到上一个utmp结构数据
 * 
 */

// return -1 on error, 0 on success
#include<stdio.h>
#include<utmp.h>
#include<stdlib.h>
#include <fcntl.h>
// #include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<time.h>
int logout_tty(char *line){
    int fd;
    struct utmp rec;
    int len = sizeof(struct utmp);
    int retval = -1;

    if((fd = open(UTMP_FILE, O_RDWR)) == -1){
        return -1;
    }

    // 寻找目标登录记录，并修改
    // 遍历文件中每一个utmp结构的记录
    while(read(fd, &rec, len) == len){
        if(strncmp(rec.ut_line, line, sizeof(rec.ut_line)) == 0){  // 匹配传入的登录记录(line)
            rec.ut_type = DEAD_PROCESS;  // 设置type为DEAD_PROCESS
            if(time(&rec.ut_time) != -1)  // 设置为当前时间
                if(lseek(fd, -len, SEEK_CUR) != -1)  // 指针跳转到上一个utmp结构
                    if(write(fd, &rec, len) == len)  // 更新，把rec设置好的的utmp结构写入，覆盖我们的登录记录
                        retval = 0;  // 成功
            break;
        }
    }

    // 关闭文件
    if(close(fd) == -1)
        retval = -1;
    
    return retval;
}