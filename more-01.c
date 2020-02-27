//编译：cc more-01.c -o more-01
//运行:more-01 more-01.c
#include<stdio.h>
#define PAGELEN 24
#define LINELEN 512

void do_more(FILE *);
int see_more();

/**av中存放的是：命令名称+第一个参数+第二个参数+第三个参数...
 * ac中存放的是av这个数组中的元素个数
 * 例如：more1 more1.c
 * av[0]="more1", av[1]="more1.c", ac=2
 */ 
int main(int ac, char av[]){
    FILE *fp;// 定义一个FILE指针

    if(ac == 1){// 一个参数则从标准输入获取数据
        do_more(stdin);
    }
    else{//不止一个参数则从文件获取数据
        // 显示信息从文件读取
        while(--ac){

            /**函数原型：FILE * fopen(const char * path,const char * mode);
             * path表示文件路径、mode表示打开方式、函数返回文件内容
             * mode="r"　　 以只读方式打开文件，该文件必须存在。
             * mode="w"　　 打开只写文件，若文件存在则文件长度清为0，即该文件内容会消失。
             * 若文件不存在则建立该文件。
             */ 
            if((fp = fopen(*++av, "r")) != NULL){// 打开文件并放入fp中
                do_more(fp);
                fclose(fp);// 关闭文件
            }
            else{// 打开文件异常
                exit(1);
            }
        }
    }
    return 0;
}

/**将数据显示在屏幕上，满屏后，调用see_more()接收用户的输入
 */ 
void do_more(FILE* fp){
    
    char line[LINELEN];
    int num_of_lines = 0;
    int reply;

    /**函数原型：char* fgets(char* s, int n, FILE* stream);
     * 从文件指针stream中读取n-1个字符，存到以s为起始地址的空间里，直到读完一行
     * 如果成功返回s的指针；失败返回NULL。
     */ 
    while(fgets(line, LINELEN, fp)){  // more input
        if( num_of_lines == PAGELEN ){// 目前已显示行数等于最大显示行数
            reply = see_more();       // 等待用户输入
            if( reply == 0)           // 用户输入q或发生错误，退出do_more函数
                break;              
            num_of_lines -= reply;    // 若用户输入Enter，当前已显示函数归0；若用户输入回车，当前已显示行数减1
        } 

        /**函数原型：int fputs(char *str, FILE *fp)：
         * 向指定的文件写入一个字符串(不自动写入字符串结束符'\0')。
         * 成功写入一个字符后，文件的位置指针会自动后移，函数返回0；
         * 否则返回EOF(符号常量，值-1)
         */ 
        if( fputs(line, stdout) == EOF )// 将line里的字符写入屏幕
            exit(1);                  // EOFile，退出
        
        num_of_lines++;               // 目前已显示行数+1
    }
}

/*print message,wait for response,return # of lines to advance
* q means no,space means yes,CR means one line
*/
int see_more(){
    int c;
    printf("\033[7m more? \033[m");// 输出“more?”提示符

    /** #define getchar() getc(stdin);
     * 从标准输入中读取一个字符，当程序调用getchar()时，程序就等着用户按键，用户输入的字符被存放在键盘缓冲区中，
     * 到用户按回车为止，当用户键入回车之后，getchar()才开始从stdin流中每次读入一个字符，
     * 其返回值是用户输入的第一个字符，若读取出错则返回-1(EOF)
     */ 
    while((c = getchar()) != EOF){// 等待用户输入
        if(c == 'q'){
            // 用户输入q字符，退出see_more并返回0
            return 0;
        }
        if(c == ' '){
            // 用户输入空格符，退出see_more并返回PAGELEN值
            return PAGELEN;
        }
        if(c == '\n'){
            //用户输入回车符，退出see_more并返回1
            return 1;
        }
    }
    return 0;// getchar()出错
}