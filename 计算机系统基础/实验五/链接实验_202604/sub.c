#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern  int  uninit_gv2;
int   extern_int =5;
 
int  fadd(int x, int y)
{
     static  int  count =0;
     int      result;
     count++;
     printf("enter into fadd count = %d\n", count);
     uninit_gv2 = 10;
     result =  uninit_gv2+10;
     return  result ;
}


void    check_gcc_condition(char *userid) 
{
   int len;
   int x=1,y=2,z=3,u=4,v=5,w=6;
   printf("student id : %s \n", userid);

   len =strlen(userid);
   if (len !=10)  {
       printf("The student number has 10 characters, such as U202415001 \n");
       exit(0);
   }

   #ifdef U0
      if (userid[len-1]=='0')   {
          x=100;  x=~x;  w = 2*x;   
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
   printf(" gcc  -g -o linktest -D U* main.c  sub.c\n");
   printf(" U* : * is the last number of your Student Id . \n");
    printf(" example :  U202415001  ->   -D U1 . \n"); 
   exit(0);
}