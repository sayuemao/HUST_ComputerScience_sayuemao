// 源程序有 两处需要修改
// char  init_gstr1[12]="U202415001";  // 请换成自己的学号
// __asm__  __volatile__(".rept 5 \n nop \n .endr \n");  // 改写源程序，将 5 换成学号的最后1位

#include <stdio.h>
#include <string.h>

int  fadd(int, int);
void    check_gcc_condition(char *userid) ;

#define  NUMBER  6
int   init_gv1 = 10;
int   uninit_gv2;
char  init_gstr1[12]="U202415001";  // 请换成自己的学号
char  uninit_gstr2[12];
int    init_garr1[3]={11, 12, 13};
int    uninit_garr2[3];
int    *init_gp1 = &init_gv1;     // 该指针中的内容需要重定位，用本文件可见的符号
int    *init_gp2 = &uninit_gv2;
int    *uninit_gp2;
int    (*init_gfp1)(int, int) = fadd;
int    (*uninit_gfp2)(int, int);
extern int  extern_int;
int   gv=NUMBER;

void fcount( )
{
    int x=0;
    static int count=0;
    x++;
    count ++;
    printf("auto x = %d , static count =%d\n",x,count);
}

static int  fsub(int x, int y)
{
    int  result;
    char fmt[ ] = "static fsub : uninit_gv2 = %d \n";
    uninit_gp2 = &uninit_gv2;
    *uninit_gp2 = 12;
    printf(fmt, uninit_gv2);
    result = x -y;
    return  result;
}

int main( )
{
     check_gcc_condition(init_gstr1);   //检查学号与条件编译是否一致
     fcount( );
     fcount( );
     __asm__  __volatile__(".rept 5 \n nop \n .endr \n");  // 改写源程序，将 5 换成学号的最后1位

     int  lv1=15;
     int  lv2 = 17;
     int  sum = 0;
     sum = lv1 +lv2;
     init_gv1=20;
     uninit_gv2 = init_gv1;
     strcpy(uninit_gstr2,"xuxy");   // 请换成自己的姓名拼音
     
     extern_int =21;
     printf("display init_gstr1 : %s \n", init_gstr1);
     sum = fadd(init_gv1, lv1);
     sum = fsub(50,30);
     sum= init_gfp1(50,30);
     printf("sum = %d \n", sum);
     return 0;
}
