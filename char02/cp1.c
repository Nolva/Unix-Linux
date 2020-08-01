/**
 * uses read and write with tunable buffer size
 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFERSIZE 4096
#define COPYMODE 0644

void oops(char *, char *);

int main(int argc, char *argv[]) {
  int in_fd, out_fd, n_chars;
  char buf[BUFFERSIZE];

  if (argc != 3) {/*命令的参数不完整*/
    fprintf(stderr, "usage: %s source destination\n", *argv);
    exit(1);
  }

  if ((in_fd = open(argv[1], O_RDONLY)) == -1)/*以只读方式打开被复制文件*/
    oops("Cannot open ", argv[1]);

  if ((out_fd = creat(argv[2], COPYMODE)) == -1)/*创建输出文件*/
    oops("Cannot create ", argv[2]);

  // copy files
  while ((n_chars = read(in_fd, buf, BUFFERSIZE)) > 0) {/*每次从被复制文件中读取一段内容*/
    if (write(out_fd, buf, n_chars) != n_chars) /*向输出文件写这段内容*/
      oops("Write error to ", argv[2]);
  }

  if (n_chars == -1)/*从被复制文件中读取内容时发生错误*/
    oops("Read error from ", argv[1]);

  if (close(in_fd) == -1 || close(out_fd) == -1)/*关闭被复制文件和输出文件*/
    oops("Error closing files ", "");

  return 0;
}

void oops(char *s1, char *s2) {
  fprintf(stderr, "Error: %s ", s1);/*重新定向stderr，使perror作用时，其错误原因为s1的内容*/
  perror(s2);
  exit(1);
}