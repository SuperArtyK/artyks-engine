#pragma once

#ifndef SCREEN
#define SCREEN
#include "include.hpp"
using namespace std;

void setcursor(int x, int y)
{//sets the output cursor to x/y pos of the screen
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void setScreenSize(int x, int y) {//sets console size, should be called again when changing font
//as it changes the screen size too :(

	COORD coord;
	coord.X = x;
	coord.Y = y;

	_SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = x - 1;
	Rect.Right = y - 1;

	HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(Handle, coord);
	SetConsoleWindowInfo(Handle, TRUE, &Rect);


}

void settitle(string title) {

	string name = title;
	LPCSTR lname = name.c_str();
	SetConsoleTitleA(lname);

}

void setFont(int x, int y, bool b_issquare, string fontname) {
	//

}


#endif