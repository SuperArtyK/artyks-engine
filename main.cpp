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

/** @file myengine.cpp
 *  This file is the main file of the engine, has main().  
 *  Should not cause everything to break.  
 */

#pragma comment(lib, "winmm.lib")

#if __cplusplus == 199711L
#pragma message("It looks like you didn't set the /Zc:__cplusplus switch, so your __cplusplus will always be \'199711L\' no matter what standard you use\nit's advisable you set it")
#else
#if __cplusplus <= 201702L
#error "C++ is not of standard of 2017 or higher! Set the standard to it!" 
#else
#pragma message("Your c++ standard is c++17 or higher, that's a good sign!")
#endif // __cplusplus <= 201702L
#endif // __cplusplus == 199711L

#ifndef UNICODE
#define UNICODE
#endif // !#define UNICODE


//modules
#include "include.hpp"
#include "typedefs.hpp"
#include "engine_flags.hpp"
#include "global_vars.hpp"

///this function initialises all stuff the engine needs, before it starts.
int initenginestuff();
///dummy variable to initialise engine stuff before main() starts
///evil hack
const int __dummy_dont_use_ever_please = initenginestuff();

#include "func_utils.hpp"
#include "func_system.hpp"
#include "AEBaseClass.hpp"
#include "AEBeep.hpp"
#include "AEKeyboard.hpp"
#include "AELog.hpp"
#include "AEScreen.hpp"
#include "AETimer.hpp"

//global stuff
using namespace std;


#ifdef AE_EXPERIMENTAL
void benchmark() {

__AEBaseClass mybc("BaseClass", 1);
mybc.benchmark();

AEBeep mybp(true,false);
mybp.benchmark();
//
AEKeyboard mykb(true, false);
mykb.benchmark();
//
AELog mylog;
mylog.benchmark();
//
AEScreen myscr(true, false);
myscr.benchmark();
//
AETimer mytime(-1);
mytime.benchmark();
//
global_timer.benchmark();


}
#endif // AE_EXPERIMENTAL



///our "starting point" of the program
int main()
{
	
//your code goes below...
	cout << "Each beep means that, 1+ seconds has passed and we're calculating the results of benchmarking.\nEach \"frame\" is calling benchmark function that uses all of engine's modules\nin a worst way I think possible\n";

	int i = 0;
	int previ = 0;
	auto timestart = std::chrono::system_clock::now();
	auto timeend = std::chrono::system_clock::now();
	float fElapsedTime;
	AEBeep mybp;
	AEScreen myscr;
	int fps = 0, maxfps = fps;
	artyk::utils::Info("I'm going to start the engine's benchmark. \nThis benchmark's goal is to see what performance you can get if to use the engine at it's maximum(e.g. worst case in using it).\nIt may use your processor to maximum, but I don't guarantee that it will do that, as it's not the goal.\nThis benchmark doesn't have an end, and the result is the \"FPS\" you will see in the app title.\nTo close this app, use task manager or your taskbar.", DEF_MNAME, false);
	artyk::utils::Info("Each beep will mean that 1+ seconds has passed, and we're calculating the results of benchmarking.\nEach \"frame\" is calling benchmark function that uses all of engine's modules\nin a worst way I, ArtyK, think possible.\n\nWARNING: This benchmark has excessive flashing imagery.\nPress \"OK\" to start benchmark.", DEF_MNAME,false);
	Sleep(500);
	for (;;) {
		i++;
		benchmark();
	
		timeend = std::chrono::system_clock::now();
		fElapsedTime = std::chrono::duration<float>(timeend - timestart).count();
	
		myscr.settitle("FPS: " + to_string(fps) + " | Time Since start : " + to_string(global_timer.getworldtime()));
		//cout << myscr.GetAppTitle()<<endl;
		if (fElapsedTime > 1.0f) {
			fps = i - previ;
			if (fps > maxfps) maxfps = fps;
			//cout<<"FPS: " + to_string(fps) + " | Time Since start : " + to_string(global_timer.getworldtime()) + " Mouse pos: " + to_string(mykb.GetMouseX())+" "+to_string(mykb.GetMouseY())+'\n';
			previ = i;
			timestart = timeend;
			mybp.makesound_async(660, 200);
		}
		
	
	}

	
	artyk::closing_app = 1;

	if (artyk::closing_app) {
		AELog mylog;
		mylog.writetolog("Closed the \"" + artyk::app_name + "\". Version: " + artyk::app_version + " Build: " + to_string(artyk::app_build), LOG_INFO);
	}
	cout << "\nPress enter to continue . . .\n";
	cin.get();
	cout << "Exiting. . .\n";
	return 0;
}



//function defs, do not touch


int initenginestuff() {
	//changes app_startstatus
	artyk::app_startstatus = 0;
	std::memset(AEKeyboard::m_keyOld, 0, 256 * sizeof(short));
	std::memset(AEKeyboard::m_keys, 0, 256 * sizeof(AEKBKey));

	for (short i = 0; i < 256; i++) {
		AEKeyboard::m_keys[i].m_keyid = i;
		for (short a = 0; a < (sizeof(AEKeyboard::m_keycodes)/sizeof(AEKeyboard::m_keycodes[0])); a++) {
			if (i == AEKeyboard::m_keycodes[a].second) {
				AEKeyboard::m_keys[i].m_name = AEKeyboard::m_keycodes[a].first;
			}
		}
		if (AEKeyboard::m_keys[i].m_name == nullptr) {
			AEKeyboard::m_keys[i].m_name = "---";
		}
	}
	//done with keyboard
	artyk::app_startstatus = 1;



	AEScreen::g_cfi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	AEScreen::g_rgb_color.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	AEScreen::g_normal_color.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	if (
		GetConsoleScreenBufferInfoEx(artyk::g_output_handle, &AEScreen::g_normal_color) &&
		GetConsoleScreenBufferInfoEx(artyk::g_output_handle, &AEScreen::g_rgb_color) &&
		GetCurrentConsoleFontEx(artyk::g_output_handle, FALSE, &AEScreen::g_cfi) &&
		GetConsoleScreenBufferInfo(artyk::g_output_handle, &AEScreen::g_csbi)
		)
	{
		artyk::utils::FError("Error initialising screen", DEF_MNAME, GET_FULL_DBG_INFO, artyk::exitcodes::INIT_SC_ERROR);
	}
	artyk::app_startstatus = 2; //done with screen

	srand(time(NULL));
	SetConsoleActiveScreenBuffer(artyk::g_output_handle);
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	std::locale loc("en_US.UTF8");
	std::cout.imbue(loc);
	std::locale::global(loc);
	std::setlocale(LC_ALL, "en_US.UTF8");

	//SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	//SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);
	//ShowScrollBar(GetConsoleWindow(), SB_VERT, 0);

	artyk::app_startstatus = SHRT_MAX;
	//done intitializing


	return 1;
}