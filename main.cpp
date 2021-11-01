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

#define WINVER 0x0601 
#define _WIN32_WINNT 0x0601  

#if __cplusplus <= 201702L
#error "C++ is not of standard of 2017 or higher! Set the standard to it!" 
#error "Also you probably didn't set the /Zc:__cplusplus switch, so your __cplusplus will always be \'199711L\' no matter what standard you use\nit's advisable you set it"
#else
#pragma message("Your c++ standard is c++17 or higher, that's a good sign!")
#endif // __cplusplus <= 201702L

#ifndef UNICODE
#error It looks like you are not using unicode in your project/solution, please fix that
#endif // !#define UNICODE

#pragma comment(lib, "winmm.lib")





#include "includeall.hpp"
#include "AEGraphic.hpp"

using namespace std;

#define scrhalfx (screenx/2)
#define scrhalfy (screeny/2)
#define halfscreen vec2int{screenx/2,screeny/2}


short screenx = 128, screeny = 128, fontw = 5, fonth = 5;

//TODO: move to separate files






int main()
{
	artyk::init_main(true, true, false);
	//your code goes below...
	int i = 0;
	int previ = 0;
	systime timestart = getsystime;
	systime timeend;
	float fElapsedTime;
	//cout << comparevec(vec3{ 0,0,0 }, vec2{ 0,0}, 0);
	int fps = 0;
	AEScreen myscr;
	AEFrame myfr(100);
	AEKeyboard mykb;
	AEPalette mypl({
		0,
		artyk::color::rgbtoint(15,15,15),
		artyk::color::rgbtoint(30,30,30),
		artyk::color::rgbtoint(45,45,45),
		artyk::color::rgbtoint(60,60,60),
		artyk::color::rgbtoint(75,75,75),
		artyk::color::rgbtoint(90,90,90),
		artyk::color::rgbtoint(105,105,105),
		artyk::color::rgbtoint(120,120,120),
		artyk::color::rgbtoint(135,135,135),
		artyk::color::rgbtoint(150,150,150),
		artyk::color::rgbtoint(165,165,165),
		artyk::color::rgbtoint(180,180,180),
		artyk::color::rgbtoint(195,195,195),
		artyk::color::rgbtoint(210,210,210),
		artyk::color::rgbtoint(225,225,225),
		});
	//AEGraphic mygx(screenx, screeny, fonth, fontw);
	AESleep myd;
	smalluint color = 0;
	myscr.setTitle("Setting up...");
	artyk::utils::waitfortick();
	myscr.setTitle("Done!");
	Sleep(100);


	char text[] = "Lorem ipsum sor sit amet...";
	for (int a = 0; a < arrsize(text); a++) {
		myscr.setColor(0, a & 0xF);
		cout << text[a];
	}

	myscr.setColorPalette(mypl.getpalette(0));
	for (;;) {



		myd.sleep(100);



		timeend = getsystime;
		fElapsedTime = calculatetime(timestart, timeend);
		if (fElapsedTime > 1.0f) {
			fps = i - previ;
			previ = i;
			timestart = timeend;

			//mybp.makesound_async(660, 200);
			myscr.setTitle("GFX FPS: " + to_string(AEGraphic::getfps()) + "|GAME FPS: " + to_string(fps));


		}
		myfr.sleep();
		i++;

	}

	artyk::closing_app = 1;

	//cout << "\nPress enter to continue . . .\n";
	std::cin.get();
	//cout << "Exiting. . .\n";
	return 0;
}




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