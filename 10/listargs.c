#include<stdio.h>

main(int ac, char* av[]) {
    int i;
    printf("Number of args: %d, Args are: \n",ac);
    for(i = 0; i < ac; i++) 
        printf("args[%d] %s\n", i, av[i]);

    // 将信息输出到stderr
    fprintf(stderr, "This message is sent to stderr\n");
}
