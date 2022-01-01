#include <stdio.h>

#ifndef WIN32

typedef enum FG_CONSOLE_COLOR {
	FG_COL_BLACK = 30,
	FG_COL_RED,
	FG_COL_GREEN,
	FG_COL_YELLOW,
	FG_COL_BLUE,
	FG_COL_MAGENTA,
	FG_COL_CYAN,
	FG_COL_WHITE
} FG_CONSOLE_COLOR;

typedef enum BG_CONSOLE_COLOR {
	BG_COL_BLACK = 40,
	BG_COL_RED,
	BG_COL_GREEN,
	BG_COL_YELLOW,
	BG_COL_BLUE,
	BG_COL_MAGENTA,
	BG_COL_CYAN,
	BG_COL_WHITE
} BG_CONSOLE_COLOR;

int _bg_color = -1;
int _fg_color = -1;
char _str_color[10] = "";
int _bold = 0;

void __color();
void _make_str_color();
void reset_console_colors();
void fg_color(FG_CONSOLE_COLOR col);
void bg_color(BG_CONSOLE_COLOR col);
void set_bold(int b);
void cursor_to(int x, int y);
void clear_screen();

void cursor_to(int x, int y)
{
	printf("\033[%d;%df", x, y);
}

void clear_screen()
{
	printf("\033[2J\033[1;1H");
}

void _make_str_color()
{
	int bc = _bg_color;
	int fc = _fg_color;
	char p[10] = "";
	if (fc == -1)
		sprintf(_str_color, "\033[0m");
	else if (bc == -1)
		sprintf(_str_color, "\033[%d;%dm", _bold, fc);
	else
		sprintf(_str_color, "\033[%d;%d;%dm", _bold, fc, bc);
	__color();
}

void set_default_colors()
{
	_bg_color = -1;
	_fg_color = -1;
	_bold = 0;
	_make_str_color();
}

void fg_color(FG_CONSOLE_COLOR col)
{
	_fg_color = col >= FG_COL_BLACK && col <= FG_COL_WHITE ? col : -1;
	_make_str_color();
}

void bg_color(BG_CONSOLE_COLOR col)
{
	_bg_color = col >= BG_COL_BLACK && col <= BG_COL_WHITE ? col : -1;
	_make_str_color();
}

void set_bold(int b)
{
	_bold = b == 1 ? 1 : 0;
	_make_str_color();
}

void reset_console_colors()
{
	_fg_color = -1;
	_bg_color = -1;
	_bold = 0;
	_make_str_color();
}

void __color()
{
	printf("%s", _str_color);
}

#else //WIN32

#include <windows.h>

#define BG_COL_BLACK 0
#define BG_COL_BLUE  1
#define BG_COL_GREEN 2
#define BG_COL_CYAN  3
#define BG_COL_RED   4
#define BG_COL_MAGENTA   5
#define BG_COL_BROWN     6
#define BG_COL_LIGHT_GRAY 7
#define BG_COL_DARK_GRAY  8
#define BG_COL_LIGHT_BLUE 9
#define BG_COL_LIGHT_GREEN 10
#define BG_COL_LIGHT_CYAN  11
#define BG_COL_LIGHT_RED   12
#define BG_COL_LIGHT_MAGENTA 13
#define BG_COL_YELLOW 14
#define BG_COL_WHITE 15

#define FG_COL_BLACK 0
#define FG_COL_BLUE  1
#define FG_COL_GREEN 2
#define FG_COL_CYAN  3
#define FG_COL_RED   4
#define FG_COL_MAGENTA   5
#define FG_COL_BROWN     6
#define FG_COL_LIGHT_GRAY 7
#define FG_COL_DARK_GRAY  8
#define FG_COL_LIGHT_BLUE 9
#define FG_COL_LIGHT_GREEN 10
#define FG_COL_LIGHT_CYAN  11
#define FG_COL_LIGHT_RED   12
#define FG_COL_LIGHT_MAGENTA 13
#define FG_COL_YELLOW 14
#define FG_COL_WHITE 15

void fg_color(WORD colore);
void bg_color(WORD colore);
void cursor_to(int x, int y);
void clear_screen();
void reset_console_colors();
void set_bold(int b);

void reset_console_colors(){}
void set_bold(int b){}

void cursor_to(int x, int y)
{
	HANDLE OutputH;
	COORD posizione = {x - 1, y - 1};
	OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(OutputH, posizione);
}

void fg_color(WORD colore)
{
	HANDLE OutputH;
	CONSOLE_SCREEN_BUFFER_INFO info;
	short int attributi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	attributi = info.wAttributes;
	attributi = attributi & 240;
	OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(OutputH, colore | attributi);
}

void bg_color(WORD colore)
{
	HANDLE OutputH;
	CONSOLE_SCREEN_BUFFER_INFO info;
	short int attributi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &info);
	attributi = info.wAttributes;
	attributi = attributi & 15;
	OutputH = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(OutputH, colore * 16 | attributi);
}

void clear_screen()
{
	cursor_to(1, 1);
	int j = 0;
	for (j = 1; j <= 90; j++)
		printf("                                                                                ");
	cursor_to(1,1);
}
#endif
