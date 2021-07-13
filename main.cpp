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

/** @file main.cpp
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
#error It looks like you are not using unicode in your project/solution, please fix that
#endif // !#define UNICODE


#include "include_engine.hpp"
using namespace std;


#ifdef AE_EXPERIMENTAL
void benchmark() {

__AEBaseClass mybc("BaseClass", 1);
mybc.benchmark();

AEBeep mybp(true, false);
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
	//not required but recommended have this function in the start of the main
	//completes engine initialisation
	artyk::init_main(false, false, false);


//your code goes below...


	//this program benchmarks the engine, putting it to the worst possible case ever, as I think.
	//it calls benchmark functions of each module, that use all possible public functions of the module once
	//as AEScreen has the setScreen() function in its benchmark, the screen will flash.
	//fps is the cycles of the main loop per second
	cout << "Each beep means that, 1+ seconds has passed and we're calculating the results of benchmarking.\nEach \"frame\" is calling benchmark function that uses all of engine's modules\nin a worst way I think possible\n";

	int i = 0;
	int previ = 0;
	auto timestart = std::chrono::system_clock::now();
	auto timeend = std::chrono::system_clock::now();
	float fElapsedTime;
	AEBeep mybp;
	AEScreen myscr;
	myscr.setcolor_con(artyk::color::DEF_BGR, artyk::color::DEF_FGR);
	int fps = 0, maxfps = fps, minfps = INT_MAX;
	artyk::utils::Info("I'm going to start the engine's benchmark. \nThis benchmark's goal is to see what performance you can get if to use the engine at it's maximum(e.g. worst case in using it).\nIt may use your processor to maximum, but I don't guarantee that it will do that, as it's not the goal.\nThis benchmark doesn't have an end, and the result is the \"FPS\" you will see in the app title.\nTo close this app, use task manager or your taskbar.", DEF_MNAME, false);
	artyk::utils::Info("Each beep will mean that 1+ seconds has passed, and we're calculating the results of benchmarking.\nEach \"frame\" is calling benchmark function that uses all of engine's modules\nin a worst way I, ArtyK, think possible.\n\nWARNING: This benchmark has excessive flashing imagery.\nPress \"OK\" to start benchmark.", DEF_MNAME,false);
	Sleep(500);
	timestart = std::chrono::system_clock::now();
	for (;;) {
		i++;
		benchmark();
	
		timeend = std::chrono::system_clock::now();
		fElapsedTime = std::chrono::duration<float>(timeend - timestart).count();
		if (fElapsedTime > 1.0f) {
			fps = i - previ;
			if (fps > maxfps) maxfps = fps;
			if (fps < minfps) minfps = fps;
			previ = i;
			timestart = timeend;
			mybp.makesound_async(660, 200);
		}
		myscr.settitle("FPS: " + to_string(fps) + "|MAX FPS: " + to_string(maxfps) + "|MIN FPS: " + to_string(minfps) + "|Time Since start : " + to_string(global_timer.getworldtime()));

		
	
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




