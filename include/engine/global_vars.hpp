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

/** @file include/engine/global_vars.hpp
 *  This file contains the global variables that can be used everywhere.  
 * 
 *  Should not cause everything to break.  
 */

#pragma once


#ifndef GLOB_VARS_HPP
#define GLOB_VARS_HPP

#include <string>
#include <Windows.h>
#include "engine_flags.hpp"
using std::string;
using std::to_string;

#ifndef _DEBUG
#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0
#endif




//engine stuff, DO NOT TOUCH

//START OF ENGINE DEFINES

///the inner game maximum fps, won't be related to graphics fps.
///Limits the amount of cycles per second of the game loops and game-related stuff(like keyboard scanning).
#define GAME_FPS 66
#ifdef GAME_FPS

#if (GAME_FPS <1)
//std::cout << "Warning, GAME_FPS is 1 or less! Game might break!\n";
#pragma message ("Warning, GAME_FPS is 1 or less! Game might break!")

#endif


#ifndef GAME_FPS
#error Warning, GAME_FPS is not defined! Always define it, as it's a part of an engine logic and delay handling
#endif // !GAME_FPS
#endif

///amount of global timer ticks to wait for, untill launching the threads.
///used that global or modules created at startup, start their threads at ~~the same time.
#define TIMER_WAIT 2*GAME_FPS //wait until timer time is 2xGAME_FPS ticks(2 world-seconds)

///default modulename for engine
#define DEF_MNAME "Engine"

//END OF ENGINE DEFINES


///converts the "string literal" to u8"utf-8/unicode string literal"
#define UTF8(x) u8 ## x //converts the "string" to the u8"string"
///converts the "string literal" to L"wide string literal"
#define WSTR(x) L ## x //converts the "string" to the L"string"
///macro for newline char
#define NLC '\n'
///macro for newline string
#define NLS "\n"



//im making this, so I can differentiate between std, mine, and other namespaces
//and have "intuitive" positions of things, that screen stuff is in screen subns, for example
///This namespace contains all of things that any of the part of program can use, and is useful.
///Such things are -- handles to windows, app names, status(we're starter or closing), flags, functions, etc.
namespace artyk {

	///flag if app is being closed, 1 if it user closes app, more -- modules are turning off
	///still unsure how to implement that tho
	inline uint_fast8_t closing_app = 0;

	///name of the app
	const string app_name = 
#ifdef AE_DEBUG
		
		string("(DEBUG)") + 
#endif // AE_DEBUG
		"ArtyK's Console Engine Test";

	///build of app, how do you autoincrement these?
	constexpr int app_build = 3635;
	///version
	const string app_version = "v.0.0.6";
	///full name of the app
	const string app_name_full = app_name + " " + app_version + ":"+to_string(app_build);

	///status of starting up
	inline short app_startstatus = 0;

	const HANDLE g_output_handle = GetStdHandle(STD_OUTPUT_HANDLE);
	const HANDLE g_input_handle = GetStdHandle(STD_INPUT_HANDLE);
	const HWND g_console_hwnd = GetConsoleWindow();

	///Screen colors for graphic engine and AEScreen.
	///Prefix 'D' means "Dark" and 'B' meand "Bright".
	///Exceptions are black gray and white
	namespace color {

		//prefix 'D' means "Dark" and 'B' meand "Bright"
		//exceptions are black gray and white
		constexpr unsigned char 

			BLACK = 0,
			D_BLUE = 1,  //DARK
			D_GREEN = 2,  //DARK
			D_CYAN = 3,  //DARK
			D_RED = 4,  //DARK
			D_MAGENTA = 5,  //DARK
			D_YELLOW = 6,  //DARK
			D_WHITE = 7,  //DARK
			GRAY = 8,
			B_BLUE = 9,  //BRIGHT
			B_GREEN = 10, //BRIGHT
			B_CYAN = 11, //BRIGHT
			B_RED = 12, //BRIGHT
			B_PURPLE = 13, //BRIGHT
			B_YELLOW = 14, //BRIGHT
			WHITE = 15, 

			//default colors
			DEF_BGR = BLACK,
			DEF_FGR = B_GREEN,
			DEF_ATTR = DEF_BGR*16+DEF_FGR;
			

	}
	///list of the engine exit codes
	namespace exitcodes {

		constexpr char
			NORMAL_EXIT = 0,
			ERROR_EXIT = 1,
			FORCED_EXIT = 2,
			BAD_SCREEN_HANDLE = 3,
			ERROR_STARTING_TIMER = 4,
			ERROR_STARTING_KEYBOARD = 5,
			INIT_KB_ERROR = 6,
			INIT_SC_ERROR = 7,
			ERROR_SETTING_SCREEN = 8
			;


	}

	///\brief this namespace contains predone characters/char cells used for drawing stuff with AEGraphic
	namespace gfx {
		///characters
		constexpr wchar_t
			CH_EMPTY = L'\0',//no difference in visual, but a difference in processing, used this to indicate empty/transparent stuff
			CH_SPACE = L' ',
			CH_25 = L'░',
			CH_50 = L'▒',
			CH_75 = L'▓',
			CH_100 = L'█',
			CH_BLOCK = CH_100,
			CH_BOX1[]{
				L'┌', L'┬', L'┐',
				L'┤', L'┘', L'┴',
				L'└', L'├', L'┼',
				L'─', L'│',
		},
		CH_BOX2[]{
			L'╒', L'╤', L'╕',
			L'╡', L'╛', L'╧',
			L'╘', L'╞', L'╪',
			L'═', L'│',
		},
		CH_BOX3[]{
			L'╓', L'╥', L'╖',
			L'╢', L'╜', L'╨',
			L'╙', L'╟', L'╫',
			L'─', L'║',
		},
		CH_BOX4[]{
			L'╔', L'╦', L'╗', L'╣', L'╝', L'╩', L'╚', L'╠', L'╬', L'═', L'║',
		};

		//char cells
		constexpr CHAR_INFO
			PX_EMPTY = { L'\0', 0 },
			PX_SPACE = { L' ', artyk::color::DEF_ATTR },//no difference in visual, but a difference in processing, used this to indicate empty/transparent stuff
			PX_25 = { L'░', artyk::color::DEF_ATTR },
			PX_50 = { L'▒', artyk::color::DEF_ATTR },
			PX_75 = { L'▓', artyk::color::DEF_ATTR },
			PX_100 = { L'█', artyk::color::DEF_ATTR },
			PX_BLOCK = PX_100,
			PX_BOX1[]{
				{L'┌', artyk::color::DEF_ATTR},
				{L'┬', artyk::color::DEF_ATTR},
				{L'┐', artyk::color::DEF_ATTR},
				{L'┤', artyk::color::DEF_ATTR},
				{L'┘', artyk::color::DEF_ATTR},
				{L'┴', artyk::color::DEF_ATTR},
				{L'└', artyk::color::DEF_ATTR},
				{L'├', artyk::color::DEF_ATTR},
				{L'┼', artyk::color::DEF_ATTR},
				{L'─', artyk::color::DEF_ATTR},
				{L'│', artyk::color::DEF_ATTR},
		},
		PX_BOX2[]{
			{L'╒', artyk::color::DEF_ATTR},
			{L'╤', artyk::color::DEF_ATTR},
			{L'╕', artyk::color::DEF_ATTR},
			{L'╡', artyk::color::DEF_ATTR},
			{L'╛', artyk::color::DEF_ATTR},
			{L'╧', artyk::color::DEF_ATTR},
			{L'╘', artyk::color::DEF_ATTR},
			{L'╞', artyk::color::DEF_ATTR},
			{L'╪', artyk::color::DEF_ATTR},
			{L'═', artyk::color::DEF_ATTR},
			{L'│', artyk::color::DEF_ATTR},
		},
		PX_BOX3[]{
			{L'╓', artyk::color::DEF_ATTR},
			{L'╥', artyk::color::DEF_ATTR},
			{L'╖', artyk::color::DEF_ATTR},
			{L'╢', artyk::color::DEF_ATTR},
			{L'╜', artyk::color::DEF_ATTR},
			{L'╨', artyk::color::DEF_ATTR},
			{L'╙', artyk::color::DEF_ATTR},
			{L'╟', artyk::color::DEF_ATTR},
			{L'╫', artyk::color::DEF_ATTR},
			{L'─', artyk::color::DEF_ATTR},
			{L'║', artyk::color::DEF_ATTR},
		},
		PX_BOX4[]{
			{L'╔', artyk::color::DEF_ATTR},
			{L'╦', artyk::color::DEF_ATTR},
			{L'╗', artyk::color::DEF_ATTR},
			{L'╣', artyk::color::DEF_ATTR},
			{L'╝', artyk::color::DEF_ATTR},
			{L'╩', artyk::color::DEF_ATTR},
			{L'╚', artyk::color::DEF_ATTR},
			{L'╠', artyk::color::DEF_ATTR},
			{L'╬', artyk::color::DEF_ATTR},
			{L'═', artyk::color::DEF_ATTR},
			{L'║', artyk::color::DEF_ATTR},
		};


	}



}



#endif // !GLOB_VARS_HPP


