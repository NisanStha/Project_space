#include<stdio.h>
#include<conio.h>
#include <Windows.h>

void gotoxy(short x, short y)
{
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}
    long frame=0;
    int gameover=0;
    int width=60;
    int height=40;
    int shoot=0;
    int bullet_shot=0;
    int score=0;
    int player_bullet_used=0;
    int player_max_bullet=3;
    int dir_player=0;           //direction of movement
    int enemy_dir_y=0;          //direction of movement
    int enemy_dir_x=1;          //direction of movement

typedef struct bullet
{
    int x,y;                //coordinates
    int last_y;             //last position to be cleared
    int dead;               //if 1 initiates kill status
    int used_or_not;        //bullet slot used or not
};

typedef struct object
{
    int x,y;                        //coordinates
    float dx;                       //direction of movement
    int life;                       //if 1 alive if 0 dead
    int last_x,last_y;              //last position to be cleared
};


void main()
{
    int i,j;
    struct object player;       //player object
    struct object enemy[76];    //enemies (76 but 75 used)
    struct object phantom_left; //non printing object for game over and direction check
    struct object phantom_rig;  //non printing object for game over and direction check
    struct bullet player_bullet[100]; //player bullet (100 defined to avoid a struct array overflow error

    init_struct_enemy(enemy,&phantom_left,&phantom_rig);    //initializes position of all enemies
    draw_init_splash();                                     // Border and get ready

    player.x=40;             // player position initialization
    player.y=height-1;             // player position initialization

    /*for(i=1 ; i <= player_max_bullet ; i++)
    {
        player_bullet[i].used_or_not=0;            //empties all player bullet slot
        player_bullet[i].x=0;
        player_bullet[i].y=0;
        player_bullet[i].dead=0;
    }*/

    while(!gameover)
        {
            gotoxy(1,5);
            printf("x= %d",player.x);
            if (frame%5==0)            //so that enemy moves every (frame_check) frames
                enemy_move_compute(enemy,&phantom_left,&phantom_rig);   //changes position of enemy and phantom object

            input();                            //take input
            collision(player_bullet,enemy);     //Check for collision and kill bullet and enemy
            draw_player(&player);               //update player position
            draw_struct_enemy(enemy);           //draws all enemies
            draw_player_bullet(player_bullet);  //draws player bullet

            //compute position of player
            if(player.x>26 || dir_player != -1)
            {
                if(player.x < (25+ width - 2) || dir_player!=1 )
                {
                    player.last_x = player.x;   //save last position clean screen at the point ; refer to void draw_player();
                    player.x += dir_player;     //update to new position according to input(); from user
                    dir_player=0;               //reset move direction
                }
            }
            shoot_check(player_bullet, &player);//compute shoot status of player
            shoot=0;                    //resets shooting status for player
            /* //debugger
        gotoxy(2,3);
        for(j=1 ; j <= player_max_bullet ; j++)
            printf("bullet status:%d=%d",j,player_bullet[j].used_or_not);
*/

            gotoxy(3,2);
            printf("frame: %d Score:%d", ++frame,score);
            Sleep(5); //frame rate control
        }

    if(score==750) printf("Congrats You won");
    else printf("Too bad.... try again");

}

void shoot_check(struct bullet player_bullet[],struct object *player)
{
    int i;
    if (shoot==1 && player_bullet_used < player_max_bullet) //to ensure limited bullet
    {
        player_bullet_used++;                           //increases counter which tracks no of active bullets
        for(i=1;player_bullet[i].used_or_not==1;i++);   //finds empty bullet slot at i

        if (player_bullet[i].used_or_not != 1)          //to ensure bullet is empty at i found
        player_bullet[i].used_or_not=1;                 //changes empty bullet slot status to used

        player_bullet_release(player_bullet,player,i); //Sets position & direction of fired bullet  and i= bullet index

        //display no of bullets successfully fired in game
        gotoxy(2,2);
        printf("Shots fired: %d ",++bullet_shot);
    }
}

void init_struct_enemy(struct object enemy[],struct object *phantom_left,struct object *phantom_rig)
{
    int c,i=5,j=2;
    for (c=1;c<=75;c++)
    {
        enemy[c].life=1;
        i+=2;
        enemy[c].x =i+25;
        enemy[c].y =j;
        if (c%25==0)
        {
            i=5;
            j+=2;
        }

    }

    phantom_left-> x  = 5+25;
    phantom_rig -> x  = 55+25;
    phantom_left-> y  = 7;
    phantom_rig -> y  = 7;

}

void draw_struct_enemy(struct object enemy[])
{
    int i;
    for (i=1;i<=75;i++)
    {
            gotoxy(enemy[i].last_x,enemy[i].last_y);
            printf(" ",157);
        if (enemy[i].life != 0)
        {
            gotoxy(enemy[i].x,enemy[i].y);
            printf("%c",157);
        }
    }
}

void draw_init_splash()
{
    int i,j;
    system("cls");

    //prints top border
    gotoxy(1,1);           //goto start of border
    for (i=1;i<=width+25+25;i++)
    {
        if(i==1 ) printf("%c",201);
        else if(i==(width+50)) printf("%c",187);
        else if(i==25 || i==25+width-1 ) printf("%c",203);
        else printf("%c",205);
    }

    //prints vertical border,
    for(j=2;j<=height-1;j++)
    {
        gotoxy(1,j);           //goto  start of each line
        for(i=1; i <= (width+25+25) ; i++)
        {
            if(i==1 || i==25 || (i==25+width-1) || i==(width+50))
                printf("%c",186);
            else
                printf(" ");
        }
    printf("\n");

    }

     //lower border
    gotoxy(1,height);      //goto last line

    for (i=1;i<=(width+25+25);i++)
    {
        if(i==1 ) printf("%c",200);
        else if(i==(width+25+25)) printf("%c",188);
        else if(i==25 || i==25+width-1 ) printf("%c",202);
        else printf("%c",205);
        }

  for(i=0;i<5;i++)                //Splash screen "get ready!".
    {
        gotoxy(30,10);
        printf("GET READY!!!");
        Sleep(500);
        gotoxy(30,10);
        printf("            ");
        Sleep(50);
    }

        gotoxy(1,26);
}
void enemy_move_compute(struct object enemy[],struct object *phantom_left,struct object *phantom_rig)
{
    int i;
    phantom_left->x +=enemy_dir_x;
    phantom_rig->x  +=enemy_dir_x;
            gotoxy(2,27);
            printf("phantom l x :%d",phantom_left->x);
            printf("phantom r x :%d",phantom_rig->x);

        if (phantom_left-> x < (25+1))
        {   gotoxy(1,26);
            //printf("phantom l x :%d",phantom_left->x);
            enemy_dir_y = 1;
            enemy_dir_x = 1;
            phantom_rig->y += 1;
        }
        if (phantom_rig-> x > (25+width-2))
        {
            enemy_dir_y     = 1;
            enemy_dir_x     = -1;
            phantom_rig->y += 1;
        }

    for (i=0;i<=75;i++)
    {   enemy[i].last_x=enemy[i].x;
        enemy[i].last_y=enemy[i].y;
        enemy[i].x += enemy_dir_x;
        enemy[i].y += enemy_dir_y;
    }

    enemy_dir_y=0;

    if (phantom_rig->y == height )
        gameover=1;
}

void draw_player(struct object *player)
{

        gotoxy(player->last_x,player->y);
        printf(" ");
        gotoxy(player->x,player->y);
        printf("%c",142);
    //
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
            {   score+=10;
                player_bullet[bc].dead=1;
                enemy[ec].life=0;
                gotoxy(player_bullet[bc].x,player_bullet[bc].y); //clear last position of bullet
                printf("x");
            }
        }
    }
}