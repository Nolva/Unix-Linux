/*  pipedemo.c	* Demonstrates: how to create and use a pipe
 *
 * 效果：创建一个管道，写入writingend，
 * 然后运行并从readingend读取
 * 有点奇怪，但就是这个思路
 */
#include <stdio.h>
#include <unistd.h>

main()
{
    int len, i, tempPipe[2]; /* two file descriptors */
    char buf[BUFSIZ];        /* for reading end	*/

    /* get a pipe */
    if (pipe(tempPipe) == -1)
    {
        perror("could not make pipe");
        exit(1);
    }
    printf("Got a pipe! It is file descriptors: { %d %d }\n",
           tempPipe[0], tempPipe[1]);

    /* read from stdin, write into pipe, read from pipe, print */

    while (fgets(buf, BUFSIZ, stdin))
    {
        len = strlen(buf);
        if (write(tempPipe[1], buf, len) != len)
        {                              /* send	*/
            perror("writing to pipe"); /* down */
            break;                     /* pipe */
        }
        for (i = 0; i < len; i++) /* wipe	*/
            buf[i] = 'X';
        len = read(tempPipe[0], buf, BUFSIZ); /* read */
        if (len == -1)
        {                                /* from */
            perror("reading from pipe"); /* pipe */
            break;
        }
        if (write(1, buf, len) != len)
        {                                /* send  */
            perror("writing to stdout"); /* to    */
            break;                       /* stdout */
        }
    }
}
