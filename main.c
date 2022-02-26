#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define FRAME_SIZE 64 //Adjust Max Frame size
#define CHARACTER 1 //Define character symbol
#define FRAME_GRID_SIZE (FRAME_SIZE*(FRAME_SIZE+1))

void Note();//prints the note on the start of program
char Menu();//displays the menu to select option
void SelectMap(); //select map among available
void Get_Map(unsigned char frame[FRAME_GRID_SIZE], int mapatt[7], int food[10][2], int MapNum);//Print Frame on Console
void MapsCount(int ME[100]); //count amount of map available
void Play(unsigned char frame[FRAME_GRID_SIZE], int mapatt[7], int food[10][2], int MapNum); //play game after selecting map
void EndScore(int FCount, int time, int idealT, int MapNum); //display and write the resulting score to datafile after playing game
void MapEditor();//Edit or Create Map for game
void EditMap(unsigned char frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int mapnum);//Edit map
void display(unsigned char frame[FRAME_SIZE][FRAME_SIZE], int element[FRAME_SIZE][FRAME_SIZE], int cursor[2], int ys, int xs);//Display Frame for Create map
void save(unsigned char frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int mapnum, int ys, int xs);//save created map to a new file
void Scoreboard(); //read and display scores of alltime
void About();//Display About info of Game
void Exit();//prompt to exit
void timedelay(int second);//delay time for specific seconds
void cls();//delay time for specific seconds

int main()
{
    Note();
    do
    {
        switch(Menu())
        {
        case '1':
            SelectMap();
            break;
        case '2':
            MapEditor();
            break;
        case '3':
            Scoreboard();
            break;
        case '4':
            About();
            break;
        case '5':
        case 27:
            Exit();
            break;
        }
    }
    while(1);
    return 0;
}
//Display a Note on the Start of Program
void Note()
{
    printf("\n\t\tNOTE!\n");
    printf("\tThe Game have some keys to control it.\n");
    printf("\tIn Menu you need to press the Menu No. you want to select.\n");
    printf("\tIn Select Map press 'S' to change map,'W' to select map or 'ESC' to exit back to MAIN MENU.\n");
    printf("\tIn Gameplay press 'W' to move UP, 'S' to move DOWN, 'A' to move LEFT, 'D' to move RIGHT, and 'ESC' to exit back to MAIN MENU.\n");
    printf("\tThe Game Print Scores on the end of play session, and record it.\n");
    printf("\tYou can create a new map or edit an existing map.\n");
    printf("\tIn MapEditor use WSAD for movement. use '1' for white frame, '2' for dark frame, '3' for food, '4' for specify start position,\n");
    printf("\t-'5' for specify end position, '6' for nullify all elements positions, '9' to save the map to file, '0' to stop operation.\n");
    printf("\tYou can view your all played game score in scoreboard.\n");
    printf("\n\tPress Any Key after 5sec to Continue...");
    timedelay(5);//time delay
    getch();//pause
    cls();
}
//Display the game menu and return selected option
char Menu()
{
    //menu printing
    printf("\t    MAZE GAME\n\n");
    printf("\t1.\t SelectMap\n");
    printf("\t2.\t MapEditor\n");
    printf("\t3.\tScoreBoard\n");
    printf("\t4.\t  About\n");
    printf("\t5.\t  Exit\n");
    printf("\nSelect Option.");
    char opt;
    //selecting option
    do
    {
        opt = getch();
    }
    while((opt < '1' || opt > '5') && opt != 27);
    cls();//clear screen
    return opt;
}
//Select map from available
void SelectMap()
{
    unsigned char Frame[FRAME_GRID_SIZE];
    int mapattribute[7] = {0};//{y_range, x_range, Start_y, Start_x, End_y, End_x, idealTime}
    int Food[10][2] = {0};
    int MapNo = 1;
    char key;
    int MapExist[100] = {0};
    MapsCount(MapExist);
    //selecting map
    do
    {
        printf("\tSELECT MAP\n");
        printf("\tMap No. %2d\n", MapNo);
        printf("Press S for Next Map, Press W to Select and Press ESC get Back to Menu\n\n");
        Get_Map(Frame, mapattribute, Food, MapNo);
        puts(Frame);
        printf("\n");
        //getting valid key
        do
        {
            key = getch();
        }
        while(key != 27 && key != 'W' && key != 'S' && key != 'w' && key != 's');
        switch(key)
        {
        case 27:
            cls();
            return;
        case 's':
        case 'S':
            while((MapExist[++MapNo] != 1) && (MapNo < 100));
            if (MapNo == 100)
                MapNo = 1;
            cls();
        }
    }
    while(key != 'w' && key != 'W');
    cls();
    Play(Frame, mapattribute, Food, MapNo);//calling play game
}
//Get Map data from file
void Get_Map(unsigned char frame[FRAME_GRID_SIZE], int mapatt[7], int food[10][2], int MapNum)
{
    int i, j;
    char MFileN[10] = {'m', 'a', 'p', MapNum/10 + '0', MapNum%10 + '0', '.', 'd', 'a', 't', 0};
    char checkch;
    FILE *mapdata;
    mapdata = fopen(MFileN, "r");
    //reading map attributes
    fscanf(mapdata, "GridSize : (%2d,%2d)\n", &mapatt[0], &mapatt[1]);//reading grid size
    fscanf(mapdata, "StartPos : (%2d,%2d)\n", &mapatt[2], &mapatt[3]);//reading start position
    fscanf(mapdata, "EndPos : (%2d,%2d)\n", &mapatt[4], &mapatt[5]);//reading end position
    fscanf(mapdata, "IdealTime : %3d\n", &mapatt[6]);//reading ideal time
    //reading food position
    for(i = 0; i < 10; i++)
        fscanf(mapdata, "(%2d,%2d)", &food[i][0], &food[i][1]);
    fgetc(mapdata);
    //reading map frame data
    for(i = 0; i < mapatt[0]; i++)
    {
        for(j = 0; j < mapatt[1]; j++)
        {
            checkch = fgetc(mapdata);
            if(checkch == '1')
                frame[i*(mapatt[1]+1)+j] = 178;
            else if(checkch == '0')
                frame[i*(mapatt[1]+1)+j] = 176;
        }
        frame[i*(mapatt[1]+1)+j] = '\n';
    }
    frame[mapatt[0]*(mapatt[1]+1)-1] = '\0';
    fclose(mapdata);
}
//Count Available maps form map1.dat to map9.dat
void MapsCount(int ME[100])
{
    FILE *mapcount;
    char filename[10] = {'m', 'a', 'p', '0', '0', '.', 'd', 'a', 't', '\0'};
    for(int i = 1; i < 100; i++)
    {
        filename[3] = i/10 + '0';
        filename[4] = i%10 + '0';
        //checking map if exist
        if((mapcount = fopen(filename, "r")) != NULL)
        {
            ME[i] = 1;
            fclose(mapcount);
        }
    }
    fclose(mapcount);
    return;
}
//Play Game
void Play(unsigned char frame[FRAME_GRID_SIZE], int mapatt[7], int food[10][2], int MapNum)
{
    int y_pos, x_pos, c_pos = -1;//c_pos combine position
    int foodcount = 0;
    int start_T = time(NULL);
    int total_T;
    //initialing play
    frame[mapatt[2]*(mapatt[1]+1)+mapatt[3]] = CHARACTER; //start character
    frame[mapatt[4]*(mapatt[1]+1)+mapatt[5]] = 20; //end point
    for(int i = 0; i < 10; i++)
        frame[food[i][0]*(mapatt[1]+1)+food[i][1]] = 4;
    y_pos = mapatt[2];
    x_pos = mapatt[3];
    //play loop
    do
    {
        //printing frame if position changes
        if(c_pos != y_pos * mapatt[1] + x_pos)
        {
            cls();
            puts(frame);
            printf("Pos   : %2d,%2d\n", y_pos, x_pos);
            printf("Score : %-4d", foodcount * 100);
        }
        c_pos = y_pos * mapatt[1] + x_pos;//combine position
        //Control Movement of the character
        switch(getch())
        {
        case 'w'://character up
        case 'W':
            if((y_pos-1 >= 0) && (frame[(y_pos-1)*(mapatt[1]+1)+x_pos] != 178 ))
            {
                if(frame[(y_pos-1)*(mapatt[1]+1)+x_pos] == 4)
                    foodcount++;
                frame[y_pos*(mapatt[1]+1)+x_pos] = 176;
                frame[(--y_pos)*(mapatt[1]+1)+x_pos] = CHARACTER;
            }
            break;
        case 's'://character down
        case 'S':
            if((y_pos+1 <= mapatt[0]-1) && (frame[(y_pos+1)*(mapatt[1]+1)+x_pos] != 178))
            {
                if(frame[(y_pos+1)*(mapatt[1]+1)+x_pos] == 4)
                    foodcount++;
                frame[y_pos*(mapatt[1]+1)+x_pos] = 176;
                frame[(++y_pos)*(mapatt[1]+1)+x_pos] = CHARACTER;
            }
            break;
        case 'a'://character left
        case 'A':
            if((x_pos-1 >= 0) && (frame[y_pos*(mapatt[1]+1)+(x_pos-1)] != 178))
            {
                if(frame[y_pos*(mapatt[1]+1)+(x_pos-1)] == 4)
                    foodcount++;
                frame[y_pos*(mapatt[1]+1)+x_pos] = 176;
                frame[y_pos*(mapatt[1]+1)+(--x_pos)] = CHARACTER;
            }
            break;
        case 'd'://character right
        case 'D':
            if((x_pos+1 <= mapatt[1]-1) && (frame[y_pos*(mapatt[1]+1)+(x_pos+1)] != 178))
            {
                if(frame[y_pos*(mapatt[1]+1)+(x_pos+1)] == 4)
                    foodcount++;
                frame[y_pos*(mapatt[1]+1)+x_pos] = 176;
                frame[y_pos*(mapatt[1]+1)+(++x_pos)] = CHARACTER;
            }
            break;
        case 27://exit function
            cls();
            return;
        }
    }
    while((y_pos != mapatt[4]) || (x_pos != mapatt[5]));
    cls();
    total_T = time(NULL) - start_T;
    EndScore(foodcount, total_T, mapatt[6], MapNum);//calling endscore
}
//Display End Score and write them to scoreboard
void EndScore(int FCount, int time, int idealT, int MapNum)
{
    int Win_R = 1000;
    int Food_R = FCount * 100;
    int Time_R = (((idealT * 1000) / time) / 10) * 10;
    int Score = Win_R + Food_R + Time_R;
    printf("\n\t\tENDSCORE\n");
    printf("\n\t\tMap No. %d\n", MapNum);
    if((time / 60) != 0)
        printf("\n\tTotal Time : %dm%ds\n", time/60, time%60);
    else
        printf("\n\tTotal Time : %ds\n", time);
    printf("\n\tWin Reward     = %-4d", Win_R);
    printf("\n\tFood %2d X 100  = %-4d", FCount, Food_R);
    printf("\n\tTime Reward    = %-4d", Time_R);
    printf("\n\n\tTotal Score    = %-4d\n", Score);
    //writing score to file
    FILE *scoreboard;
    scoreboard = fopen("scoreboard.dat", "a");
    fprintf(scoreboard, "{%2d,%4d,%3d}", MapNum, Score, time);
    fclose(scoreboard);
    printf("\nPress Any Key After 5sec To Continue...");
    timedelay(5);//5 sec delay
    getch();
    cls();
}
//Edit or Create Map for game
void MapEditor()
{
    unsigned char MapFrame[FRAME_SIZE][FRAME_SIZE] = {0};
    unsigned char GetFrame[FRAME_GRID_SIZE] = {0};
    int mapatt[7] = {0};
    int food[10][2] = {0};
    int MapNo;
    int i, j;
    int MapExist[100] = {0};
    char opt;
    char mapfilename[10] = {'m', 'a', 'p', '0', '0', '.', 'd', 'a', 't', '\0'};
    //Menu printing
    printf("\t    MAP EDITOR\n\n");
    printf("\t1.\t CreateMap\n");
    printf("\t2.\t EditMap\n");
    printf("\t3.\t DeleteMap\n");
    printf("\nSelect Option.");
    MapsCount(MapExist);
    //selecting option
    do
    {
        opt = getch();
        switch(opt)
        {
        case '1'://create map
            cls();
            printf("Enter Name (<100): ");
            scanf("%d", &MapNo);
            //check map existence
            if(MapExist[MapNo] != 0)
            {
                printf("Map Already Exist...");
                getch();
            }
            else
            {
                //creating new map
                printf("Enter Grid Size (i.e 64x64)(y,x): ");
                scanf("%2dx%2d", &mapatt[0], &mapatt[1]);
                printf("Enter Ideal Time (in sec)  : ");
                scanf("%3d", &mapatt[6]);
                for(i = 0; i < mapatt[0]; i++)
                    for(j = 0; j < mapatt[1]; j++)
                        MapFrame[i][j] = 176;//assign white to frame
                EditMap(MapFrame, mapatt, food, MapNo);
            }
            break;
        case '2'://edit existing map
            cls();
            printf("Enter Map No. : ");
            scanf("%2d", &MapNo);
            //checking map existence
            if(MapExist[MapNo] != 1)
            {
                printf("Map not Exist...");
                getch();
            }
            else
            {
                //continue with existing map
                Get_Map(GetFrame, mapatt, food, MapNo);
                for(i = 0; i < mapatt[0]; i++)
                    for(j = 0; j < mapatt[1]; j++)
                    MapFrame[i][j] = GetFrame[i*(mapatt[1]+1)+j];
                EditMap(MapFrame, mapatt, food, MapNo);
            }
            break;
        case '3'://Delete Map file
            cls();
            printf("Enter Map No. : ");
            scanf("%2d", &MapNo);
            //checking map existence
            if(MapExist[MapNo] != 1)
            {
                printf("Map not Exist...");
                getch();
            }
            else
            {
                mapfilename[3] = MapNo/10 + '0';
                mapfilename[4] = MapNo%10 + '0';
                remove(mapfilename);// deleting map file
                printf("Map File Deleted...");
                getch();
            }
            break;
        }
    }
    while(opt != '1' && opt != '2' && opt != '3' && opt != 27);
    cls();
    return;
}
//Edit Map
void EditMap(unsigned char frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int mapnum)
{
    int MapElement[FRAME_SIZE][FRAME_SIZE] = {0};
    int Cursor[2] = {0};
    const char ch[] = {111, 176, 178, 4, 1, 20, 0};//cursor = 111, white frame = 176, black frame = 178, food = 4, start = 1, end = 20, NULL.
    int y_pos, x_pos, c_pos = -1;
    char key;
    int operation = 0;
    int f = 0;
    int i, j;
    //initializing editing
    MapElement[mapatt[2]][mapatt[3]] = ch[4];
    MapElement[mapatt[4]][mapatt[5]] = ch[5];
    for(i = 0; i < 10; i++)
        MapElement[food[i][0]][food[i][1]] = ch[3];
    y_pos = x_pos = 0;
    //editing infinite loop
    do
    {
        //printing frame if cursor move
        if(c_pos != y_pos*mapatt[1]+x_pos)
        {
            cls();
            display(frame, MapElement, Cursor, mapatt[0], mapatt[1]);//calling display
        }
        c_pos = y_pos*mapatt[1]+x_pos;
        key = getch();
        switch(key)
        {
        case 'W'://cursor up
        case 'w':
            if((y_pos-1) >= 0)
            {
                y_pos--;
                Cursor[0] = y_pos;
                Cursor[1] = x_pos;
                if(operation == 1)
                    frame[y_pos][x_pos] = ch[1];
                else if(operation == 2)
                    frame[y_pos][x_pos] = ch[2];
            }
            break;
        case 'S'://cursor down
        case 's':
            if((y_pos+1) < mapatt[0])
            {
                y_pos++;
                Cursor[0] = y_pos;
                Cursor[1] = x_pos;
                if(operation == 1)
                    frame[y_pos][x_pos] = ch[1];
                else if(operation == 2)
                    frame[y_pos][x_pos] = ch[2];
            }
            break;
        case 'A'://cursor left
        case 'a':
            if((x_pos-1) >= 0)
            {
                x_pos--;
                Cursor[0] = y_pos;
                Cursor[1] = x_pos;
                if(operation == 1)
                    frame[y_pos][x_pos] = ch[1];
                else if(operation == 2)
                    frame[y_pos][x_pos] = ch[2];
            }
            break;
        case 'D'://cursor right
        case 'd':
            if((x_pos+1) < mapatt[1])
            {
                x_pos++;
                Cursor[0] = y_pos;
                Cursor[1] = x_pos;
                if(operation == 1)
                    frame[y_pos][x_pos] = ch[1];
                else if(operation == 2)
                    frame[y_pos][x_pos] = ch[2];
            }
            break;
        case '0'://no operation
            operation = 0;
            break;
        case '1'://white frame
            if(operation != 1)
            {
                operation = 1;
                frame[y_pos][x_pos] = ch[1];
            }
            else
                operation = 0;
            break;
        case '2'://black frame
            if(operation != 2)
            {
                operation = 2;
                frame[y_pos][x_pos] = ch[2];
            }
            else
                operation = 0;
            break;
        case '3'://food
            if(MapElement[y_pos][x_pos] != ch[3])
            {
                if(f < 10)
                {
                    MapElement[y_pos][x_pos] = ch[3];
                    food[f][0] = y_pos;
                    food[f][1] = x_pos;
                    f++;
                }
            }
            else
            {
                MapElement[y_pos][x_pos] = 0;
            }
            break;
        case '4'://start pos
            if(MapElement[y_pos][x_pos] != ch[4])
            {
                MapElement[y_pos][x_pos] = ch[4];
                mapatt[2] = y_pos;
                mapatt[3] = x_pos;
            }
            else
            {
                MapElement[y_pos][x_pos] = 0;
            }
            break;
        case '5'://end pos
            if(MapElement[y_pos][x_pos] != ch[5])
            {
                MapElement[y_pos][x_pos] = ch[5];
                mapatt[4] = y_pos;
                mapatt[5] = x_pos;
            }
            else
            {
                MapElement[y_pos][x_pos] = 0;
            }
            break;
        case '6'://Zeros all map elements
            for(i = 0; i < FRAME_SIZE; i++)
                for(j = 0; j < FRAME_SIZE; j++)
                    MapElement[i][j] = 0;
            for(i = 0; i < 10; i++)
                for(j = 0; j < 2; j++)
                    food[i][j] = 0;
            for(i = 0; i < 4; i++)
                mapatt[i+2] = 0;
            f = 0;
            c_pos = -1;
            break;
        case '9'://saving map data
            save(frame, mapatt, food, mapnum, mapatt[0], mapatt[1]);
            printf("Saved...");
            break;
        case 27://exiting map editor
            cls();
            return;
        }
    }
    while(1);
    cls();
    return;
}
//Display frame for CreateMap
void display(unsigned char frame[FRAME_SIZE][FRAME_SIZE], int element[FRAME_SIZE][FRAME_SIZE], int cursor[2], int ys, int xs)
{
    int i, j;
    unsigned char frame2[FRAME_GRID_SIZE] = {0};
    for(i = 0; i < ys; i++)
    {
        for(j = 0; j < xs; j++)
        {
            //checking if cursor exist at this position
            if(i != cursor[0] || j != cursor[1])
            {
                //checking if any map element exist at this position
                if((element[i][j] != 4) && (element[i][j] != 1) && (element[i][j] != 20))
                    frame2[i*(xs+1)+j] = frame[i][j];
                else
                    frame2[i*(xs+1)+j] = element[i][j];
            }
            else
                frame2[i*(xs+1)+j] = 111;
        }
        frame2[i*(xs+1)+j] = '\n';
    }
    puts(frame2);
    return;
}
//Svae created map to a new file
void save(unsigned char frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int mapnum, int ys, int xs)
{
    int i, j;
    FILE *savefile;
    char name[10] = {'m', 'a', 'p', mapnum/10 +'0', mapnum%10 +'0', '.', 'd', 'a', 't', '\0'};
    savefile = fopen(name, "w");
    //recrding map attributes to map data
    fprintf(savefile, "GridSize : (%2d,%2d)\n", mapatt[0], mapatt[1]);
    fprintf(savefile, "StartPos : (%2d,%2d)\n", mapatt[2], mapatt[3]);
    fprintf(savefile, "EndPos : (%2d,%2d)\n", mapatt[4], mapatt[5]);
    fprintf(savefile, "IdealTime : %3d\n", mapatt[6]);
    for(i = 0; i < 10; i++)
        fprintf(savefile, "(%2d,%2d)", food[i][0], food[i][1]);
    fprintf(savefile, "\n");
    //recording map frame data to file
    for(i = 0; i < ys; i++)
    {
        for(j = 0; j < xs; j++)
        {
            if(frame[i][j] == 178)
                fprintf(savefile, "1");
            else if(frame[i][j] == 176)
                fprintf(savefile, "0");
        }
    }
    fclose(savefile);
}
//Display the Scoreboard of alltime
void Scoreboard()
{
    int score[64][3];
    int i, j;
    FILE *readscore;
    readscore = fopen("scoreboard.dat", "r");
    //reading score data from file
    for(i = 0; !feof(readscore); i++)
        fscanf(readscore, "{%2d,%4d,%3d}", &score[i][0], &score[i][1], &score[i][2]);
    fclose(readscore);
    printf("\n\tSCOREBOARD\n\n");
    //printing score data to display screen
    for(j = 0; j <= i-2; j++)
    {
        printf("%2d. Map %2d - Score : %4d - Time : ", j+1, score[i-j-2][0], score[i-j-2][1]);
        if((score[i-j-2][2] / 60) != 0)
            printf("%dm%ds\n", score[i-j-2][2]/60, score[i-j-2][2]%60);
        else
            printf("%ds\n", score[i-j-2][2]);
    }
    printf("\nPress Any key after 5sec to continue...");
    timedelay(5);//5sec delay
    getch();
    cls();
}
//Display About info of Game
void About()
{
    //printing about text
    printf("\n\t\tABOUT Game!\n\n");
    printf("\tThis Game is a Semester Project\n");
    printf("\tIt is a Game based on Maze, written in C Language.\n\n");
    printf("\tMade By  : Muhammad Taimoor & Talha Sohail\n");
    printf("\tRoll No  : FA21-BSE-134 & FA21-BSE-020\n\n");
    printf("\tSemester : 01\n");
    printf("\tPress Any Key after 3sec to Continue...");
    timedelay(3);//3sec delay
    getch();
    cls();
}
//prompt to exit
void Exit()
{
    //prompting for exit
    printf("Do you Realy want to exit\n");
    printf("Press 'Y' to Exit or 'N' to return.");
    //selecting option
    do
    {
        switch(getch())
        {
        case 'y'://exit
        case 'Y':
            printf("\nexiting...");
            timedelay(2);
            exit(0);
            break;
        case 'N'://return to main menu
        case 'n':
            cls();
            return;
        }
    }
    while(1);
}
//delay time for specific seconds
void timedelay(int second)
{
    int start_time = time(NULL);//start time
    while(time(NULL) != start_time + second);//run while time is not specific secong late
}
//clear screen
void cls()
{
    system("cls");
}
/*.....End of Program.....*/
