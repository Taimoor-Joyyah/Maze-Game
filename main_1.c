#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FRAME_SIZE 64 //Adjust Max Frame size
#define CHARACTER 1 //Define character symbol
#define MAPS_COUNT 3 //Define Amount of Available Maps

void Note();
void Menu(char *opt);
void SelectMap(); //select map among available
void Play(int frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int MapNum); //play game after selecting map
void EndScore(int FCount, int time, int idealT, int MapNum); //display and write the resulting score to datafile after playing game
void Scoreboard(); //read and display scores of alltime
void About();
void timedelay(int second);
void cls();
void Frame_Display(int frame[FRAME_SIZE][FRAME_SIZE], int yr, int xr);
void Get_Map(int frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int MapNum);

void main() {
    int MenuOpt;
    Note();
    do {
        Menu(&MenuOpt);
        switch(MenuOpt) {
            case '1':
                SelectMap();
                break;
            case '2':
                Scoreboard();
                break;
            case '3':
                About();
                break;
        }
    } while(MenuOpt != '4');
}
//Display a Note on the Start of Program
void Note() {
    printf("\t\nNOTE!\n");
    //.........Note
    printf("\tPress Any Key after 5sec to Continue...");
    timedelay(5);
    getch();
    cls();
}
//Display the game menu and return selected option
void Menu(char *opt) {
    printf("\t MAZE GAME\n\n");
    printf("\t1.\t  Start\n");
    printf("\t2.\tScoreBoard\n");
    printf("\t3.\t  About\n");
    printf("\t4.\t  Exit\n");
    printf("\nSelect Option.");
    do {
        *opt = getch();
    } while(*opt < '1' || *opt > '4');
    cls();
}
//Select map from available
void SelectMap() {
    int MazeFrame[FRAME_SIZE][FRAME_SIZE];
    int mapattribute[7];//{y_range, x_range, Start_y, Start_x, End_y, End_x, idealTime}
    int Food[10][2];
    int MapNo = 1;
    char key;
    while(key != 'w' && key != 'W') {
        printf("\tSELECT MAP\n");
        printf("\tMap No. %d\n", MapNo);
        printf("Press S for Next Map, Press W to Select and Press ESC to Back to Menu\n\n");
        Get_Map(MazeFrame, mapattribute, Food, MapNo);
        Frame_Display(MazeFrame, mapattribute[0], mapattribute[1]);
        printf("\n");
        do {
            key = getch();
        } while(key != 27 && key != 'W' && key != 'S' && key != 'w' && key != 's');
        if(key == 27) {
            cls();
            return;
        }
        else if(key == 's' || key == 'S') {
            if(MapNo != MAPS_COUNT)
                MapNo++;
            else
                MapNo = 1;
        cls();
        }
    }
    cls();
    Play(MazeFrame, mapattribute, Food, MapNo);
}
//Play Game
void Play(int frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int MapNum) {
    int y_range, x_range;
    int y_pos, x_pos, c_pos;
    int foodcount = 0;
    int start_T = time(NULL);
    int total_T;
    frame[mapatt[2]][mapatt[3]] = CHARACTER; //start character
    frame[mapatt[4]][mapatt[5]] = 20; //end point
    for(int i = 0; i < 10; i++)
        frame[food[i][0]][food[i][1]] = 4;
    y_pos = mapatt[2], x_pos = mapatt[3], c_pos = -1;
    do {
        if(c_pos != y_pos * mapatt[1] + x_pos) {
            cls();
            Frame_Display(frame, mapatt[0], mapatt[1]); //[0] y_range, [1] x_range
            printf("Pos   : %2d,%2d\n", y_pos, x_pos);
            printf("Score : %-4d", foodcount * 100);
        }
        c_pos = y_pos * mapatt[1] + x_pos;
        //Control Movement of the character
        switch(getch()) {
            case 'w':
            case 'W':
                if(frame[y_pos-1][x_pos] != '1') {
                    if(frame[y_pos-1][x_pos] == 4)
                        foodcount++;
                    frame[y_pos][x_pos] = 176;
                    frame[--y_pos][x_pos] = CHARACTER;
                }
                break;
            case 's':
            case 'S':
                if(frame[y_pos+1][x_pos] != '1') {
                    if(frame[y_pos+1][x_pos] == 4)
                        foodcount++;
                    frame[y_pos][x_pos] = 176;
                    frame[++y_pos][x_pos] = CHARACTER;
                }
                break;
            case 'a':
            case 'A':
                if(frame[y_pos][x_pos-1] != '1') {
                    if(frame[y_pos][x_pos-1] == 4)
                        foodcount++;
                    frame[y_pos][x_pos] = 176;
                    frame[y_pos][--x_pos] = CHARACTER;
                }
                break;
            case 'd':
            case 'D':
                if(frame[y_pos][x_pos+1] != '1') {
                    if(frame[y_pos][x_pos+1] == 4)
                        foodcount++;
                    frame[y_pos][x_pos] = 176;
                    frame[y_pos][++x_pos] = CHARACTER;
                }
                break;
            case 27:
                cls();
                return;
        }
    } while((y_pos != mapatt[4]) || (x_pos != mapatt[5]));
    cls();
    total_T = time(NULL) - start_T;
    EndScore(foodcount, total_T, mapatt[6], MapNum);
}
//Display End Score and write them to scoreboard
void EndScore(int FCount, int time, int idealT, int MapNum) {
    int Win_R = 1000;
    int Food_R = FCount * 100;
    int Time_R = (((idealT * 1000) / time) / 10) * 10;
    int Score = Win_R + Food_R + Time_R;
    printf("\n\tSCORE\n");
    printf("\n\tWin Reward     = %-4d", Win_R);
    printf("\n\tFood %2d X 100  = %-4d", FCount, Food_R);
    printf("\n\tTime Reward    = %-4d", Time_R);
    printf("\n\n\tTotal Score    = %-4d\n", Score);
    printf("\n\tMap No. %d\n", MapNum);
    if((time / 60) != 0)
        printf("\n\tTotal Time : %dm%ds\n", time/60, time%60);
    else
        printf("\n\tTotal Time : %ds\n", time);
    FILE *scoreboard;
    scoreboard = fopen("scoreboard.dat", "a");
    fprintf(scoreboard, "-%2d_%4d_%3d\0", MapNum, Score, time);
    fclose(scoreboard);
    printf("\nPress Any Key After 5sec To Continue...");
    timedelay(5);
    getch();
    cls();
}
//Display the Scoreboard of alltime
void Scoreboard() {
    int score[64][3];
    int i, j;
    FILE *readscore;
    readscore = fopen("scoreboard.dat", "r");
    printf("\n\tSCOREBOARD\n\n");
    for(i = 0; !feof(readscore); i++)
        fscanf(readscore, "-%2d_%4d_%3d", &score[i][0], &score[i][1], &score[i][2]);
    for(j = 0; j <= i-2; j++)
        printf("%2d. Map %2d - Score : %4d - Time : %3d\n", j+1, score[i-j-2][0], score[i-j-2][1], score[i-j-2][2]);
    printf("\nPress Any key after 5sec to continue...");
    timedelay(5);
    getch();
    cls();
}
//Display About info of Game
void About() {
    printf("\t\nABOUT Game!\n");
    //.........About
    printf("\tPress Any Key after 5sec to Continue...");
    timedelay(5);
    getch();
    cls();
}
//delay time for specific seconds
void timedelay(int second) {
    int start_time = time(NULL);
    while(time(NULL) != start_time + second);
}
//clear screen
void cls() {
  system("cls");
}
//Print Frame on Console
void Frame_Display(int frame[FRAME_SIZE][FRAME_SIZE], int yr, int xr) {
    for(int y = 0; y <= yr - 1; y++) {
        for(int x = 0; x <= xr - 1; x++) {
            if(frame[y][x] == '1')
                printf("%c", 178);
            else if(frame[y][x] == '0')
                printf("%c", 176);
            else
                printf("%c", frame[y][x]);
        }
        printf("\n");
    }
}
//Get Map data from file
void Get_Map(int frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int MapNum) {
    int ch[2];
    int i;
    char MFileN[9] = {'m', 'a', 'p', MapNum + '0', '.', 'd', 'a', 't', 0};
    FILE *mapdata;
    if((mapdata = fopen(MFileN, "r")) == NULL) {
        printf("\nFile Cannot Be Opened OR Selected2 Map is not Available\n");
        printf("press any key to exit.");
        getch();
        exit(0);
    }
    //reading map attributes
    for(i = 0; i < 6; i+=2)
        fscanf(mapdata, "%2d_%2d-", &mapatt[i], &mapatt[i+1]);
    fscanf(mapdata, "%3d-", &mapatt[6]);
    //reading food position
    for(i = 0; i < 10; i++)
        fscanf(mapdata, "%2d_%2d-", &food[i][0], &food[i][1]);
    for(int y = 0; y <= mapatt[0] - 1; y++)
        for(int x = 0; x <= mapatt[1] - 1; x++)
            frame[y][x] = fgetc(mapdata);
}

/*8 for backspace \b, 13 for start of current line \r, 10 for endofline and to next line \n, */
/*219, 176, 177, 178 for frame background*/
/*58 for pipe represent*/
/*1, 2 for character*/
/*3-6 food*/
/*0, 32 for blank space*/
/*31 end symbol*/

/*Press key to change character. Press key to change background*/

/*
SLECT MAP     ->  Select Map -> Play
SCOREBOARD  -> read score.dat
ABOUT -> info how to play game and info about keys (press s to move cursor/change map, w to select, q to exit)
EXIT
*/
//display total time at end
