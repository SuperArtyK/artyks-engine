#pragma once

#ifndef SCREEN
#define SCREEN
#include "include.hpp"
using namespace std;

//var name cheatsheet
/*
<scope>_<datatype><Name>

EX:
m_bDev          |g_flgDeflogger|m_strMessg|l_strPathtolog
m:member;b:bool;|g:global;     |vec:vector|l:local

b bool
i int
c char
ll long long (int)
ull unsigned long long (int)/uint64_t
ld long double
trd thread
fst fstream
cst const
flg filelog
arr array
vec vector
ptr pointer
lpvd lpvoid


etc..
*/


//set of funcs(and vars) that work with console window
//will not be really used here
//for backwards compatibility
//and easy window control(aka, I just need to set the window to this size/title)
bool interruptscroll;
CONSOLE_FONT_INFOEX cfi;
CONSOLE_CURSOR_INFO ci;
CONSOLE_SCREEN_BUFFER_INFO csbi;
HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
HWND console = GetConsoleWindow();



void setcursor(int x, int y);
void setScreenSize(int x, int y);
void settitle(string title);
void setFont(int x, int y, bool b_issquare, string fontname);
void scrolltitle(const string title, int delay = 50, int scrolloffset = 1);
void clear(void);
void setcolor(int back, int fore);

void setcursor(int x, int y)
{//sets the output cursor to x/y pos of the screen
	static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	std::cout.flush();
	COORD coord = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(hOut, coord);
}

void setScreenSize(int y, int x) {//sets console size
//units are character cells

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


void setFont(int x, int y, bool b_issquare = false, string fontname = "") {
ZeroMemory(&cfi, sizeof(cfi));
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = x;                   // Width of each character in the font
	cfi.dwFontSize.Y = y;                  // Height
	if (b_issquare) {
		cfi.dwFontSize.Y = x;
	}
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	std::wcscpy(cfi.FaceName, L"Lucida Console"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}
void clear(void) {
	COORD topLeft = { 0, 0 };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(console, &screen);
	FillConsoleOutputCharacterA(
		console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		console, FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE,
		screen.dwSize.X * screen.dwSize.Y, topLeft, &written
	);
	setcursor(topLeft.X, topLeft.Y);
}


void setcolor(int back, int fore) {
	int tcolor = 0;
	tcolor = back * 16;
	tcolor += fore;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tcolor);

}
#endif