//--- Console Helper ---///

#ifndef CONSOLE_UTILS_H
#define CONSOLE_UTILS_H

#include <windows.h>

class ConsoleUtils
{
public: 
	//Hides the blinking underscore cursor
	static void hideCursor() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = FALSE;
		SetConsoleCursorInfo(hConsole, &cursorInfo);
	}

	//Shows the cursor (for the Menu and TicTacToe Game)
	static void showCursor() {
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(hConsole, &cursorInfo);
		cursorInfo.bVisible = TRUE;
		SetConsoleCursorInfo(hConsole, &cursorInfo);
	}

	//Moves the "drawing pen" to a specific X, Y coordinate
	static void gotoxy(int x, int y) {
		COORD coord;
		coord.X = (SHORT)x;
		coord.Y = (SHORT)y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
	}
};

#endif

