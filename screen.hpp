#pragma once

#ifndef SCREEN
#define SCREEN
#include "include.hpp"
using namespace std;

//set of funcs(and vars) that work with console window

bool interruptscroll;

void setcursor(int x, int y);
void setScreenSize(int x, int y);
void settitle(string title);
void setFont(int x, int y, bool b_issquare, string fontname);
void scrolltitle(const string title, int delay = 100, int scrolloffset = 1);
void tmpfunc(const string title, int delay = 100, int scrolloffset = 1);

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

void scrolltitle(const string title, int delay, int scrolloffset) {
	//FIXME:Remove thread blocking
	interruptscroll = false;
	string temp = title;
	int i = 0;
	while (!interruptscroll) {
		if (i >= temp.length()) {
			i = 0;
		}
		temp.erase(0, 1);
		temp += title[i];
		settitle(temp);
		Sleep(delay);
		i++;
	}


}
// void tmpfunc(const string title, int delay, int scrolloffset) {
// 	string temp = title;
// 	int i = 0;
// 	while (!interruptscroll) {
// 		if (i >= temp.length()) {
// 			i = 0;
// 		}
// 		temp.erase(0, 1);
// 		temp += title[i];
// 		settitle(temp);
// 		Sleep(delay);
// 		i++;
//  	}
// }
void setFont(int x, int y, bool b_issquare, string fontname) {
	//

}


#endif