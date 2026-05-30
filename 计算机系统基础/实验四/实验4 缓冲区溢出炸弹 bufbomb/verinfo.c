#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint32_t hash_string(const char *str) {
    uint32_t hash = 0x12345678;
    while (*str) {
        hash = ((hash << 5) + hash) + (*str++);
    }
    return hash;
}

// 自动生成：基于 编译时间 + 源码路径 的唯一随机ID
#define RANDOM_UNIQUE_ID  hash_string(__DATE__ __TIME__ __FILE__)

// 生成随机字符串（固定8位十六进制）
#define RANDOM_STR_LEN 9
char RANDOM_CODE[RANDOM_STR_LEN];

// 初始化随机码
void init_random_code(void) {
    snprintf(RANDOM_CODE, RANDOM_STR_LEN, "%08X", RANDOM_UNIQUE_ID);
}

void version_info( )
{
    init_random_code();
    printf("===== 自动生成的唯一随机代码 =====\n");
    printf("随机ID(十进制)：%u\n", RANDOM_UNIQUE_ID);
    printf("随机代码(十六进制)：%s\n", RANDOM_CODE);
    printf("编译时间：%s %s\n", __DATE__, __TIME__);
    printf("\n");
}

void    initialize_bomb(char *userid) 
{
   int len;
   int x=10,y=20,z=30,u=40,v=50,w=60;
   len =strlen(userid);
   if (len !=10)  {
       printf("The student number has 10 characters, such as U202415003 \n");
       exit(0);
   }
   printf("welcome  %s \n",userid);
   #ifdef U0
      if (userid[len-1]=='0')   {
          x=100;  x=~x;  w = 2*x;   u=x+y+z;
          return;
     }
   #endif
   #ifdef U1
      if (userid[len-1]=='1')   {
            w = 2*x+3*y;   return;
      }
   #endif
   #ifdef U2
      if (userid[len-1]=='2')  {
          w = 2*x+3*y+4*u;  return;
      }
   #endif
   #ifdef U3
      if (userid[len-1]=='3')   {
         w = 2*x+3*y+4*u+x;  return;
      }
   #endif
   #ifdef U4
      if (userid[len-1]=='4')   {
         w = 2*x+3*y+4*u+x+5*w;  return;
      }
   #endif
   #ifdef U5
      if (userid[len-1]=='5')   {
          w =-u;  return;
      }
   #endif
   #ifdef U6
      if (userid[len-1]=='6')   {
        w =-u-v;   return;
      }
   #endif
   #ifdef U7
      if (userid[len-1]=='7')   {
          return;
      }
   #endif
   #ifdef U8
      if (userid[len-1]=='8')   {
           x=4*w+10;   return;
      }
   #endif
   #ifdef U9
      if (userid[len-1]=='9')   {
          x=4*w+5*y;   return;
      }
   #endif
   printf(" gcc  -g -o binarybomb -D U* bomb.c  support.c  phase.o\n");
   printf(" U* : * is the last number of your Student Id . \n");
    printf(" example :  U202415003  ->   -D U3 . \n"); 
   exit(0);
}

