/*
	ArtyK's Console (Game) Engine. Console engine for apps and games
	Copyright (C) 2021  Artemii Kozhemiak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//////////////////////////////////////////////////////////////////////////
// this file contains the set of funcs(and vars) that work with console window
// will not be used here much
// for backwards compatibility and not complicated tasks
// which dont require much of complexity and speed
// like ''realtime'' console logging(we can skip one or two log entries)
// should not cause everything to break
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef SCREEN
#define SCREEN
#include "include.hpp"
#include "global_vars.hpp"
#include "global_functions.hpp"
using namespace std;



 

class Screen {//screen class for comfortability
public:
	//constructors
	Screen() { 
		m_hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
		m_hStdIn = GetStdHandle(STD_INPUT_HANDLE);
		m_console = GetConsoleWindow();
		interruptscroll = false;
		m_cfi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		m_rgb_color.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		m_normal_color.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
		GetConsoleScreenBufferInfoEx(m_hStdOut, &m_normal_color);
		GetCurrentConsoleFontEx(m_hStdOut, FALSE, &m_cfi);
		GetConsoleScreenBufferInfoEx(m_hStdOut, &m_rgb_color);
		GetConsoleScreenBufferInfoEx(m_hStdOut, &m_normal_color);

	
	};
	~Screen() { };

	//definitions
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
		ZeroMemory(&m_cfi, sizeof(m_cfi));

	}


	void settitle(string title) {//speaks for itself
		//sets app title
		string name = title;
		LPCSTR lname = name.c_str();
		SetConsoleTitleA(lname);

	}

	void scrolltitle(const string title, int delay, int scrolloffset) {
		//scrolls title, doesnt really work, so dont use it
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


	void setFont(int x, int y, bool b_issquare = false, string fontname = "Lucida Console") {//sets font size
		ZeroMemory(&m_cfi, sizeof(m_cfi));
		m_cfi.cbSize = sizeof(m_cfi);
		m_cfi.nFont = 0;
		m_cfi.dwFontSize.X = x;                   // Width of each character in the font
		m_cfi.dwFontSize.Y = y;                  // Height
		if (b_issquare) {
			m_cfi.dwFontSize.Y = x;
		}
		m_cfi.FontFamily = FF_DONTCARE;
		m_cfi.FontWeight = FW_NORMAL;
		std::wcscpy(m_cfi.FaceName, artyk::widen(fontname).c_str()); // Choose your font
		SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &m_cfi);
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


	void setcolor_con(int back, int fore) {

		SetConsoleScreenBufferInfoEx(m_hStdOut, &m_normal_color);


		if (back < 1) {
			back = 0;
		}
		if (fore < 1) {
			fore = 0;
		}
		int tcolor = 0;
		tcolor = back * 16;
		tcolor += fore;
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), tcolor);

	}
	void setcolor_rgb(uint_fast8_t fred, uint_fast8_t fgreen, uint_fast8_t fblue) {//doesnt really work, if you'll fork this engine, plz find a way to fix this
		
//  fred, foreground red
//  fgreen, foreground green
//  fblue, foreground blue
//  bred = 0, backgound red
//  bgreen = 0, backgound green
//  bblue = 0 backgound blue
		
		m_rgb_color.ColorTable[1] = RGB(fred, fgreen, fblue);

		SetConsoleScreenBufferInfoEx(m_hStdOut, &m_rgb_color);
		SetConsoleTextAttribute(m_hStdOut, 1);
	}
	
	std::string getmodulename() { return m_modulename; }
private:


	
	std::string m_modulename = "Screen";
	bool interruptscroll;
	CONSOLE_SCREEN_BUFFER_INFOEX m_rgb_color;
	CONSOLE_SCREEN_BUFFER_INFOEX m_normal_color;

	CONSOLE_FONT_INFOEX m_cfi;
	CONSOLE_CURSOR_INFO m_ci;
	CONSOLE_SCREEN_BUFFER_INFO m_csbi;
	HANDLE m_hStdOut;
	HANDLE m_hStdIn;
	HWND m_console;



};







#endif