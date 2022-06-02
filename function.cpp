#include "function.h"

int32 Map[MAP_SIZE + 1][MAP_SIZE + 1]; // 이차원 map

int32 snake_speed;		// 뱀이 움직이는 시간
char running_direction; // 방향 : right, left, up, donw
bool gameover;

//position vector
vector<BasicPosition> snake;
// item
BasicPosition upgrade_item;
BasicPosition downgrade_item;

// gate
BasicPosition first_gate_position;
BasicPosition second_gate_position;
BasicPosition exit_position;

int upgrade_item_cnt;
int downgrade_item_cnt;
int gate_cnt;
int upgrade_now_cnt, downgrade_now_cnt, gate_pass_cnt, now_body_len;
int leftover_time_up;
int leftover_time_down;
int leftover_time_gate;
int passed_gate_cnt;
int stage;

// int exit_x, exit_y;
int check_up_item, check_down_item, check_passed_g, max_body_len;


void Start()
{

	////////////////////////////////////////////////////////
	clear();
	Init_Window();
	mvprintw(MAP_SIZE, MAP_SIZE, "Stage %d", stage + 1);
	refresh();
	usleep(2000000);

	running_direction = 'l';
	snake_speed = 120000;
	upgrade_item_cnt = 0;
	downgrade_item_cnt = 0;
	// gate
	gate_cnt = 0;
	leftover_time_up = 50;
	leftover_time_down = 50;
	leftover_time_gate = 0;
	passed_gate_cnt = 0;

	check_passed_g = 0;
	check_up_item = 0;
	check_down_item = 0;
	max_body_len = 3;
	snake = {};
	Init_Window();
    switch (stage)
    {
        case 0:
        set_StageOne();
        break;
        case 1:
        set_StageTwo();
        break;
        case 2:
        set_StageThree();
        break;
        case 3:
        set_StageFour();
        break;
        default:
        break;
    }
	set_Snake();
	set_UpgradeItem();
	set_DowngradeItem();
	set_Gate();
	draw_cnt_all();
	draw_Window();
	// draw_cnt_all();
	set_goal();

	refresh();
}

void Play()
{
	while (true)
	{
		if (gameover)
		{
			mvprintw(MAP_SIZE / 2, MAP_SIZE, "GAME OVER");
			break;
		}
		if (upgrade_item_cnt == 0)
			set_UpgradeItem();
		if (downgrade_item_cnt == 0)
			set_DowngradeItem();
		// gate
		if (gate_cnt == 0)
			set_Gate();
		all_timer();
		moveSnake();
		// draw_cnt_all();
		draw_Window();
		draw_cnt_all();
		usleep(snake_speed);
	}
}

void End()
{
	nodelay(stdscr, false); // nodelay = 키 사용x, stdscr = 기본화면
	getch();
	endwin();
}

void printMap()
{
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			cout << Map[i][j];
		}
		cout << endl;
	}
	// cout << upgrade_item.x << "," << upgrade_item.y << endl;
	cout << first_gate_position.x << " " << first_gate_position.y << endl;
	cout << second_gate_position.x << " " << second_gate_position.y << endl;
	cout << exit_position.x << " " << exit_position.y << endl;
	// cout << snake[0].x << "," << snake[0].y << endl;
}

void draw_Window()
{
	clear();
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (Map[i][j] == WALL)
			{
				attron(COLOR_PAIR(WALL));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(WALL));
			}
			else if (Map[i][j] == PERMANENT_WALL)
			{
				attron(COLOR_PAIR(PERMANENT_WALL));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(PERMANENT_WALL));
			}
			else if (Map[i][j] == HEAD)
			{
				attron(COLOR_PAIR(HEAD));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(HEAD));
			}
			else if (Map[i][j] == BODY)
			{
				attron(COLOR_PAIR(BODY));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(BODY));
			}
			else if (Map[i][j] == UPITEM)
			{
				attron(COLOR_PAIR(UPITEM));
				mvprintw(i, j * 2, "\u2605");
				attroff(COLOR_PAIR(UPITEM));
			}
			else if (Map[i][j] == DOWNITEM)
			{
				attron(COLOR_PAIR(DOWNITEM));
				mvprintw(i, j * 2, "\u2605");
				attroff(COLOR_PAIR(DOWNITEM));
			}
			else if (Map[i][j] == GATE)
			{
				attron(COLOR_PAIR(GATE));
				mvprintw(i, j * 2, "\u25A0");
				attroff(COLOR_PAIR(GATE));
			}
		}
	}
	refresh();
}

void draw_cnt_all()
{
	for (int i = 0; i < 18; i++)
	{
		for (int j = 44; j < 59; j++)
		{

			if (i == 0 || i == 6 || i == 8 || i == 14 || i == 17)
				mvprintw(i, j, "-");
			else if (i!=7 && (j == 44 || j == 58))
				mvprintw(i, j, "|");
		}
	}

	mvprintw(1, 47, "Score Board");
	mvprintw(2, 47, "Len: %d / %d", snake.size(), max_body_len);
	mvprintw(3, 47, "Up: %d", check_up_item);
	mvprintw(4, 47, "Down: %d", check_down_item);
	mvprintw(5, 47, "Gate: %d", check_passed_g);

	mvprintw(9, 47, "Goal Board ");
	mvprintw(10, 47, "Len: %d", now_body_len);
	if (max_body_len >= now_body_len)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(11, 47, "Up: %d", upgrade_now_cnt);
	if (check_up_item >= upgrade_now_cnt)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(12, 47, "Down: %d", downgrade_now_cnt);
	if (check_down_item >= downgrade_now_cnt)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(13, 47, "Gate: %d", gate_pass_cnt);
	if (check_passed_g >= gate_pass_cnt)
	{
		printw(" (V)");
	}
	else
	{
		printw(" ( )");
	}
	mvprintw(15, 47, "Gate Time");
	mvprintw(16, 50, " %d ", leftover_time_gate);

	refresh();
}

void moveSnake()
{

	if (passed_gate_cnt > 0)
	{
		if (passed_gate_cnt == 1)
		{
			Map[first_gate_position.y][first_gate_position.x] = WALL;
			Map[second_gate_position.y][second_gate_position.x] = WALL;
			check_passed_g += 1; //획득개수 체크
						  // set_Gate();
		}
		passed_gate_cnt--;
	}

	int32 last = snake.size();

	BasicPosition last_body;
	last_body.x = snake[last - 1].x; //몸통 마지막 좌표 저장
	last_body.y = snake[last - 1].y;
	Map[snake[last - 1].y][snake[last - 1].x] = EMPTY; // 몸통 마지막 좌표 지우기

	for (int i = last - 1; i > 0; i--)
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
	}

	int32 key = getch(); // 방향키 입력받기
	switch (key)
	{
	case KEY_LEFT:
		if (running_direction != 'r')
			running_direction = 'l';
		else
			gameover = true;
		break;
	case KEY_RIGHT:
		if (running_direction != 'l')
			running_direction = 'r';
		else
			gameover = true;
		break;
	case KEY_UP:
		if (running_direction != 'd')
			running_direction = 'u';
		else
			gameover = true;
		break;
	case KEY_DOWN:
		if (running_direction != 'u')
			running_direction = 'd';
		else
			gameover = true;
		break;
	}

	if (running_direction == 'l')
		snake[0].x--;
	else if (running_direction == 'r')
		snake[0].x++;
	else if (running_direction == 'u')
		snake[0].y--;
	else if (running_direction == 'd')
		snake[0].y++;

	// 자기 몸통에 부딪힐 때
	for (int32 i = 1; i < snake.size(); i++)
	{
		if (snake[0].x == snake[i].x && snake[0].y == snake[i].y)
			gameover = true;
	}
	// 벽에 부딪힐 때
	if (Map[snake[0].y][snake[0].x] == WALL || Map[snake[0].y][snake[0].x] == PERMANENT_WALL)
		gameover = true;

	// 성장 아이템
	if (Map[snake[0].y][snake[0].x] == UPITEM)
	{
		upgrade_item_cnt--;
		check_up_item += 1;
		snake.push_back(last_body);

		if (max_body_len < snake.size())
		{
			max_body_len = snake.size();
		}
	}

	// 독 아이템
	if (Map[snake[0].y][snake[0].x] == DOWNITEM)
	{
		if (snake.size() == 3)
			gameover = true;
		Map[snake[last - 1].y][snake[last - 1].x] = EMPTY;
		check_down_item += 1;
		downgrade_item_cnt--;
		snake.pop_back();
	}

	// pass gate
	if (Map[snake[0].y][snake[0].x] == GATE)
	{
		// BasicPosition exit_position;
		passed_gate_cnt = last;

		if (snake[0].y == first_gate_position.x && snake[0].x == first_gate_position.y)
		{
			exit_position.x = second_gate_position.x;
			exit_position.y = second_gate_position.y;
		}
		else
		{
			exit_position.x = first_gate_position.x;
			exit_position.y = first_gate_position.y;
		}

		//게이트의 위치가 가장자리인 경우
		if (exit_position.y == 0)
		{
			snake[0].y = exit_position.x;
			snake[0].x = exit_position.y + 1;
			running_direction = 'r';
		}
		else if (exit_position.y == MAP_SIZE)
		{
			snake[0].y = exit_position.x;
			snake[0].x = exit_position.y - 1;
			running_direction = 'l';
		}
		else if (exit_position.x == 0)
		{
			snake[0].y = exit_position.x + 1;
			snake[0].x = exit_position.y;
			running_direction = 'd';
		}
		else if (exit_position.x == MAP_SIZE)
		{
			snake[0].y = exit_position.x - 1;
			snake[0].x = exit_position.y;
			running_direction = 'u';
		}

		//게이트의 위치가 가장자리가 아닌 경우
		else
		{
			snake[0].y = exit_position.x;
			snake[0].x = exit_position.y;

			char tempdir = running_direction;

			// 진입 방향과 일치
			if (running_direction == 'l')
				snake[0].x--;
			else if (running_direction == 'r')
				snake[0].x++;
			else if (running_direction == 'u')
				snake[0].y--;
			else if (running_direction == 'd')
				snake[0].y++;

			if (Map[snake[0].y][snake[0].x] == GATE || Map[snake[0].y][snake[0].x] == WALL || Map[snake[0].y][snake[0].x] == PERMANENT_WALL)
			{
				snake[0].y = exit_position.x;
				snake[0].x = exit_position.y;

				// 진입 방향의 시계방향
				switch (running_direction)
				{
				case 'u':
					running_direction = 'r';
					break;
				case 'd':
					running_direction = 'l';
					break;
				case 'l':
					running_direction = 'u';
					break;
				case 'r':
					running_direction = 'd';
					break;
				}
				if (running_direction == 'l')
					snake[0].x--;
				else if (running_direction == 'r')
					snake[0].x++;
				else if (running_direction == 'u')
					snake[0].y--;
				else if (running_direction == 'd')
					snake[0].y++;

				if (Map[snake[0].y][snake[0].x] == GATE || Map[snake[0].y][snake[0].x] == WALL || Map[snake[0].y][snake[0].x] == PERMANENT_WALL)
				{
					snake[0].y = exit_position.x;
					snake[0].x = exit_position.y;

					// 진입 방향의 반시계방향
					switch (tempdir)
					{
					case 'u':
						running_direction = 'l';
						break;
					case 'd':
						running_direction = 'r';
						break;
					case 'l':
						running_direction = 'd';
						break;
					case 'r':
						running_direction = 'u';
						break;
					}
					if (running_direction == 'l')
						snake[0].x--;
					else if (running_direction == 'r')
						snake[0].x++;
					else if (running_direction == 'u')
						snake[0].y--;
					else if (running_direction == 'd')
						snake[0].y++;

					if (Map[snake[0].y][snake[0].x] == GATE || Map[snake[0].y][snake[0].x] == WALL || Map[snake[0].y][snake[0].x] == PERMANENT_WALL)
					{
						snake[0].y = exit_position.x;
						snake[0].x = exit_position.y;
						switch (tempdir)
						{
						case 'u':
							running_direction = 'd';
							break;
						case 'd':
							running_direction = 'u';
							break;
						case 'l':
							running_direction = 'r';
							break;
						case 'r':
							running_direction = 'l';
							break;
						}

						if (running_direction == 'l')
							snake[0].x--;
						else if (running_direction == 'r')
							snake[0].x++;
						else if (running_direction == 'u')
							snake[0].y--;
						else if (running_direction == 'd')
							snake[0].y++;
					}
				}
			}
		}
	}

	// Map 에 그리기
	Map[snake[0].y][snake[0].x] = HEAD;
	for (int i = 1; i < snake.size(); i++)
	{
		Map[snake[i].y][snake[i].x] = BODY;
	}
	check_goal();
}


void all_timer()
{
	leftover_time_up--;
	leftover_time_down--;
	leftover_time_gate--;

	if (leftover_time_up == 0)
	{
		Map[upgrade_item.x][upgrade_item.y] = EMPTY;
		upgrade_item_cnt--;
		set_UpgradeItem();
	}

	if (leftover_time_down == 0)
	{
		Map[downgrade_item.x][downgrade_item.y] = EMPTY;
		downgrade_item_cnt--;
		set_DowngradeItem();
	}

	if (leftover_time_gate == 0)
	{
		Map[first_gate_position.x][first_gate_position.y] = WALL;
		Map[second_gate_position.x][second_gate_position.y] = WALL;
		gate_cnt--;
		set_Gate();
	}
}

void check_goal()
{

	if (check_passed_g >= gate_pass_cnt && check_up_item >= upgrade_now_cnt && check_down_item >= downgrade_now_cnt && max_body_len >= now_body_len)
	{
		if (stage <= 3)
		{
			clear();
			mvprintw(MAP_SIZE / 2, MAP_SIZE, "You Cleared Stage %d!", stage + 1);
			mvprintw(MAP_SIZE / 2 + 1, MAP_SIZE + 1, "Lets' move on to the Next Stage");
			Map[snake[0].y][snake[0].x] = HEAD;
			for (int i = 1; i < 3; i++)
			{
				Map[snake[i].y][snake[i].x] = BODY;
			}
			refresh();
			usleep(2000000);
			stage++;
			Start();
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