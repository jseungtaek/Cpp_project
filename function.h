#ifndef FUNCTION_H
#define FUNCTION_H
#include <iostream>
#include <ncursesw/curses.h> //nurses unicode 출력
#include <locale.h> // 유니코드 그리는 용
#include <vector>	// snake 구현용
#include <cstdlib>	// rand 함수
#include <ctime>	// 난수 생성
#include <unistd.h> // sleep 함수
#include <string>
#include "BasicPosition.h"
#include "SetGame.h"

#define MAP_SIZE 23
#define EMPTY 0
#define WALL 1		  // WHITE
#define PERMANENT_WALL 2 // YELLOW
#define HEAD 3  // CYAN
#define BODY 4  // BLUE
#define UPITEM 5	  // GREEN
#define DOWNITEM 6	  // RED
#define GATE 7		  // MAGENTA
using namespace std;
using int32 = int;

void all_timer();

// step5
void check_goal();

//draw
void draw_Window();
void draw_cnt_all();
void moveSnake();
void printMap();

//game
void Start();
void Play();
void End();

extern int32 Map[MAP_SIZE + 1][MAP_SIZE + 1]; // 이차원 map

extern int32 snake_speed;		// 뱀이 움직이는 시간
extern string running_direction; // 방향 : right, left, up, donw
extern bool gameover;

extern int upgrade_item_cnt;
extern int downgrade_item_cnt;
extern int gate_cnt;
extern int upgrade_now_cnt, downgrade_now_cnt, gate_pass_cnt, now_body_len;
extern int leftover_time_up;
extern int leftover_time_down;
extern int leftover_time_gate;
extern int passed_gate_cnt;
extern int stage;

// extern int exit_x, exit_y;
extern int check_up_item, check_down_item, check_passed_g, max_body_len;

#endif