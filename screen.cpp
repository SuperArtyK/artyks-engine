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
// this file contains the screen functions definitions
// should not cause everything to break
//////////////////////////////////////////////////////////////////////////

#include "screen.hpp"
using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;	

//constructors/destructors
Screen::Screen() {
	concoord = { (short)0, (short)0 };
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
	GetConsoleScreenBufferInfo(m_hStdOut, &m_csbi);
	
}

Screen::~Screen() { }

//other stuff

void Screen::setcursor(short x, short y)
{//sets the output cursor to x/y pos of the screen
	std::cout.flush();
	concoord = { x, y };
	SetConsoleCursorPosition(m_hStdOut, concoord);
}

void Screen::setConsoleBufferSize(unsigned short x, unsigned short y) {//sets console size
	//units are character cells

	COORD coord;
	coord.X = x;
	coord.Y = y;

	SMALL_RECT Rect;
	Rect.Top = 0;
	Rect.Left = 0;
	Rect.Bottom = x - 1;
	Rect.Right = y - 1;

	//HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleScreenBufferSize(m_hStdOut, coord);
	//SetConsoleWindowInfo(m_hStdOut, TRUE, &Rect);

}

void Screen::setScreenSize(unsigned short x, unsigned short y)
{
	GetWindowRect(m_console, &m_r);
	MoveWindow(m_console, m_r.left, m_r.top, x, y, TRUE);

}





void Screen::settitle_val(const string title) {//speaks for itself
		//sets app title
		//passed by value
	SetConsoleTitleA(title.c_str());

}
void Screen::settitle_ref(const string& title) {//speaks for itself
		//sets app title
		//passed by reference, a little faster because of that
	SetConsoleTitleA(title.c_str());

}

void Screen::scrolltitle(const string title, short delay, short scrolloffset) {
	//scrolls title, doesnt really work right now, so dont use it
	//FIXME:Remove thread blocking
	if (title.empty()) {
		return;
	}
	interruptscroll = false;
	string temp = title;
	int i = 0;
	while (!interruptscroll) {
		if (i >= temp.length()) {
			i = 0;
		}
		temp.erase(0, 1);
		temp += title[i];
		settitle_val(temp);
		Sleep(delay);
		i++;
	}


}

void Screen::setFont(int x, int y, bool b_issquare, string fontname) {//sets font size
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
	wcscpy_s(m_cfi.FaceName, artyk::widen(fontname).c_str()); // Choose your font
	SetCurrentConsoleFontEx(m_hStdOut, FALSE, &m_cfi);
}

void Screen::clear(void) {//clears screen; much faster than system() but still slow for "draw frame, clear, draw again" scheme
	COORD topLeft = { 0, 0 };
	//HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	//CONSOLE_SCREEN_BUFFER_INFO screen;
	DWORD written;

	GetConsoleScreenBufferInfo(m_hStdOut, &m_csbi);
	FillConsoleOutputCharacterA(
		m_hStdOut, ' ', m_csbi.dwSize.X * m_csbi.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		m_hStdOut, BLACK,
		m_csbi.dwSize.X * m_csbi.dwSize.Y, topLeft, &written
	);
	setcursor(topLeft.X, topLeft.Y);

}

void Screen::setcolor_con(uint_fast8_t  back, uint_fast8_t  fore) {//sets color of text out of 16 color pallete

	SetConsoleScreenBufferInfoEx(m_hStdOut, &m_normal_color); // this is for setcolor_rgb() but completely breaks the pallete

	int tcolor = 0;
	tcolor = back * 16 + fore;
	SetConsoleTextAttribute(m_hStdOut, tcolor);

}

void Screen::setcolor_rgb(uint_fast8_t fred, uint_fast8_t fgreen, uint_fast8_t fblue) {//doesnt really work, if you'll fork this engine, plz find a way to fix this

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