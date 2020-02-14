#include<stdio.h>
#include<conio.h>
#include <Windows.h>

void gotoxy(short x, short y)
{
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}
    long frame=0;
    int bullet_shot=0;
    int height=26;
    int width=60;
    int gameover=0;
    int dir_player=0;
    int shoot=0;
    int score=0;
    int player_bullet_used=0;
    int player_max_bullet=3;
    int enemy_dir_y=0;
    int enemy_dir_x=1;
    //char buffer_clearer[100];

typedef struct bullet
{
    int x,y,dy,last_y;
    int dead;
    int used_or_not;       //bullet slot used or not
};

typedef struct object
{
    int x,y;
    float dx;
    int life;
    int last_x,last_y;
};

void enemy_move_compute(struct object enemy[],struct object *phantom_left,struct object *phantom_rig)
{
    int i;
            gotoxy(2,27);
            printf("phantom l x :%d",phantom_left->x);
            printf("phantom r x :%d",phantom_rig->x);

        if (phantom_left->x<1)
        {   gotoxy(1,26);
            //printf("phantom l x :%d",phantom_left->x);
            enemy_dir_y=1;
            enemy_dir_x=1;
        }
        if (phantom_rig->x>56)
        {
            enemy_dir_y=1;
            enemy_dir_x=-1;
        }
    phantom_left->x +=enemy_dir_x;
    phantom_rig->x  +=enemy_dir_x;

    for (i=0;i<=75;i++)
    {   enemy[i].last_x=enemy[i].x;
        enemy[i].last_y=enemy[i].y;
        enemy[i].x += enemy_dir_x;
        enemy[i].y += enemy_dir_y;
    }
    phantom_rig->y+=enemy_dir_y;
    phantom_left->y+=enemy_dir_y;
    enemy_dir_y=0;
    if (phantom_rig->y==(height-1))
        gameover=1;
}
void main()
{
    int i,j;
    struct bullet player_bullet[player_max_bullet];
    struct object player;
    struct object enemy[76];
    struct object phantom_left;
    struct object phantom_rig;
    init_struct_enemy(enemy,&phantom_left,&phantom_rig);
    draw_init_splash();      // Border and get ready

    player.x=40;             // player position initialization
    player.y=24;             // player position initialization

    for(i=1 ; i <= player_max_bullet ; i++)
    {
        player_bullet[i].used_or_not=0;            //empties all player bullet slot
        player_bullet[i].x=0;
        player_bullet[i].y=0;
        player_bullet[i].dead=0;
    }
    phantom_left.x  = 5;
    phantom_rig.x = 55;
    phantom_left.y  = 6;
    phantom_rig.y = 6;
    while(!gameover)
        {
            if(player.last_x != player.x)
            draw_player(&player);       //update player position

            draw_struct_enemy(enemy);

            //if (frame%1==0)
                enemy_move_compute(enemy,&phantom_left,&phantom_rig);
            draw_player_bullet(player_bullet); //draws player bullet
            input();                    //take input
            //printf("\n");
            //gotoxy(1,26);

        //compute position of player

            player.last_x = player.x;   //save last position in temporary variable to print empty space and clean screen at the point ; refer to void draw_player();
            player.x += dir_player;     //update to new position according to input(); from user
            dir_player=0;               //reset move direction


        //compute shoot status of player
            if (shoot==1 && player_bullet_used < player_max_bullet)
            {
                player_bullet_used++;                           //increases counter which tracks no of active bullets
                for(i=1;player_bullet[i].used_or_not==1;i++);   //finds empty bullet slot at i

                if (player_bullet[i].used_or_not != 1)
                player_bullet[i].used_or_not=1;                 //changes empty bullet slot status to used
                player_bullet_release(player_bullet,&player,i);  //Sets position & direction of fired bullet  and i= bullet index

                //display no of bullets successfully fired in game
                gotoxy(1,26);
                printf("shot: %d",++bullet_shot);
            }

            shoot=0;                                        //resets shooting status for player
        //debugger
        gotoxy(1,27);
        for(j=1 ; j <= player_max_bullet ; j++)
            printf(" \n  bullet status:%d=%d",j,player_bullet[j].used_or_not);


            collision(player_bullet,enemy);
            gotoxy(80,26);
            printf("frame: %d", ++frame);
            Sleep(50); //frame rate control
        }

    if(score==300) printf("Congrats You won");                  //score not implemented.....yet
    else printf("Too bad.... try again");

}

void init_struct_enemy(struct object enemy[],struct object *phantom_left,struct object *phantom_rig)
{
    int c,i=5,j=2;
    for (c=1;c<=75;c++)
    {
        enemy[c].life=1;
        i+=2;
        enemy[c].x =i;
        enemy[c].y =j;
        if (c%25==0)
        {
            i=5;
            j+=2;
        }

    }

}

void draw_struct_enemy(struct object enemy[])
{
    int i;
    for (i=1;i<=75;i++)
    {
        if (enemy[i].life != 0)
        {
            gotoxy(enemy[i].x,enemy[i].y);
            printf("%c",157);
            gotoxy(enemy[i].last_x,enemy[i].last_y);
            printf(" ",157);
        }
    }
    gotoxy(1,26);

}

void draw_init_splash()
{
    int i,j;
    system("cls");
    for (i=1;i<=width;i++)
    {
        if(i==1) printf("%c",201);
        else if(i==(width)) printf("%c",187);           //prints top border
        else printf("%c",205);
    }

    printf("\n");

    for(j=1;j<=height-2;j++)
    {
        for(i=1;i<=width;i++)
        {
            if(i==1 || i==width)
                printf("%c",186);        //prints side border,
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

 /* for(i=0;i<5;i++)                //Splash screen "get ready!".
    {
        gotoxy(30,10);
        printf("GET READY!!!");
        Sleep(500);
        gotoxy(30,10);
        printf("            ");
        Sleep(50);
    }
*/
        gotoxy(1,26);
}


void draw_player(struct object *player)
{
   if (player->x != player->last_x)
   {
        gotoxy(player->x,player->y);
        printf("%c",142);
        gotoxy(player->last_x,player->y);
        printf(" ");
    }
    gotoxy(1,26);
}

void input()
{
if(_kbhit())
{
switch(_getch())
{                           //if user presses stuff then only getch() activates and hence takes input
case 'a':
dir_player=-1;
break;

case 'd':
dir_player=1;
break;

case 's':
shoot = 1;

break;
case 'x':
gameover=1;
break;
                default:
                    shoot=0;
                    dir_player=0;
                    break;
}
}

}


 void player_bullet_release(struct bullet player_bullet[],struct object *player, int i)
    {   int j;
            player_bullet[i].x = player->x;
            player_bullet[i].y = (player->y-1);

            //data check
            gotoxy(1,27);
            printf("player position %d, %d",player->x, player->y);
            printf(" bullet: %d init_pos: %d, %d",i,player_bullet[i].x,player_bullet[i].y);

    }

 void draw_player_bullet(struct bullet player_bullet[])
    {int i,cursor_debug=1;
        for(i=1;i<=player_max_bullet;i++)
        {
// Debug Tool
//gotoxy(cursor_debug,28);
//cursor_debug+=25;
//printf("bullet: %d pos:%d, %d   ",i,player_bullet[i].x,player_bullet[i].y);
            if (player_bullet[i].used_or_not==1)
            {
                    player_bullet[i].last_y=player_bullet[i].y; //store current line of bullet in temp
                    player_bullet[i].y -= 1;  //update bullet one step up

                    gotoxy(player_bullet[i].x,player_bullet[i].last_y); //clear last position of bullet
                    printf(" ");
                    gotoxy(player_bullet[i].x,player_bullet[i].y);
                    printf("1");

                if ( player_bullet[i].y <= 2)
                    player_bullet[i].dead=1;
            }

            //if dead remove bullet
            if (player_bullet[i].dead==1)
            {
                gotoxy(player_bullet[i].x,player_bullet[i].y); //clear last position of bullet
                printf(" ");
                player_bullet[i].used_or_not=0; //reset used status
                player_bullet_used--;           //update number of active bullet
                player_bullet[i].dead=0;  //reset dead

            }

        }
        gotoxy(1,26);
    }

void collision(struct bullet player_bullet[],struct object enemy[])
{
    int  ec,bc,ey,ex,bx,by;
    for(ec=1;ec<=75;ec++)
    {
        ex=enemy[ec].x;  //storing coordinate for easy view
        ey=enemy[ec].y;

        for(bc=1;bc <= player_max_bullet;bc++)   //for checking each enmy with each bullet
        {
            bx=player_bullet[bc].x;
            by=player_bullet[bc].y;

            if(ex==bx && ey==by && player_bullet[bc].dead == 0 && enemy[ec].life==1)
            {
                player_bullet[bc].dead=1;
                enemy[ec].life=0;
            }
        }
    }
}
