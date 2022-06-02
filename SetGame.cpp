#include "SetGame.h"

void Init_Window()
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

void reset_Stage()
{
	for (int i = 0; i <= MAP_SIZE; i++)
		for (int j = 0; j <= MAP_SIZE; j++)
			Map[i][j] = EMPTY;
}

void set_StageOne()
{
    reset_Stage();
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (i == 0 || j == 0 || i == MAP_SIZE || j == MAP_SIZE)
			{
                Map[i][j] = WALL;
                if (i == j || (i == MAP_SIZE && j == 0) || (i == 0 && j == MAP_SIZE))
                    Map[i][j] = PERMANENT_WALL;
			}
		}
	}
}

void set_StageTwo()
{
    set_StageOne();
	for (int i = 0; i <= MAP_SIZE; i++)
	{
		for (int j = 0; j <= MAP_SIZE; j++)
		{
			if (i % 2 == 0 && j == MAP_SIZE / 2)
				Map[i][j] = WALL;
			if (j >= 1 && j <= MAP_SIZE - 1)
				if(i == MAP_SIZE / 2 || i == MAP_SIZE / 2 + 1 || i == MAP_SIZE / 2 - 1)
					Map[i][j] = EMPTY;
		}
	}
}

void set_StageThree()
{
    set_StageOne();
	for (int i = 0; i <= MAP_SIZE; i++)
	{
        for (int j = 0; j <= MAP_SIZE; j++)
        {
            if (i == MAP_SIZE / 2)
            {
                if (j <= MAP_SIZE / 2 - 5 || j >= MAP_SIZE / 2 + 5)
                    Map[i][j] = WALL;
                if (j == 0 || j == MAP_SIZE)
                    Map[i][j] = PERMANENT_WALL;
            }
            else if (j == MAP_SIZE / 2)
            {
                if (i <= MAP_SIZE / 2 - 5 || i >= MAP_SIZE / 2 + 5)
                    Map[i][j] = WALL;
                if (i == 0 || i == MAP_SIZE)
                    Map[i][j] = PERMANENT_WALL;
            }
        }
    }
}

void set_StageFour()
{
    set_StageOne();
	for (int i = 1; i < MAP_SIZE; i++)
	{
		for (int j = 1; j < MAP_SIZE; j++)
		{
			if (i == j)
				Map[i][j] = WALL;
			if (i == MAP_SIZE / 2 || i == MAP_SIZE / 2 + 1 || i == MAP_SIZE / 2 - 1)
				Map[i][j] = EMPTY;
		}
	}
}

void set_Snake()
{
	for (int32 i = 0; i < 3; i++)
		snake.push_back(BasicPosition(MAP_SIZE / 2, MAP_SIZE / 2 + i)); //snake 초기 position
}

void set_Modified(BasicPosition &l_body, int32 &last)
{
	switch(Map[snake[0].y][snake[0].x])
	{
		case WALL: // 벽
		gameover = true;
		break;
		case PERMANENT_WALL: // 벽
		gameover = true;
		break;
		case UPITEM: // upitem
		{
			upgrade_item_cnt--;
			check_up_item += 1;
			snake.push_back(l_body);

			if (max_body_len < snake.size())
			{
				max_body_len = snake.size();
			}
		}
		break;
		case DOWNITEM: //downitem
		{
			if (snake.size() == 3)
				gameover = true;
			Map[snake[last - 1].y][snake[last - 1].x] = EMPTY;
			check_down_item += 1;
			downgrade_item_cnt--;
			snake.pop_back();
		}
		break;
	}
}

void set_UpgradeItem()
{
	int x, y;
	srand((unsigned int)time(NULL));
	while (true)
	{
		x = rand() % (MAP_SIZE - 2) + 1; //0, 21제외
		y = rand() % (MAP_SIZE - 2) + 1;
		if (!Map[x][y])
		{
			Map[x][y] = UPITEM;
			upgrade_item.x = x;
			upgrade_item.y = y;
			upgrade_item_cnt++;
			leftover_time_up = 50;
			break;
		}
	}
}

void set_DowngradeItem()
{
	int x, y;
	srand((unsigned int)time(NULL));
	while (true)
	{
		x = rand() % (MAP_SIZE - 2) + 1;
		y = rand() % (MAP_SIZE - 2) + 1;
		if (!Map[x][y])
		{
			Map[x][y] = DOWNITEM;
			downgrade_item.x = x;
			downgrade_item.y = y;
			downgrade_item_cnt++;
			leftover_time_down = 50;
			break;
		}
	}
}

// make Gate
void set_Gate()
{

	int x_1, y_1, x_2, y_2;
	srand((unsigned int)time(NULL)); //난수 seed
	while (true)
	{
		x_1 = rand() % (MAP_SIZE + 1);  //난수
		y_1 = rand() % (MAP_SIZE + 1);

		if (Map[x_1][y_1] == WALL)
		{
			Map[x_1][y_1] = GATE;
			first_gate_position.x = x_1;
			first_gate_position.y = y_1;
			leftover_time_gate = 50;
			break;
		}
	}

	srand((unsigned int)time(NULL));
	while (true)
	{
		x_2 = rand() % (MAP_SIZE + 1);
		y_2 = rand() % (MAP_SIZE + 1);
		if (Map[x_2][y_2] == WALL && Map[x_2][y_2] != GATE && !(x_1 == x_2 && y_1 == y_2))
		{
			Map[x_2][y_2] = GATE;
			second_gate_position.x = x_2;
			second_gate_position.y = y_2;
			gate_cnt++;
			break;
		}
	}
}


void set_goal()
{
	srand((unsigned)time(NULL) + 999);

	now_body_len = (rand() % 4) + 5;
	upgrade_now_cnt = (rand() % 3) + 3;
	downgrade_now_cnt = (rand() % 3) + 2;
	gate_pass_cnt = (rand() % 3) + 1;
}