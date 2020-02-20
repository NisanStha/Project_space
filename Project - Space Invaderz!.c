#include<stdio.h>
#include<conio.h>
#include <Windows.h>

    char difficulty[10]="Medium";
    long frame=1;                   //current game loop
    int bullet_shot=0;              //total bullets fired in game
    int score=0;                    //for every enemy score = +10
    int moving_frame=10;            //No. of frames after which enemy will move
    int gameover=0;                 //if 1 game loop is broken
    int pause=0;                    //if 1 game is paused
    int width=60;                   //width of central game play console
    int height=40;                  //height of game border
    int shoot=0;                    //if 1 checks if player is allowed to shoot
    int player_bullet_used=0;       //no of active bullet on screen
    int player_max_bullet=3;        // maximum no of bullet available at a time
    int dir_player=0;           //direction of movement of player
    int enemy_dir_y=0;          //direction of movement of enemy vertically
    int enemy_dir_x=1;          //direction of movement of enemy horizontally
    int ending=0;               //if 1 skips to end the program

typedef struct bullet
{
    int x,y;                //coordinates
    int last_y;             //last position to be cleared
    int dead;               //if 1 initiates kill/bullet freezing algorithm
    int used_or_not;        //bullet slot used or not
};

typedef struct object
{
    int x,y;                        //coordinates
    float dx;                       //direction of movement
    int life;                       //if 1 alive if 0 dead
    int last_x,last_y;              //last position to be cleared
};

void gotoxy(short x, short y)
{
    COORD pos={x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);

}
void fullscreen()
{
keybd_event(VK_MENU,0x38,0,0);
keybd_event(VK_RETURN,0x1c,0,0);
keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);
keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
}


void init_struct_enemy(struct object enemy[],struct object *phantom_left,struct object *phantom_rig)
{
    int c,i=5,j=3;
    for (c=1;c<=75;c++)
    {
        enemy[c].life=1;
        i+=2;
        enemy[c].x =i+25;
        enemy[c].y =j;
        enemy[c].last_x =enemy[c].x ; //to prevent flicker if not initialized
        enemy[c].last_y =enemy[c].y ;
        if (c%25==0)                    //after every 25 enemies setup x position reset but y increased
        {
            i=5;
            j+=2;
        }
    }
    phantom_left-> x  = enemy[51].x;    //initialize phantom as bottom left and bottom right enemies
    phantom_left-> y  = enemy[51].y;
    phantom_rig -> x  = enemy[75].x;
    phantom_rig -> y  = enemy[75].y;

}

void init_bullet(struct bullet player_bullet[])         //empties every value of all bullets
{
    int i;
    for (i=1;i<10;i++)
    {
        player_bullet[i].x=0;
        player_bullet[i].y=0;
        player_bullet[i].used_or_not=0;
        player_bullet[i].dead=0;
    }
    player_bullet_used=0;                               //empties active bullet count for new game
    bullet_shot=0;                                      //reset no. of bullet shot for new game

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
    //prints styled name.
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

    main_menu();                //calls main menu displaying function

    gotoxy(45,14);
    if(ending==0)               //i.e. if game not quited
        printf("    GET READY!!!   ");
    else
        printf("Quitting Please Wait"); //if game quited
    Sleep(500);                         //delay
    gotoxy(45,14);
    printf("                    ");     //clear message
    Sleep(500);                         //delay

}

void main_menu(){
        char ch;
        int i,j,switch_complete=0;
        for(i=26;i<84;i++)                  //clears certain portion of screen
        {
            for(j=5; j<15; j++)
            {
                gotoxy(i,j);
                printf(" ");
            }
        }
        gotoxy(40,9);                           //display options
        printf("Enter number to do what is listed:");
        gotoxy(40,10);
        printf("1.Play Game");
        gotoxy(40,11);
        printf("2.Set difficulty. (current= %s)",difficulty);
        gotoxy(40,12);
        printf("3.Quit ");

        while(switch_complete!=1)
        {
            ch=getch();
            switch(ch)                  //to do as selected option requires
            {
                case '1':
                    clear_console(); //clears loading screen
                    switch_complete = 1;
                    break;
                case '2':
                    difficulty_select(); //call difficulty select(); to Display difficulty option
                    switch_complete = 1;
                    break;
                case '3':
                    gotoxy(30,13);
                    printf("THANK YOU FOR CHECKING OUT ASCII SPACE INVADERZ!!!");
                    switch_complete = 1;
                    ending=1;
                    break;
         }
        }
}

void difficulty_select()
{
    int i,j,switch_complete=0;
    char ch;
    for(i=26;i<84;i++)                  //clears certain portion of screen
        {
            for(j=5; j<15; j++)
            {
                gotoxy(i,j);
                printf(" ");
            }
        }
        gotoxy(40,9);
        printf("Enter number to do what is listed:");
        gotoxy(40,10);
        printf("1.Easy");
        gotoxy(40,11);
        printf("2.Medium");
        gotoxy(40,12);
        printf("3.Hard");
        gotoxy(40,13);
        printf("4.Impossible");
        gotoxy(40,14);
        printf("5.Baby mode");

    while(switch_complete!=1 )
        {
            ch=getch();
            switch(ch)
            {
                case '1':
                    moving_frame=16;            //so that enemies move every 16 frames
                    player_max_bullet=5;        //limit max bullet allowed
                    strcpy(difficulty,"Easy");  //copy difficulty status to show in main menu
                    switch_complete = 1;
                    break;
                case '2':
                    moving_frame=10;                //so that enemies move every 16 frames
                    player_max_bullet=4;            //limit max bullet allowed
                    strcpy(difficulty,"Medium");    //copy difficulty status to show in main menu
                    switch_complete = 1;
                    break;
                case '3':
                    moving_frame=6;                 //so that enemies move every 16 frames
                    player_max_bullet=3;            //limit max bullet allowed
                    strcpy(difficulty,"Hard");      //copy difficulty status to show in main menu
                    switch_complete = 1;
                    break;
                case '4':
                    moving_frame=3;                     //so that enemies move every 16 frames
                    player_max_bullet=2;                //limit max bullet allowed
                    strcpy(difficulty,"Impossible!");   //copy difficulty status to show in main menu
                    switch_complete = 1;
                    break;
                case '5':
                    moving_frame=26;                    //so that enemies move every 16 frames
                    strcpy(difficulty,"Baby mode");     //limit max bullet allowed
                    player_max_bullet=8;                //copy difficulty status to show in main menu
                    switch_complete = 1;
                    break;
                case '9':                           //so that enemies move every 16 frames
                    moving_frame=2;                 //limit max bullet allowed
                    strcpy(difficulty,"debug");     //copy difficulty status to show in main menu
                    player_max_bullet=8;
                    switch_complete = 1;
                    break;
                default:
                    break;

            }
        }
    main_menu(); //recalls main_menu(); as if it were a recursion to menu screen
}

void clear_console()                //clears certain portion of screen
{
    int i,j;
    for(i=26;i<84;i++)              //clears certain portion of screen
        {
            for(j=2; j<35 ; j++)
            {
                gotoxy(i,j);
                printf(" ");
            }
        }
}

void enemy_move_compute(struct object enemy[],struct object *phantom_left,struct object *phantom_rig) //compute new position of enemy
{
    int i;
        if (phantom_left-> x <= (25+1))     //checking left side border collision
        {
            enemy_dir_y = 1;                //vertical velocity 1
            enemy_dir_x = 1;                //change horizontal velocity
        }
        if (phantom_rig-> x >= (25+width-2))    //checking right side border collision
        {
            enemy_dir_y     = 1;            //vertical velocity 1
            enemy_dir_x     = -1;           //change horizontal velocity
        }

    for (i=1;i<=75;i++)
    {
        enemy[i].last_x=enemy[i].x; //store last position before updating
        enemy[i].last_y=enemy[i].y; //store last position before updating
        enemy[i].x += enemy_dir_x;  //update x position
        enemy[i].y += enemy_dir_y;  //update x position
    }
    phantom_left->x +=enemy_dir_x;  //update position of phantom object
    phantom_left->y += enemy_dir_y;
    phantom_rig->x  +=enemy_dir_x;
    phantom_rig->y += enemy_dir_y;

    enemy_dir_y=0;                  //reset y direction velocity

    if (phantom_rig->y == height-1 )    //if any enemy reaches bottom
        gameover=1;
}

void input()
{
    if(_kbhit())                    //if user presses stuff then only getch() activates and hence takes input
    {
        switch(getch())
        {
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

void collision(struct bullet player_bullet[],struct object enemy[])
{
    int  ec,bc,ey,ex,bx,by;
    for(ec=1;ec<=75;ec++)
    {
        ex=enemy[ec].x;                         //storing coordinate for easy view
        ey=enemy[ec].y;                         //storing coordinate for easy view

        for(bc=1;bc <= player_max_bullet;bc++)  //for checking each enemy with each bullet
        {
            bx=player_bullet[bc].x;             //storing coordinate for easy view
            by=player_bullet[bc].y;             //storing coordinate for easy view

            if(ex==bx && ey==by && player_bullet[bc].dead == 0 && enemy[ec].life==1)
            {
                score+=10;                  //basic score
                enemy[ec].life=0;           //kill enemy
                player_bullet[bc].dead=1;                   //initialize bullet freeing algorithm in draw_bullet
                gotoxy(player_bullet[bc].x,player_bullet[bc].y);
                printf("x");                                //replace last position of bullet with collision indicator

            }
        }
    }
}

void draw_player(struct object *player)
{
        gotoxy(player->last_x,player->y);   //clears last position
        printf(" ");
        gotoxy(player->x,player->y);        //prints player in new position
        printf("%c",142);
        gotoxy(1,26);
}

void draw_struct_enemy(struct object enemy[])       //to draw enemy if alive and clear last position of enemy
{
    int i;
    for (i=1;i<=75;i++)
    {
        if(enemy[i].y<height)                   //unless enemy is outside border
            {
                gotoxy(enemy[i].last_x,enemy[i].last_y);    //goto last position and clear it
                printf(" ",157);
            }

        if (enemy[i].life != 0)                 //i.e. if enemy is not dead
        {
            gotoxy(enemy[i].x,enemy[i].y);      //goto current position and print enemy
            printf("%c",157);
        }
    }
}

void draw_player_bullet(struct bullet player_bullet[])
    {
        int i;
        for(i=1;i<=player_max_bullet;i++)
        {
            if (player_bullet[i].used_or_not==1) //i.e.if bullet active
            {
                player_bullet[i].last_y=player_bullet[i].y; //store current line of bullet in temp
                player_bullet[i].y -= 1;  //update bullet one step up

                gotoxy(player_bullet[i].x,player_bullet[i].last_y); //clear last position of bullet
                printf(" ");
                gotoxy(player_bullet[i].x,player_bullet[i].y);      //print new position
                printf("%c",140);

                if ( player_bullet[i].y <= 2)
                    player_bullet[i].dead=1;                //out of bound check
            }

            //if dead remove||free bullet
            if (player_bullet[i].dead==1)
            {
                gotoxy(player_bullet[i].x,player_bullet[i].y); //clear last position of bullet
                printf(" ");
                player_bullet[i].y=26;          //prevent unintended false positive collision
                player_bullet[i].used_or_not=0; //reset used status
                player_bullet[i].dead=0;        //reset freeing algorithm
                player_bullet_used--;           //update number of active bullet

            }

        }
    }

void phantom_update(struct object enemy[],struct object *phantom_left,struct object *phantom_rig)
{
    int bottom_alive=0;
    int mid_alive=0;
    int i;
    for (i=26;i<=50;i++)
    {
        if(enemy[i].life!=0)
            mid_alive++;        //at end is 0 only if all mid row enemies are dead
    }
    for (i=51;i<=75;i++)
    {
        if(enemy[i].life!=0)
            bottom_alive++;     //at end is 0 only if all bottom row enemy are dead
    }
    if(bottom_alive==0)
        {
            phantom_left -> y = enemy[26].y;    //if all bottom dead change phantom to mid row
            phantom_rig  -> y = enemy[50].y;
            if(mid_alive==0)
            {
                phantom_left -> y = enemy[1].y;     //if mid also dead change phantom to top row
                phantom_rig  -> y = enemy[25].y;
            }
        }
}

void shoot_check(struct bullet player_bullet[],struct object *player)   //to check if bullet can be shot
{
    int i;
    if (shoot==1 && player_bullet_used < player_max_bullet) //to ensure limited bullet
    {
        player_bullet_used++;                           //increases counter which tracks no of active bullets
        for(i=1;player_bullet[i].used_or_not==1;i++);   //finds empty bullet slot at i

        if (player_bullet[i].used_or_not != 1)          //to ensure bullet is empty at i found
        player_bullet[i].used_or_not=1;                 //changes empty bullet slot status to used

        player_bullet_release(player_bullet,player,i); //Sets position & direction of fired bullet  and i= empty bullet index

        gotoxy(2,3);
        printf("Shots fired: %d ",++bullet_shot);       //display no of bullets successfully fired in game and increase it
    }
}

 void player_bullet_release(struct bullet player_bullet[],struct object *player, int i) //initialize bullet after checking in shoot_check()
    {
        int j;
        player_bullet[i].x = player->x;
        player_bullet[i].y = (player->y-1);     //one step above player
    }


void main()
{
    int i,j;
    struct object player;       //player object
    struct object enemy[76];    //enemies (76 but 75 used)
    struct object phantom_left; //non printing object for game over and direction check
    struct object phantom_rig;  //non printing object for game over and direction check
    struct bullet player_bullet[100]; //player bullet (100 defined to avoid a struct array overflow error
    char replay;
    fullscreen();
    start:
    init_struct_enemy(enemy,&phantom_left,&phantom_rig);    //initializes position of all enemies
    init_bullet(player_bullet);                             //empties bullet properties
    player.x=40;            // player position initialization
    player.y=height-1;      // player position initialization
    score = 0;              //reset score
    frame = 1;              //reset frame count
    pause = 0;              //reset pause status
    draw_init_splash();     // Border and get ready
    if (ending==1)          // 3 pressed on main menu
        goto end;
    gotoxy(87,2);
    printf("Press P to Pause");
    gotoxy(87,3);
    printf("Press X to End Game!!!");
    gotoxy(87,4);
    printf("Press A to move Left");
    gotoxy(87,5);
    printf("Press D to Move Right");
    gotoxy(87,6);
    printf("Press S to Shoot");
    fflush(stdin);          //clear input buffer
    while(!gameover && score < 750)
        {
            if (frame % moving_frame==0)     //so that enemy moves every (moving_frame) frames
                enemy_move_compute(enemy,&phantom_left,&phantom_rig);   //changes position of enemy and phantom object

            input();            //take input
            fflush(stdin);      //clear input buffer

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
            collision(player_bullet,enemy);     //Check for collision and kill bullet and enemy
            draw_player(&player);               //update player position
            draw_struct_enemy(enemy);           //draws all enemies
            draw_player_bullet(player_bullet);  //draws player bullet
            phantom_update(enemy,&phantom_left,&phantom_rig);

            if (score>650)
                moving_frame = 3;               //last 10 enemy move with very high speed.

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

            //shooting algorithm
            shoot_check(player_bullet, &player);//compute shoot status of player
            shoot=0;                    //resets shooting status for player

            gotoxy(2,2);
            printf("Frame: %d | Score:%d", ++frame,score);

            Sleep(25); //frame rate control
        }
    gotoxy(26,4);
    printf("                                                          ");
    gotoxy(26,5);
    if(score==750)
    {
        printf("----------------------Congrats You WON!!!-----------------");
    }
    else
    {
        printf("-------------------------GAME OVER!!!---------------------");
    }
        gotoxy(26,6);
        printf("------------------------SCORE IS: %3d---------------------",score);
    gotoxy(26,7);
    printf("                                                          ");
    gotoxy(40,8);
    printf("Press any key to continue");
    gameover=0;
    getch();
    Sleep(300);
    goto start;

    end:
    gotoxy(1,height+1);
}
