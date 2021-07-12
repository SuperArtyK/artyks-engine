/*
	ArtyK's Console (Game) Engine. Console engine for apps and games
	Copyright (C) 2021  Artemii Kozhemiak

	https://github.com/SuperArtyK/artyks-engine

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License 3.0 as published
	by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License 3.0 for more details.

	You should have received a copy of the GNU General Public License 3.0
	along with this program.  If not, see <https://www.gnu.org/licenses/gpl-3.0.txt>.
*/

/** @file include/AEScreen.hpp
 *  This file contains the declaration of engine's simple screen manager, aka AEScreen
 *  Good if you don't need the power of graphics engine.  
 *  
 *  Should not cause everything to break.  
 */

#pragma once

#ifndef SCREEN_HPP
#define SCREEN_HPP
#include "AELog.hpp"
#include "global_vars.hpp"
#include "func_utils.hpp"
#include "AEBaseClass.hpp"
#include <Windows.h>
using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;



/// \brief This module is made for simple screen management, and not made for the performance.  
/// Good if you don't need the power of graphics engine, that will be added in v0.0.7.  
/// Doesn't create any threads, purely single-threaded.  
class AEScreen: public __AEBaseClass {//screen class for comfortability
public:
	friend int initenginestuff();

	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="enablelog">flag to check if we need to log stuff at all</param>
	/// <param name="useGlobLog">flag to check if to use gloabal_logger or not(if it exists)</param>
	AEScreen(bool enablelog = true, bool useGlobLog = false);
	~AEScreen() override;

	/// sets the output cursor to the given x/y pos of the screen
	void setcursor(short x, short y);

	/// <summary>
	/// sets the console screen to given dimensions if possible, otherwise throws Fatal error
	/// </summary>
	/// <param name="swidth">screen width, in console character cells</param>
	/// <param name="sheight">screen height, in console character cells</param>
	/// <param name="fonth">character font height, in pixels, doesnt't go less than 2</param>
	/// <param name="fontw">character font width, in pixels, doesnt't go less than 2</param>
	void setScreen(short swidth = 128, short sheight = 128, short fonth = 14, short fontw = 7);//sets console size

	///sets console buffer size to the dimensions given, units are character cells
	void setConsoleBufferSize(short x, short y);
	///sets the app title
	///@note if the AE_ADD_APP_TITLE flag is defined, it will append artyk::app_name_full to  
	void settitle(const string& title);//sets app title
	
	
	/// <summary>
	/// sets the font setting to provided ones
	/// </summary>
	/// <param name="y">height of the font</param>
	/// <param name="x">width of the font</param>
	/// <param name="fontname">font name</param>
	void setFont(short y, short x, const string& fontname = "Consolas");//sets font size
	
	///clears the console screen; faster than using system(cls), but still slow for "draw frame, clear, draw again" scheme
	void clear(void);

	///sets the color of the future text(with printf/cout) out of 16color pallete. Colors are in the artyk::screen namespace
	void setcolor_con(const smalluint back, const smalluint fore);//sets color of text out of 16 color pallete
	
	/// returns current app title
	/// @bug There's a known bug, where if you use feed GetAppTitle() into settitle() continuously, it will set title to what you previously set it to
	static inline string GetAppTitle() {
		char wnd_title[256];
		GetWindowTextA(artyk::g_console_hwnd, wnd_title, sizeof(wnd_title));
		return wnd_title;
	}

#ifdef AE_EXPERIMENTAL
	///UNDONE!!! **should** scroll the title
	void scrolltitle(const string& title, short delay, short scrolloffset);//scrolls title
	/// <summary>
	/// changes the pallete to the rgb value, broken, dont use
	/// </summary>
	/// <param name="fred">red</param>
	/// <param name="fgreen">green</param>
	/// <param name="fblue">blue</param>
	void setcolor_rgb(smalluint fred, smalluint fgreen, smalluint fblue);//doesnt really work, if you'll fork this engine, plz find a way to fix this
#endif
	
	
#ifdef AE_EXPERIMENTAL
	/// uses all utils for class
	///@see __AEBaseClass::benchmark()
	void benchmark(void) override {
		using namespace artyk::color;
		//setcursor(0, 0);
		setScreen(96, 20, 14, 7);
		setConsoleBufferSize(96, 20);
		setFont(14, 7);
		GetAppTitle();
		setcolor_con(DEF_BGR, DEF_FGR);
		getmodulename();
		getmodulenum();
	}
#endif

	//returns screen size/resolution, in character cells
	static inline COORD GetScreenRes(void) { return AEScreen::g_csbi.dwSize; }

private:
	///checks HANDLE's and HWND's being valid. if some fail the check, engine closes with error message and log entry, otherwise return true;
	bool checkhandles(void);

	///buffer info extended, used in setcolor_rgb() to set the color
	static CONSOLE_SCREEN_BUFFER_INFOEX g_rgb_color;
	///buffer info extended, was used in setcolor() to revert back the pallete to default
	static CONSOLE_SCREEN_BUFFER_INFOEX g_normal_color;
	///console font info extended, used to manage the font of console
	static CONSOLE_FONT_INFOEX g_cfi;
	///console buffer info, used to manage the screen
	static CONSOLE_SCREEN_BUFFER_INFO g_csbi;
	///temp variable to store window sizes
	SMALL_RECT g_rectWindow;
	///variable to store the module index number
	static biguint m_globalmodulenum;
	///our screen resolution
	static COORD m_screenres;
	// variable for the scrolltitle()
	//bool interruptscroll;
	
	
	
	


};




#ifdef AE_GLOBALMODULE
///our global simple screen manager module
inline AEScreen global_screen(true,true);
#endif







#endif // SCREEN_HPP