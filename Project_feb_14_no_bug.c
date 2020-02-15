#include<stdio.h>
#include<conio.h>
#include <Windows.h>

void gotoxy(short x, short y)
{
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}
void fullscreen()
{
keybd_event(VK_MENU,0x38,0,0);
keybd_event(VK_RETURN,0x1c,0,0);
keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
}
    long frame=0;
    int gameover=0;
    int pause=0;
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
    int moving_frame=10;
    char difficulty[10]="Medium";
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
    player.x=40;            // player position initialization
    player.y=height-1;      // player position initialization
    fullscreen();
    init_struct_enemy(enemy,&phantom_left,&phantom_rig);    //initializes position of all enemies


    draw_init_splash();                                     // Border and get ready
    fflush(stdin);          //clear input buffer
    while(!gameover)
        {
            gotoxy(0,0);        //to stabilize output screen
            if (frame%moving_frame==0)     //so that enemy moves every (frame_check) frames
                enemy_move_compute(enemy,&phantom_left,&phantom_rig);   //changes position of enemy and phantom object

            input();            //take input

            if (pause==1)
            {
                gotoxy(27,27);
                printf("Press any key to continue.");
                pause=0;
                getch();
                gotoxy(27,27);
                printf("                           ");
                                        //to pause until user presses enter.
            }
            fflush(stdin);                      //clear input buffer
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

        gotoxy(30,16);
        printf("            SSSSS PPPPP   A   CCCCC EEEEE\n");
        gotoxy(30,17);
        printf("            S     P   P  A A  C     E    \n");
        gotoxy(30,18);
        printf("            SSSSS PPPPP A   A C     EEEEE\n");
        gotoxy(30,19);
        printf("                S P     AAAAA C     E    \n");
        gotoxy(30,20);
        printf("            SSSSS P     A   A CCCCC EEEEE\n");
        gotoxy(30,21);
        printf("\n");
        gotoxy(30,22);
        printf("IIIII N   N V   V    A   DDDD  EEEEE RRRR  ZZZZZ  !!\n");
        gotoxy(30,23);
        printf("  I   NN  N V   V   A A  D   D E     R  RR    Zz  !!\n");
        gotoxy(30,24);
        printf("  I   N N N V   V  A   A D   D EEEEE RRRR    Z    !!\n");
        gotoxy(30,25);
        printf("  I   N  NN  V V   AAAAA D   D E     R  R  zZ       \n");
        gotoxy(30,26);
        printf("IIIII N   N   V    A   A DDDD  EEEEE R   R ZZZZZ  !!\n");

    main_menu();
  for(i=0;i<5;i++)                //Splash screen "get ready!".
    {
        gotoxy(55,25);
        printf("GET READY!!!");
        Sleep(50);
        gotoxy(55,25);
        printf("            ");
        Sleep(50);
    }

        //gotoxy(1,26);
}
void main_menu(){
        char ch;
        int i,j,switch_complete;
        for(i=26;i<84;i++)
        {
            for(j=5; j<15; j++)
            {
                gotoxy(i,j);
                printf(" ");
            }
        }
        gotoxy(40,9);
        printf("Enter number to do what is listed:");
        gotoxy(49,10);
        printf("1.Play Game");
        gotoxy(49,11);
        printf("2.Set difficulty. (current= %s)",difficulty);
        gotoxy(49,12);
        printf("3.Quit ");

        while(switch_complete!=1)
        {
            ch=getch();
            switch(ch)
            {
                case '1':
                    clear_console(); //clears loading screen
                    switch_complete = 1;
                    break;
                case '2':
                    ch = difficulty_select();
                    switch_complete = 1;
                    break;
                case '3':
                    gotoxy(26,12);
                    printf("THNK YOU FOR CHECKING OUT ASCII SPACE INVADERZ!!!");
                    switch_complete = 1;
                    gameover=1;
                    break;

            }
        }
}

void difficulty_select()
{   int i,j,switch_complete;
    char ch;
    for(i=26;i<84;i++)
        {
            for(j=5; j<15; j++)
            {
                gotoxy(i,j);
                printf(" ");
            }
        }
        gotoxy(50,10);
        printf("1.Easy");
        gotoxy(50,11);
        printf("2.Medium");
        gotoxy(50,12);
        printf("3.Hard");
    while(switch_complete!=1 )
        {
            ch=getch();
            switch(ch)
            {
                case '1':
                    moving_frame=15;
                    strcpy(difficulty,"Easy");
                    switch_complete = 1;
                    break; //continue without doing anything
                case '2':
                    moving_frame=15;
                    strcpy(difficulty,"Medium");
                    switch_complete = 1;
                    break;
                case '3':
                    moving_frame=15;
                    strcpy(difficulty,"Hard");
                    switch_complete = 1;
                    break;
                default:
                    break;

            }
        }
    main_menu(); //returns to menu screen
}

void clear_console()
{
    int i,j;
    for(i=26;i<84;i++)
        {
            for(j=2; j<height-1 ; j++)
            {
                gotoxy(i,j);
                printf(" ");
            }
        }
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

void enemy_move_compute(struct object enemy[],struct object *phantom_left,struct object *phantom_rig)
{
    int i;
    phantom_left->x +=enemy_dir_x;
    phantom_rig->x  +=enemy_dir_x;
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
            case 'p':
                pause = 1;
                break;
            case 'x':
                gameover=1;
                break;

            default:
                shoot      = 0;
                dir_player = 0;
                break;
        }
    }

}


 void player_bullet_release(struct bullet player_bullet[],struct object *player, int i)
    {
        int j;
        player_bullet[i].x = player->x;
        player_bullet[i].y = (player->y-1);
    }

 void draw_player_bullet(struct bullet player_bullet[])
    {
        int i,cursor_debug=1;
        for(i=1;i<=player_max_bullet;i++)
        {
            if (player_bullet[i].used_or_not==1)
            {
                player_bullet[i].last_y=player_bullet[i].y; //store current line of bullet in temp
                player_bullet[i].y -= 1;  //update bullet one step up

                gotoxy(player_bullet[i].x,player_bullet[i].last_y); //clear last position of bullet
                printf(" ");
                gotoxy(player_bullet[i].x,player_bullet[i].y);      //print new positon
                printf("1");

                if ( player_bullet[i].y <= 2)
                    player_bullet[i].dead=1;
            }

            //if dead remove||free bullet
            if (player_bullet[i].dead==1)
            {
                gotoxy(player_bullet[i].x,player_bullet[i].y); //clear last position of bullet
                printf(" ");
                player_bullet[i].used_or_not=0; //reset used status
                player_bullet[i].dead=0;        //reset freeing algorithm
                player_bullet_used--;           //update number of active bullet

            }

        }
        gotoxy(1,26);
    }

void collision(struct bullet player_bullet[],struct object enemy[])
{
    int  ec,bc,ey,ex,bx,by;
    for(ec=1;ec<=75;ec++)
    {
        ex=enemy[ec].x;     //storing coordinate for easy view
        ey=enemy[ec].y;     //storing coordinate for easy view

        for(bc=1;bc <= player_max_bullet;bc++)   //for checking each enemy with each bullet
        {
            bx=player_bullet[bc].x;     //storing coordinate for easy view
            by=player_bullet[bc].y;     //storing coordinate for easy view

            if(ex==bx && ey==by && player_bullet[bc].dead == 0 && enemy[ec].life==1)
            {   score+=10;                  //basic score
                enemy[ec].life=0;           //empty bullet
                player_bullet[bc].dead=1;   //initialize bullet freeing algorithm in draw_bullet
                gotoxy(player_bullet[bc].x,player_bullet[bc].y);
                printf("x");                //replace last position of bullet with collision indicator
            }
        }
    }
}
