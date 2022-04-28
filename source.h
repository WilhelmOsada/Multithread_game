#pragma once
#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <curses.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>
#include <semaphore.h>
#include <map>

using namespace std;

#define PURPLE 1
#define RED 2
#define YELLOW 3
#define GREEN 4

#define CH_CHAR(ch) (ch & A_CHARTEXT)
#define CH_COLOR(ch) (ch & A_COLOR)

#define WALL 0x2588
#define END_ME 6969
#define SPAWN_ME 9696
#define SECOND 1000000
#define ROUND_TIME 0.2
#define MAX_NUM_OF_BEASTS 10

#define SEM_IN_1 "in1"
#define SEM_IN_2 "in2"
#define SEM_IN_3 "in3"
#define SEM_IN_4 "in4"

#define SEM_OUT_1 "out1"
#define SEM_OUT_2 "out2"
#define SEM_OUT_3 "out3"
#define SEM_OUT_4 "out4"

#define SEM_SERV_1 "serv_sem_1"
#define SEM_SERV_2 "serv_sem_2"
#define SEM_SERV_3 "serv_sem_3"
#define SEM_SERV_4 "serv_sem_4"

#define BEAST "beast_sem"

typedef enum
{
    GO_UP = 72,
    GO_DOWN = 80,
    GO_LEFT = 75,
    GO_RIGHT = 77,
    QUIT1 = 'q',
    QUIT2 = 'Q',
    COIN = 'c',
    SMALL_TREASURE = 't',
    BIG_TREASURE = 'T'
}moveset;

typedef enum
{
    HUMAN,CPU,NONE
}
interface;

typedef struct
{
    int PID;
    int player_x;
    int player_y;
    int coins_found;
    int coins_brought;
    int deaths;
    int round;
    int waiting;
    int number;
    interface who;
    wchar_t map[25];
}
Player_Stats;

typedef struct 
{
    int move;
    int process_id;    
}comms;

typedef struct 
{
    int on;
    int is_chasing;
    int beast_x;
    int beast_y;

}beasts;






