#include <iostream>
#include <chrono>
#include <thread>
#include "console.h"

typedef unsigned int uint;

#define WORLD_X 32
#define WORLD_Y 28
#define ALIVE_CHAR "#"
#define DEAD_CHAR "."
#define change_color(cell) (cell ? fg_color(FG_COL_YELLOW) : fg_color(FG_COL_BLUE))
#define draw_cell(cell) (cell ? ALIVE_CHAR : DEAD_CHAR)
#define check_cell(matrix, x, y) (x < 0 || x >= WORLD_X || y < 0 || y >= WORLD_Y ? C_NONE : (matrix[x][y] ? C_ALIVE : C_DEAD))

enum CELL_T {
	C_NONE, C_ALIVE, C_DEAD
};

bool blink[7][7] = {
		{0, 0, 0, 1, 0, 0, 0},
		{0, 0, 1, 1, 1, 0, 0},
		{0, 1, 0, 1, 0, 1, 0},
		{1, 1, 1, 0, 1, 1, 1},
		{0, 1, 0, 1, 0, 1, 0},
		{0, 0, 1, 1, 1, 0, 0},
		{0, 0, 0, 1, 0, 0, 0}
};

bool LWSS[7][7] = {
		{0, 1, 0, 0, 1, 0, 0},
		{1, 0, 0, 0, 0, 0, 0},
		{1, 0, 0, 0, 1, 0, 0},
		{1, 1, 1, 1, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
};

bool glider[7][7] = {
		{1, 1, 1, 0, 0, 0, 0},
		{1, 0, 0, 0, 0, 0, 0},
		{0, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
};

bool block[7][7] = {
		{1, 1, 0, 0, 0, 0, 0},
		{1, 1, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0},
		{0, 0, 0, 0, 0, 0, 0}
};

bool current[WORLD_X][WORLD_Y];
bool next[WORLD_X][WORLD_Y];

void initMatrix(bool matrix[][WORLD_Y], bool value);
void copyMatrix(bool matrix[][WORLD_Y], bool target[][WORLD_Y]);
void drawMatrix(bool matrix[][WORLD_Y]);
void generation(void);
void update(void);
CELL_T checkAdjacent(bool matrix[][WORLD_Y], uint x, uint y);
void incorporate(bool part[7][7], bool matrix[][WORLD_Y], uint ix, uint iy);

int main(int argc, char** argv)
{
	initMatrix(current, false);
	initMatrix(next, false);
	incorporate(LWSS, current, 22, 23);
	incorporate(LWSS, current, 4, 18);
	incorporate(glider, current, 20, 15);
	incorporate(glider, current, 23, 8);
	incorporate(blink, current, 6, 6);
	incorporate(blink, current, 8, 23);
	incorporate(block, current, 16, 10);
	incorporate(block, current, 10, 0);
	incorporate(block, current, 15, 3);
	incorporate(block, current, 19, 5);
	incorporate(block, current, 26, 2);
	update();
	return 0;
}

//UNSAFE: Boundaries are not checked - JUST FOR TESTING
void incorporate(bool part[7][7], bool matrix[][WORLD_Y], uint ix, uint iy)
{
	for (uint x = ix; x < ix + 7; x++)
		for (uint y = iy; y < iy + 7; y++)
			matrix[x][y] = part[x - ix][y - iy];
}

void generation(void)
{
	CELL_T cell_t = C_NONE;
	for (uint x = 0; x < WORLD_X; x++)
	{
		for (uint y = 0; y < WORLD_Y; y++)
		{
			cell_t = checkAdjacent(current, x, y);
			if (cell_t == C_ALIVE)
				next[x][y] = true;
			else if (cell_t == C_DEAD)
				next[x][y] = false;
		}
	}
	copyMatrix(next, current);
}


CELL_T checkAdjacent(bool matrix[][WORLD_Y], uint x, uint y)
{
	CELL_T cell_t = C_NONE;
	uint alive_count = 0;
	bool cell = matrix[x][y];
	for (int _x = -1; _x < 2; _x++)
		for (int _y = -1; _y < 2; _y++)
		{
			if (_x == 0 && _y == 0) continue;
			cell_t = check_cell(matrix, x + _x, y + _y);
			if (cell_t == C_NONE) continue;
			if (cell_t == C_ALIVE) alive_count++;
		}
	return (cell ? (alive_count == 2 || alive_count == 3 ? C_ALIVE : C_DEAD) : (alive_count == 3 ? C_ALIVE : C_DEAD));
}

void update(void)
{
	while (true)
	{
		drawMatrix(current);
		system("pause");
		generation();
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
		clear_screen();
	}
}

void initMatrix(bool matrix[][WORLD_Y], bool value)
{
	for (uint x = 0; x < WORLD_X; x++)
		for (uint y = 0; y < WORLD_Y; y++)
			matrix[x][y] = value;
}

void copyMatrix(bool matrix[][WORLD_Y], bool target[][WORLD_Y])
{
	for (uint x = 0; x < WORLD_X; x++)
		for (uint y = 0; y < WORLD_Y; y++)
			target[x][y] = matrix[x][y];
}

void drawMatrix(bool matrix[][WORLD_Y])
{
	for (uint x = 0; x < WORLD_X; x++)
	{
		for (uint y = 0; y < WORLD_Y; y++)
		{
			change_color(matrix[x][y]);
			std::cout << draw_cell(matrix[x][y]) << " ";
		}
		std::cout << "\n";
	}
}
