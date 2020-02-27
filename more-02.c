#include<stdio.h>
#define PAGELEN 24
#define LINELEN 512
void do_more(FILE*);
int see_more(FILE*);

int main(int ac, char* av[]){

    FILE* fp;
    if(ac == 1){
        do_more(stdin);
    }
    else{
        while(--ac){
            if((fp = fopen(*++av, "r")) != NULL){
                do_more(fp);
                fclose(fp);
            }
            else{
                exit(1);
            }
        }
    }
    return 0;
}

void do_more(FILE* fp){
    char line[LINELEN];
    int num_of_lines = 0;
    int see_more(FILE*), reply;
    FILE* fp_tty;
    /**文件:/dev/tty，是键盘和显示器的设备描述文件，向这个文件写相当于显示在用户的屏幕上，
     * 读相当于从键盘获取用户的输入。即使程序的输入/输出被重定向，程序仍可通过这个文件同终端交换数据
     */ 
    fp_tty = fopen("/dev/tty", "r");
    if(fp_tty == NULL){
        exit(1);
    }
    while(fgets(line, LINELEN, fp)){
        if(num_of_lines == PAGELEN){
            reply = see_more(fp_tty);
            if(reply == 0)
                break;
            num_of_lines -= reply;
        }
        if(fputs(line, stdout) == EOF)
            exit(1);
        num_of_lines++;
    }
}

int see_more(FILE* cmd){
    int c;
    printf("\033[7m more?\033[m");
    while((c == getc(cmd)) != EOF){
        if(c == 'q')
            return 0;
        if(c == ' ')
            return PAGELEN;    
        if(c == '\n')
            return 1;
    }
    return 0;
}