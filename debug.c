#include<stdio.h>
#include<conio.h>
#include <Windows.h>
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))

struct master
{
    int x,y;
    struct sub mini;
};
struct sub
{
    int type;

};

void main()
{   struct master mega;
    mega.x=0;
    mega.y=1;
    printf("hello world");
    printf("%d %d  \n", mega.x, mega.y);
    fun(&mega);

}

void fun(struct master *mega)
{
    printf("%d %d", mega->x, mega->y);
}
