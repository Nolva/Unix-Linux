/**utmplib.c - 从utmp文件读取数据到缓冲区
 * 
 *   函数：
 *     1. utmp_open(filename) - 打开文件
 *          return -1 on error
 * 
 *     2. utmp_next(filename) - 返回 指向下一个utmp结构的指针
 *          return NULL on EOF
 * 
 *     3. utmp_next(filename) - 关闭文件
 * 
 *   每次读取都要访问NRECS(缓冲区中utmp结构的个数)，并将它们从缓冲区分配出去 
 * 
 * features:
 *   禁止空记录
 *   格式化时间
 *   缓冲技术: （使用utplib）一次读取大量数据放入缓冲区，需要的时候再缓存区读取
 * 
 *  read: 把数据从内核缓冲区复制到进程缓冲区
 * write: 把书记从进程缓冲区复制到内核缓冲区，并非与磁盘数据进行交换
 * 因此，不是所有的write操作都会导致内核的写入磁盘，而是内核缓冲区积累一定量之后再一次写入。
 * 
 * 断电后，内核尚未把内核缓冲区的数据写入磁盘，此次更新的数据将会丢失。
 */ 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>

#define SHOWHOST
#define NULLUT ((struct utmp *) NULL)

extern void show_info(struct utmp *utmp_record);
//不用头文件，直接声明全在 utmplib.c 的函数。
extern int utmp_open(char *filename);
extern struct utmp *utmp_next(void);
extern int utmp_reload(void);
extern void utmp_close(void);

int main(int argc, char *argv[]) {
  struct utmp *utbufp;
  int utmp_fd = utmp_open(UTMP_FILE);

  if (utmp_fd == -1) {
    perror(UTMP_FILE);
    exit(EXIT_FAILURE);
  }

  while ((utbufp = utmp_next()) != NULLUT)
    show_info(utbufp);

  utmp_close();
  return EXIT_SUCCESS;
}

extern void show_info(struct utmp *utmp_record) {
  if (utmp_record->ut_type != USER_PROCESS) return;

  printf("%-8.8s", utmp_record->ut_user);
  printf(" ");
  printf("%-8.8s", utmp_record->ut_line);
  printf(" ");
  printf("%12.12s", ctime(&utmp_record->ut_time) + 4);
  printf(" ");
#ifdef SHOWHOST
  printf("(%s)", utmp_record->ut_host);
#endif
  printf("\n");
}
// use ctime to build a string then picks parts out of it
void showtime(long timeval){
    char *cp;
    cp = ctime(&timeval);
    printf("%25.25s", cp);
}