#include<stdio.h>
#include<conio.h>
#include <Windows.h>
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
struct sub  {
                int type;
            };

struct master{
                int x,y;
                struct sub mini;
             };

void main()
{   struct master mega[10][10];
    mega[1][1].x=10;
    mega[1][1].y=10;
    printf("hello world");
    printf("%d %d  \n", mega[1][1].x, mega[1][1].y);
    fun(mega);
    printf("%d %d  \n", mega[1][1].x, mega[1][1].y);
}

void fun(struct master meg[][10])
{
    printf("%d %d\n", meg[1][1].x, meg[1][1].y);
    meg[1][1].y=20;
}
