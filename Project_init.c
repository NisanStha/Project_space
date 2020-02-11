#include<stdio.h>
#include<conio.h>
#include <Windows.h>
#define gotoxy(x,y) printf("\033[%d;%dH", (y), (x))
	int height=25;
	int width=80;
	int gameover=0;

typedef struct object
{
    int type,life;
};

typedef struct coordinate
{
    int status;  //status=0 null , status=1 something present
    struct object obj;
};

//	enum eDirection{STOP=0, LEFT,RIGHT,UP,DOWN};
//	enum eDirection dir;

void main()
{
	struct coordinate globalco[height][width];
    draw_init_splash();
    draw_init(globalco);
    draw_confirm(globalco);
		/*while(!gameover){
			Draw();
			Input();
			Logic();
			Sleep(10);
		}
		if(score==300) printf("Congrats You won");                  //score not implemented.....yet
		else printf("Too bad.... try again");
		*/
}

void draw_init_splash(){
		int i,j;
		system("cls");
		for (i=1;i<=width;i++)
		{
			if(i==1) printf("%c",201);
			else if(i==(width)) printf("%c",187);           //prints top border
			else printf("%c",205);
		}

		printf("\n");
		for(j=1;j<=height-2;j++){
			for(i=1;i<=width;i++){
				if(i==1 || i==width)
                    printf("%c",186);        //prints side border, pacman(C), "dot(.)",,,
                else
                    printf(" ");
            }
			printf("\n");
		}

		for (i=1;i<=width;i++)
		{
			if(i==1) printf("%c",200);
			else if(i==width) printf("%c",188);           //lower border
			else printf("%c",205);
		}
    for(i=0;i<5;i++)                //Splash screen
     {  gotoxy(30,10);
        printf("GET READY!!!");
        Sleep(500);
        gotoxy(30,10);
        printf("            ");
        Sleep(500); }

    gotoxy(0,0);
    getch();
	}
void draw_init(struct coordinate globalco[][height])
{
    int x,y;
    for (y=2;y<=height-1;y++)
    {
        for (x=2;x<=width-2;x++)
        {
            if(x%2==0)
                globalco[x][y].status =0;
            else
                globalco[x][y].status =1;
        }
    }
}
/*

	void Draw(){
		int i,j;
		system("cls");
		for (i=0;i<width;i++)
		{
			if(i==0) printf("%c",201);
			else if(i==width-1) printf("%c",187);           //prints top border
			else printf("%c",205);
		}

		printf("\n");
		for(i=0;i<height;i++){
			for(j=0;j<width;j++){
				if(j==0 || j==width-1) printf("%c",186);        //prints side border, pacman(C), "dot(.)",,,
                                                                //add ghosts too
				else if(j==x && i==y) printf("C");

				else printf(".");

			}
			printf("\n");
		}

		for (i=0;i<width;i++)
		{
			if(i==0) printf("%c",200);
			else if(i==width-1) printf("%c",188);           //lower border
			else printf("%c",205);
		}

	}

	void Input(){
		if(_kbhit()){
			switch(_getch()){                           //if user presses stuff then only getch() activates and hence takes input
				case 'a':
					dir=LEFT;
					break;

				case 'd':
					dir=RIGHT;
					break;

				case 'w':
					dir=UP;
					break;


				case 's':
					dir=DOWN;
					break;

				case 'x':
					gameover=true;
					break;
			}
		}
	}

	void Logic(){

			switch(dir){                            //pacman's co-ordinate changes when corresponding key is pressed
				case LEFT:
					x--;
					break;
				case RIGHT:
					x++;
					break;
				case UP:
					y--;
					break;
				case DOWN:
					y++;
					break;
			}
		if(x>width) x=width-2;
		if(y>height) y=height-1;
		if(x<0) x=1;
		if(y<0)y=0;
	}
*/
void draw_confirm(struct coordinate globalco[][height])
{
    int x,y;
    for (y=2;y<=height-1;y++)
    {
        for (x=2;x<=width-2;x++)
        { gotoxy(x,y);
            printf("%d",globalco[x][y].status);
        }
    }
}
