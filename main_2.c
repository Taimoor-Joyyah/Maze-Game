#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define FRAME_SIZE 64 //Adjust Max Frame size
#define CHARACTER 1 //Define character symbol

void Note();//prints the note on the start of program
char Menu();//displays the menu to select option
void SelectMap(); //select map among available
int mapscount(); //count amount of map available
void Play(int frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int MapNum); //play game after selecting map
void EndScore(int FCount, int time, int idealT, int MapNum); //display and write the resulting score to datafile after playing game
void Scoreboard(); //read and display scores of alltime
void About();//Display About info of Game
void Exit();//prompt to exit
void timedelay(int second);//delay time for specific seconds
void cls();//delay time for specific seconds
void Frame_Display(int frame[FRAME_SIZE][FRAME_SIZE], int yr, int xr);//Print Frame on Console
void Get_Map(int frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int MapNum);//Print Frame on Console

void main() {
    Note();
    do {
        switch(Menu()) {
            case '1':
                SelectMap();
                break;
            case '2':
                Scoreboard();
                break;
            case '3':
                About();
                break;
            case '4':
            case 27:
                Exit();
                break;
        }
    } while(1);
}
//Display a Note on the Start of Program
void Note() {
    printf("\n\t\tNOTE!\n");
    printf("\tThe Game have some keys to control it.\n");
    printf("\tIn Menu you need to press the Menu No. you want to select.\n");
    printf("\tIn Select Map press 'S' to change map,'W' to select map or 'ESC' to exit back to MAIN MENU.\n");
    printf("\tIn Gameplay press 'W' to move UP, 'S' to move DOWN, 'A' to move LEFT, 'D' to move RIGHT, and 'ESC' to exit back to MAIN MENU.\n");
    printf("\tThe Game Print Scores on the end of play session, and record it.\n");
    printf("\tYou can view your all played game score in scoreboard.\n");
    //.........Note
    printf("\n\tPress Any Key after 5sec to Continue...");
    timedelay(5);
    getch();
    cls();
}
//Display the game menu and return selected option
char Menu() {
    printf("\t MAZE GAME\n\n");
    printf("\t1.\t  Start\n");
    printf("\t2.\tScoreBoard\n");
    printf("\t3.\t  About\n");
    printf("\t4.\t  Exit\n");
    printf("\nSelect Option.");
    char opt;
    do {
        opt = getch();
    } while(opt < '1' || opt > '4');
    cls();
    return opt;
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
            if(MapNo != mapscount())
                MapNo++;
            else
                MapNo = 1;
        cls();
        }
    }
    cls();
    Play(MazeFrame, mapattribute, Food, MapNo);
}
//Count Available maps form map1.dat to map9.dat
int mapscount() {
    FILE *mapcount;
    int i;
    char filename[9] = {'m', 'a', 'p', '1', '.', 'd', 'a', 't', '\0'};
    for(i = 2; (mapcount = fopen(filename, "r") != NULL);i++)
        filename[3] = i;
    return i;
}
//Play Game
void Play(int frame[FRAME_SIZE][FRAME_SIZE], int mapatt[7], int food[10][2], int MapNum) {
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
                if((y_pos-1 >= 0) && (frame[y_pos-1][x_pos] != '1')) {
                    if(frame[y_pos-1][x_pos] == 4)
                        foodcount++;
                    frame[y_pos][x_pos] = 176;
                    frame[--y_pos][x_pos] = CHARACTER;
                }
                break;
            case 's':
            case 'S':
                if((y_pos+1 <= mapatt[0]-1)  && (frame[y_pos+1][x_pos] != '1')) {
                    if(frame[y_pos+1][x_pos] == 4)
                        foodcount++;
                    frame[y_pos][x_pos] = 176;
                    frame[++y_pos][x_pos] = CHARACTER;
                }
                break;
            case 'a':
            case 'A':
                if((x_pos-1 >= 0) && (frame[y_pos][x_pos-1] != '1')) {
                    if(frame[y_pos][x_pos-1] == 4)
                        foodcount++;
                    frame[y_pos][x_pos] = 176;
                    frame[y_pos][--x_pos] = CHARACTER;
                }
                break;
            case 'd':
            case 'D':
                if((x_pos+1 <= mapatt[1]) && (frame[y_pos][x_pos+1] != '1')) {
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

    FILE *scoreboard;
    scoreboard = fopen("scoreboard.dat", "a");
    fprintf(scoreboard, "{%2d,%4d,%3d}", MapNum, Score, time);
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
        fscanf(readscore, "{%2d,%4d,%3d}", &score[i][0], &score[i][1], &score[i][2]);
    fclose(readscore);
    for(j = 0; j <= i-2; j++) {
        printf("%2d. Map %2d - Score : %4d - Time : ", j+1, score[i-j-2][0], score[i-j-2][1]);
        if((score[i-j-2][2] / 60) != 0)
            printf("%dm%ds\n", score[i-j-2][2]/60, score[i-j-2][2]%60);
        else
            printf("%ds\n", score[i-j-2][2]);
    }
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
//prompt to exit
void Exit() {
    printf("Do you Realy want to exit\n");
    printf("Press 'Y' to Exit or 'N' to return.");
    do {
        switch(getch()) {
        case 'y':
        case 'Y':
            exit(0);
            break;
        case 'N':
        case 'n':
            cls();
            return;
        }
    } while(1);
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
    fscanf(mapdata, "GridSize : (%2d,%2d)\n", &mapatt[0], &mapatt[1]);//reading grid size
    fscanf(mapdata, "StartPos : (%2d,%2d)\n", &mapatt[2], &mapatt[3]);
    fscanf(mapdata, "EndPos : (%2d,%2d)\n", &mapatt[4], &mapatt[5]);
    fscanf(mapdata, "IdealTime : %3d\n", &mapatt[6]);
    //reading food position
    for(i = 0; i < 10; i++)
        fscanf(mapdata, "(%2d,%2d)", &food[i][0], &food[i][1]);
    fgetc(mapdata);
    for(int y = 0; y <= mapatt[0] - 1; y++)
        for(int x = 0; x <= mapatt[1] - 1; x++)
            frame[y][x] = fgetc(mapdata);
    fclose(mapdata);
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
