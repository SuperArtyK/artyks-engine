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
#include "global_vars.hpp"
#include "global_functions.hpp"
#include <windows.h>
using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;

class Screen {//screen class for comfortability
public:
	//constructors
	Screen();
	~Screen();

	//definitions
	void setcursor(short x, short y);//sets the output cursor to x/y pos of the screen

	void setConsoleBufferSize(unsigned short y, unsigned short x);//sets console size

	void setScreenSize(unsigned short x, unsigned short y);//sets console size



	void settitle_val(const string title);	//speaks for itself

	void settitle_ref(const string& title);//speaks for itself
	
	void scrolltitle(const string title, short delay, short scrolloffset);//scrolls title
	
	void setFont(int x, int y, bool b_issquare = false, string fontname = "Lucida Console");//sets font size
	
	void clear(void);//clears screen; much faster than system() but still slow for "draw frame, clear, draw again" scheme

	void setcolor_con(uint_fast8_t  back, uint_fast8_t  fore);//sets color of text out of 16 color pallete

	void setcolor_rgb(uint_fast8_t fred, uint_fast8_t fgreen, uint_fast8_t fblue);//doesnt really work, if you'll fork this engine, plz find a way to fix this
	
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
	COORD concoord;
	RECT m_r;
	const COORD topLeft = { 0, 0 };

};







#endif