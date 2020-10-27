#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "smsh.h"

#define DFL_PROMPT ">"

int main() {
    char *cmdline, *prompt, **arglist;
    int result， process(char**);
    void setup();

    prompt = DFL_PROMPT;
    setup();

    // next_cmd从输入流读入下一个命令
    while((cmdline = next_cmd(prompt, stdin)) != NULL) {
        // splitline将一个字符串分解为字符串数组，并返回这个数组
        if((arglist = splitline(cmdline)) != NULL) {
            // execute使用fork、execvp和wait来运行一个命令
            result = process(arglist);
            freelist(arglist);
        }
        free(cmdline);
    }
    return 0;
}

/**
 * 初始化shell
 * 无返回，有错误则调用fatal()
 */ 
void setup() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_IGN);
}

void fatal(char* s1, char* s2, int n) {
    fprintf(stderr, "Error: %s, %s\n", s1, s2);
    exit(n);
}