#include <iostream>
// #include <ncurses.h>
#include <ncursesw/curses.h> //nurses unicode 출력
#include <locale.h> // 유니코드 그리는 용
#include <vector>	// snake 구현용
#include <cstdlib>	// rand 함수
#include <ctime>	// 난수 생성
#include <unistd.h> // sleep 함수

#define MAP_SIZE 21
#define EMPTY 0
#define WALL 1		  // WHITE
#define IMMUNE_WALL 2 // YELLOW
#define SNAKE_HEAD 3  // CYAN
#define SNAKE_BODY 4  // BLUE
#define GROWTH 5	  // GREEN
#define POISON 6	  // RED
#define GATE 7		  // MAGENTA

using namespace std;
using int32 = int;

struct CharPosition
{
	int32 x, y;
	CharPosition(int32 col, int32 row);
	CharPosition();
};

CharPosition::CharPosition(int32 col, int32 row)
{
	x = col;
	y = row;
}

CharPosition::CharPosition()
{
	x = 0;
	y = 0;
}

int32 snakeMap[MAP_SIZE + 1][MAP_SIZE + 1]; // 맵이 저장된 이차원 배열

int32 sdelay;	// 뱀이 움직이는 시간
char direction; // 방향
bool gameover;
vector<CharPosition> snake;

CharPosition growth_item;
CharPosition poison_item;
// gate
CharPosition gate_position1;
CharPosition gate_position2;
CharPosition exit_position;

int num_growth;
int num_poison;
int num_gate;

int n_growth, n_poison, n_gate, n_body;

int time_growth;
int time_poison;
int time_gate;
int gatecount;
int stage = 0;

// int exit_x, exit_y;
int chkgrowth, chkpoison, chkgate, maxbody;

void InitWindow();
void resetStage();
void setStageOne();
void setStageTwo();
void setStageThree();
void setStageFour();
void setSnake();
void drawWindow();
void moveSnake();
void drawCount();

void StartGame();
void PlayGame();
void EndGame();

void setGrowthItem();
void setPosionItem();
void timer();

// step4
void setGate();

// step5
void chkgoal();
void setgoal();

void StartGame()
{

	////////////////////////////////////////////////////////
	clear();
	InitWindow();
	mvprintw(MAP_SIZE, MAP_SIZE, "Stage %d", stage + 1);
	refresh();
	usleep(2000000);

	direction = 'l';
	sdelay = 120000;
	num_growth = 0;
	num_poison = 0;
	// gate
	num_gate = 0;
	time_growth = 50;
	time_poison = 50;
	time_gate = 0;
	gatecount = 0;

	chkgate = 0;
	chkgrowth = 0;
	chkpoison = 0;
	maxbody = 3;

	snake = {};

	InitWindow();
	resetStage();
	if (stage == 0)
	{
		setStageOne();
	}
	if (stage == 1)
	{
		setStageTwo();
	}
	if (stage == 2)
		setStageThree();
	if (stage == 3)
		setStageFour();
	setSnake();
	setGrowthItem();
	setPosionItem();
	setGate();
	drawCount();
	drawWindow();
	// drawCount();
	setgoal();

	refresh();
}

void PlayGame()
{
	while (true)
	{
		if (gameover)
		{
			mvprintw(MAP_SIZE / 2, MAP_SIZE, "GAME OVER");
			break;
		}
		if (num_growth == 0)
			setGrowthItem();
		if (num_poison == 0)
			setPosionItem();
		// gate
		if (num_gate == 0)
			setGate();
		timer();
		moveSnake();
		// drawCount();
		drawWindow();
		drawCount();
		usleep(sdelay);
	}
}

void EndGame()
{
	nodelay(stdscr, false); // nodelay = 키 사용x, stdscr = 기본화면
	getch();
	endwin();
}

void InitWindow()
{
	setlocale(LC_ALL, "");
	initscr();
	nodelay(stdscr, TRUE);
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0); // 0에 가까울수록 커서 보이지 않음
	resize_term(MAP_SIZE * 3, MAP_SIZE * 3);

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(4, COLOR_BLUE, COLOR_BLACK);
	init_pair(5, COLOR_GREEN, COLOR_BLACK);
	init_pair(6, COLOR_RED, COLOR_BLACK);
	init_pair(7, COLOR_MAGENTA, COLOR_BLACK);
}

void resetStage()
{
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			snakeMap[i][j] = EMPTY;
		}
	}
}

void setStageOne()
{
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (i == 0 || j == 0 || i == MAP_SIZE || j == MAP_SIZE)
			{
				snakeMap[i][j] = WALL;
			}
			else
			{
				snakeMap[i][j] = EMPTY;
			}
		}
	}
	snakeMap[0][0] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[0][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][0] = IMMUNE_WALL;
}

void setStageTwo()
{
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (i == 0 || j == 0 || i == MAP_SIZE || j == MAP_SIZE)
				snakeMap[i][j] = WALL;
			else if (i % 2 == 0 && j == MAP_SIZE / 2)
				snakeMap[i][j] = WALL;
			else
				snakeMap[i][j] = EMPTY;
		}
	}
	snakeMap[0][0] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[0][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][0] = IMMUNE_WALL;
}

void setStageThree()
{
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (i == 0 || j == 0 || i == MAP_SIZE || j == MAP_SIZE)
				snakeMap[i][j] = WALL;
			else if (i % MAP_SIZE < MAP_SIZE / 2 - 4 && j == MAP_SIZE / 2)
				snakeMap[i][j] = WALL;
			else if (i % MAP_SIZE > MAP_SIZE / 2 + 4 && j == MAP_SIZE / 2)
				snakeMap[i][j] = WALL;
			else if (j % MAP_SIZE < MAP_SIZE / 2 - 4 && i == MAP_SIZE / 2)
				snakeMap[i][j] = WALL;
			else if (j % MAP_SIZE > MAP_SIZE / 2 + 4 && i == MAP_SIZE / 2)
				snakeMap[i][j] = WALL;
			else
				snakeMap[i][j] = EMPTY;
		}
	}
	snakeMap[0][0] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[0][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][0] = IMMUNE_WALL;
	snakeMap[0][MAP_SIZE / 2] = IMMUNE_WALL;
	snakeMap[MAP_SIZE / 2][0] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][MAP_SIZE / 2] = IMMUNE_WALL;
	snakeMap[MAP_SIZE / 2][MAP_SIZE] = IMMUNE_WALL;
}

void setStageFour()
{
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (i == 0 || j == 0 || i == MAP_SIZE || j == MAP_SIZE)
				snakeMap[i][j] = WALL;
			else if (i == j)
				snakeMap[i][j] = WALL;
			else
				snakeMap[i][j] = EMPTY;
		}
	}
	snakeMap[0][0] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[0][MAP_SIZE] = IMMUNE_WALL;
	snakeMap[MAP_SIZE][0] = IMMUNE_WALL;
}

void setSnake()
{
	for (int32 i = 0; i < 3; i++)
	{
		snake.push_back(CharPosition(MAP_SIZE / 2 + i, MAP_SIZE / 2));
	}
}

void printMap()
{
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			cout << snakeMap[i][j];
		}
		cout << endl;
	}
	// cout << growth_item.x << "," << growth_item.y << endl;
	cout << gate_position1.x << " " << gate_position1.y << endl;
	cout << gate_position2.x << " " << gate_position2.y << endl;
	cout << exit_position.x << " " << exit_position.y << endl;
	// cout << snake[0].x << "," << snake[0].y << endl;
}

void drawWindow()
{
	clear();
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (snakeMap[i][j] == WALL)
			{
				attron(COLOR_PAIR(WALL));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(WALL));
			}
			else if (snakeMap[i][j] == IMMUNE_WALL)
			{
				attron(COLOR_PAIR(IMMUNE_WALL));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(IMMUNE_WALL));
			}
			else if (snakeMap[i][j] == SNAKE_HEAD)
			{
				attron(COLOR_PAIR(SNAKE_HEAD));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(SNAKE_HEAD));
			}
			else if (snakeMap[i][j] == SNAKE_BODY)
			{
				attron(COLOR_PAIR(SNAKE_BODY));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(SNAKE_BODY));
			}
			else if (snakeMap[i][j] == GROWTH)
			{
				attron(COLOR_PAIR(GROWTH));
				mvprintw(i, j * 2, "\u2605");
				attroff(COLOR_PAIR(GROWTH));
			}
			else if (snakeMap[i][j] == POISON)
			{
				attron(COLOR_PAIR(POISON));
				mvprintw(i, j * 2, "\u2605");
				attroff(COLOR_PAIR(POISON));
			}
			else if (snakeMap[i][j] == GATE)
			{
				attron(COLOR_PAIR(GATE));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(GATE));
			}
		}
	}
	refresh();
}

void drawCount()
{

	mvprintw(1, 47, "Score Board");
	mvprintw(2, 47, "B: %d / %d", snake.size(), maxbody);
	mvprintw(3, 47, "+: %d", chkgrowth);
	mvprintw(4, 47, "-: %d", chkpoison);
	mvprintw(5, 47, "G: %d", chkgate);

	mvprintw(9, 47, "Mission Board ");
	mvprintw(10, 47, "B: %d", n_body);
	if (maxbody >= n_body)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(11, 47, "+: %d", n_growth);
	if (chkgrowth >= n_growth)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(12, 47, "-: %d", n_poison);
	if (chkpoison >= n_poison)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(13, 47, "G: %d", n_gate);
	if (chkgate >= n_gate)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(15, 47, "Gate Time\n");
	mvprintw(16, 47, " %d ", time_gate);

	refresh();
}

void moveSnake()
{

	if (gatecount > 0)
	{
		if (gatecount == 1)
		{
			snakeMap[gate_position1.y][gate_position1.x] = WALL;
			snakeMap[gate_position2.y][gate_position2.x] = WALL;
			chkgate += 1; //획득개수 체크
			// setGate();
		}
		gatecount--;
	}

	int32 last = snake.size();

	CharPosition last_body;
	last_body.x = snake[last - 1].x; //몸통 마지막 좌표 저장
	last_body.y = snake[last - 1].y;
	snakeMap[snake[last - 1].y][snake[last - 1].x] = EMPTY; // 몸통 마지막 좌표 지우기

	for (int i = last - 1; i > 0; i--)
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	int32 key = getch(); // 방향키 입력받기
	switch (key)
	{
	case KEY_LEFT:
		if (direction != 'r')
			direction = 'l';
		else
			gameover = true;
		break;
	case KEY_RIGHT:
		if (direction != 'l')
			direction = 'r';
		else
			gameover = true;
		break;
	case KEY_UP:
		if (direction != 'd')
			direction = 'u';
		else
			gameover = true;
		break;
	case KEY_DOWN:
		if (direction != 'u')
			direction = 'd';
		else
			gameover = true;
		break;
	}

	if (direction == 'l')
		snake[0].x--;
	else if (direction == 'r')
		snake[0].x++;
	else if (direction == 'u')
		snake[0].y--;
	else if (direction == 'd')
		snake[0].y++;

	// 자기 몸통에 부딪힐 때
	for (int32 i = 1; i < snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			gameover = true;
	}
	// 벽에 부딪힐 때
	if (snakeMap[snake[0].y][snake[0].x] == WALL || snakeMap[snake[0].y][snake[0].x] == IMMUNE_WALL)
		gameover = true;

	// 성장 아이템
	if (snakeMap[snake[0].y][snake[0].x] == GROWTH)
	{
		num_growth--;
		chkgrowth += 1;
		snake.push_back(last_body);

		if (maxbody < snake.size())
		{
			maxbody = snake.size();
		}
	}

	// 독 아이템
	if (snakeMap[snake[0].y][snake[0].x] == POISON)
	{
		if (snake.size() == 3)
			gameover = true;
		snakeMap[snake[last - 1].y][snake[last - 1].x] = EMPTY;
		chkpoison += 1;
		num_poison--;
		snake.pop_back();
	}

	// pass gate
	if (snakeMap[snake[0].y][snake[0].x] == GATE)
	{
		// CharPosition exit_position;
		gatecount = last;

		if (snake[0].y == gate_position1.x && snake[0].x == gate_position1.y)
		{
			exit_position.x = gate_position2.x;
			exit_position.y = gate_position2.y;
		}
		else
		{
			exit_position.x = gate_position1.x;
			exit_position.y = gate_position1.y;
		}

		//게이트의 위치가 가장자리인 경우
		if (exit_position.y == 0)
		{
			snake[0].y = exit_position.x;
			snake[0].x = exit_position.y + 1;
			direction = 'r';
		}
		else if (exit_position.y == MAP_SIZE)
		{
			snake[0].y = exit_position.x;
			snake[0].x = exit_position.y - 1;
			direction = 'l';
		}
		else if (exit_position.x == 0)
		{
			snake[0].y = exit_position.x + 1;
			snake[0].x = exit_position.y;
			direction = 'd';
		}
		else if (exit_position.x == MAP_SIZE)
		{
			snake[0].y = exit_position.x - 1;
			snake[0].x = exit_position.y;
			direction = 'u';
		}

		//게이트의 위치가 가장자리가 아닌 경우
		else
		{
			snake[0].y = exit_position.x;
			snake[0].x = exit_position.y;

			char tempdir = direction;

			// 진입 방향과 일치
			if (direction == 'l')
				snake[0].x--;
			else if (direction == 'r')
				snake[0].x++;
			else if (direction == 'u')
				snake[0].y--;
			else if (direction == 'd')
				snake[0].y++;

			if (snakeMap[snake[0].y][snake[0].x] == GATE || snakeMap[snake[0].y][snake[0].x] == WALL || snakeMap[snake[0].y][snake[0].x] == IMMUNE_WALL)
			{
				snake[0].y = exit_position.x;
				snake[0].x = exit_position.y;

				// 진입 방향의 시계방향
				switch (direction)
				{
				case 'u':
					direction = 'r';
					break;
				case 'd':
					direction = 'l';
					break;
				case 'l':
					direction = 'u';
					break;
				case 'r':
					direction = 'd';
					break;
				}
				if (direction == 'l')
					snake[0].x--;
				else if (direction == 'r')
					snake[0].x++;
				else if (direction == 'u')
					snake[0].y--;
				else if (direction == 'd')
					snake[0].y++;

				if (snakeMap[snake[0].y][snake[0].x] == GATE || snakeMap[snake[0].y][snake[0].x] == WALL || snakeMap[snake[0].y][snake[0].x] == IMMUNE_WALL)
				{
					snake[0].y = exit_position.x;
					snake[0].x = exit_position.y;

					// 진입 방향의 반시계방향
					switch (tempdir)
					{
					case 'u':
						direction = 'l';
						break;
					case 'd':
						direction = 'r';
						break;
					case 'l':
						direction = 'd';
						break;
					case 'r':
						direction = 'u';
						break;
					}
					if (direction == 'l')
						snake[0].x--;
					else if (direction == 'r')
						snake[0].x++;
					else if (direction == 'u')
						snake[0].y--;
					else if (direction == 'd')
						snake[0].y++;

					if (snakeMap[snake[0].y][snake[0].x] == GATE || snakeMap[snake[0].y][snake[0].x] == WALL || snakeMap[snake[0].y][snake[0].x] == IMMUNE_WALL)
					{
						snake[0].y = exit_position.x;
						snake[0].x = exit_position.y;
						switch (tempdir)
						{
						case 'u':
							direction = 'd';
							break;
						case 'd':
							direction = 'u';
							break;
						case 'l':
							direction = 'r';
							break;
						case 'r':
							direction = 'l';
							break;
						}

						if (direction == 'l')
							snake[0].x--;
						else if (direction == 'r')
							snake[0].x++;
						else if (direction == 'u')
							snake[0].y--;
						else if (direction == 'd')
							snake[0].y++;
					}
				}
			}
		}
	}

	// snakeMap 에 그리기
	snakeMap[snake[0].y][snake[0].x] = SNAKE_HEAD;
	for (int i = 1; i < snake.size(); i++)
	{
		snakeMap[snake[i].y][snake[i].x] = SNAKE_BODY;
	}
	chkgoal();
}

void setGrowthItem()
{
	int i, j;
	srand((unsigned int)time(NULL));
	while (true)
	{
		i = rand() % (MAP_SIZE - 2) + 1;
		j = rand() % (MAP_SIZE - 2) + 1;
		if (snakeMap[i][j] == EMPTY)
		{
			snakeMap[i][j] = GROWTH;
			growth_item.x = i;
			growth_item.y = j;
			num_growth++;
			time_growth = 50;
			break;
		}
	}
}

void setPosionItem()
{
	int i, j;
	srand((unsigned int)time(NULL));
	while (true)
	{
		i = rand() % (MAP_SIZE - 2) + 1;
		j = rand() % (MAP_SIZE - 2) + 1;
		if (snakeMap[i][j] == EMPTY)
		{
			snakeMap[i][j] = POISON;
			poison_item.x = i;
			poison_item.y = j;
			num_poison++;
			time_poison = 50;
			break;
		}
	}
}

// make Gate
void setGate()
{

	int i, j, k, s;
	srand((unsigned int)time(NULL));
	while (true)
	{
		i = rand() % (MAP_SIZE + 1);
		j = rand() % (MAP_SIZE + 1);

		if (snakeMap[i][j] == WALL)
		{
			snakeMap[i][j] = GATE;
			gate_position1.x = i;
			gate_position1.y = j;
			time_gate = 50;
			break;
		}
	}

	srand((unsigned int)time(NULL));
	while (1)
	{
		k = rand() % (MAP_SIZE + 1);
		s = rand() % (MAP_SIZE + 1);
		if (snakeMap[k][s] == WALL && snakeMap[k][s] != GATE && !(i == k && j == s))
		{
			snakeMap[k][s] = GATE;
			gate_position2.x = k;
			gate_position2.y = s;
			num_gate++;
			break;
		}
	}
}

void timer()
{
	time_growth--;
	time_poison--;
	time_gate--;

	if (time_growth == 0)
	{
		snakeMap[growth_item.x][growth_item.y] = EMPTY;
		num_growth--;
		setGrowthItem();
	}

	if (time_poison == 0)
	{
		snakeMap[poison_item.x][poison_item.y] = EMPTY;
		num_poison--;
		setPosionItem();
	}

	if (time_gate == 0)
	{
		snakeMap[gate_position1.x][gate_position1.y] = WALL;
		snakeMap[gate_position2.x][gate_position2.y] = WALL;
		num_gate--;
		setGate();
	}
}

void setgoal()
{
	srand((unsigned)time(NULL) + 111);

	n_body = (rand() % 4) + 5;
	n_growth = (rand() % 3) + 3;
	n_poison = (rand() % 3) + 1;
	n_gate = (rand() % 5) + 1;
}

void chkgoal()
{

	if (chkgate >= n_gate && chkgrowth >= n_growth && chkpoison >= n_poison && maxbody >= n_body)
	{
		if (stage <= 3)
		{
			clear();
			mvprintw(MAP_SIZE / 2, MAP_SIZE, "You Cleared Stage %d!", stage + 1);
			mvprintw(MAP_SIZE / 2 + 1, MAP_SIZE + 1, "Lets' move on to the Next Stage");
			snakeMap[snake[0].y][snake[0].x] = SNAKE_HEAD;
			for (int i = 1; i < 3; i++)
			{
				snakeMap[snake[i].y][snake[i].x] = SNAKE_BODY;
			}
			refresh();
			usleep(2000000);
			stage++;

			StartGame();
		}

		else
		{
			clear();
			mvprintw(MAP_SIZE / 2, MAP_SIZE, "You Cleared All Stages");
			mvprintw(MAP_SIZE / 2 + 1, MAP_SIZE + 1, "Great Job!");
			refresh();
			usleep(2000000);
		}
	}
}

int main()
{
	StartGame();
	PlayGame();
	EndGame();
	printMap();
	return 0;
}
