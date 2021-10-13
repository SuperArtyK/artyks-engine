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



namespace artyk::color {
	constexpr uint rgbtoint(const smalluint r, const smalluint g, const smalluint b) {
		return uint(r | (g << 8) | (b << 16));
	}
	constexpr std::array<smalluint, 3> inttorgb(const uint rgbint) {
		return std::array<smalluint, 3>{smalluint(rgbint), smalluint(rgbint >> 8), smalluint(rgbint >> 16)};
	}

	constexpr uint invertcolor(const uint rgb) {
		return (~rgb << 8) >> 8;
	}
}


class AEPalette {
public:



	AEPalette(const std::array<uint, 16>& palette) {
		for (int i = 0; i < 16; i++) {
			colorpal[i] = palette[i];
		}
	}

	uint getcolor(const smalluint colornum) const {
		return colorpal[colornum];
	}
	
	void setcolor(const smalluint colornum, const uint col) {
		colorpal[colornum] = col;
	}

	constexpr static uint rgbtoint_ae(const smalluint r, const smalluint g, const smalluint b) {
		return uint(r | (g << 8) | (b << 16));
	}
	constexpr static std::array<smalluint, 3> inttorgb(const uint rgbint) {
		return std::array<smalluint, 3>{smalluint(rgbint), smalluint(rgbint >> 8), smalluint(rgbint >> 16)};
	}

	constexpr static inline uint invertcolor(const uint rgb) {
		return (~rgb << 8) >> 8;
	}


private:
	///default engine palette
	constexpr static uint cp_default[16]{
		//dark
		0,                      //black 
		rgbtoint_ae(0,   0,   128),//blue  
		rgbtoint_ae(0,   128, 0),  //green 
		rgbtoint_ae(0,   128, 128),//cyan  
		rgbtoint_ae(128, 0,   0),  //red   
		rgbtoint_ae(128, 0,   128),//violet
		rgbtoint_ae(128, 128, 0),  //yellow
		rgbtoint_ae(172, 172, 172),//white 
		//light
		rgbtoint_ae(86,  86,  86), //black 
		rgbtoint_ae(0,   0,   255),//blue  
		rgbtoint_ae(0,   255, 0),  //green 
		rgbtoint_ae(0,   255, 255),//cyan  
		rgbtoint_ae(255, 0,   0),  //red   
		rgbtoint_ae(255, 0,   255),//violet
		rgbtoint_ae(255, 208, 0),  //yellow
		rgbtoint_ae(255, 255, 255),//white 
	};
	///classic console palette
	constexpr static uint cp_conclassic[16]{
		//dark
		rgbtoint_ae(12,  12,  12), //black 
		rgbtoint_ae(0,   55,  218),//blue  
		rgbtoint_ae(19,  161, 14), //green 
		rgbtoint_ae(58,  150, 221),//cyan  
		rgbtoint_ae(197, 15,  31), //red   
		rgbtoint_ae(136, 23,  152),//violet
		rgbtoint_ae(193, 156, 0), 	//yellow
		rgbtoint_ae(204, 204, 204),//white 
		//light
		rgbtoint_ae(118, 118, 118),//black 
		rgbtoint_ae(59,  120, 255),//blue  
		rgbtoint_ae(22,  198, 12), //green 
		rgbtoint_ae(97,  214, 214),//cyan  
		rgbtoint_ae(231, 72,  86), //red   
		rgbtoint_ae(180, 0,   158),//violet
		rgbtoint_ae(249, 241, 165),//yellow
		rgbtoint_ae(242, 242, 242),//white 
	};
	constexpr static uint test[2]{
		1,2
	};
	uint colorpal[16];

};


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
	//AEGraphic mygx(screenx, screeny, fonth, fontw);
	AESleep myd;
	smalluint color = 3;
	myscr.settitle("Setting up...");
	artyk::utils::waitfortick();
	myscr.settitle("Done!");
	Sleep(100);

	unsigned long colortable[16]{
		//dark
		0,
		rgbtoint(0,0,128),
		rgbtoint(0,128,0),
		rgbtoint(0,128,128),
		rgbtoint(128,0,0),
		rgbtoint(128,0,128),
		rgbtoint(192,156,0),
		rgbtoint(172,172,172),
		//light
		rgbtoint(86,86,86),
		rgbtoint(0,0,255),
		rgbtoint(0,255,0),
		rgbtoint(0,255,255),
		rgbtoint(255,0,0),
		rgbtoint(255,0,255),
		rgbtoint(255,208,0),
		rgbtoint(255,255,255),
	};

	constexpr uint test[2] = { rgbtoint(0, 0, 128), rgbtoint(0, 0, 128) };

	CONSOLE_SCREEN_BUFFER_INFOEX g_normal_color;
	g_normal_color.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(artyk::g_output_handle, &g_normal_color);

	cout << "\n" << g_normal_color.dwSize.X << " " << g_normal_color.dwSize.Y << "\n";

	for (int i = 0; i < 16; i++) {
		myscr.setcolor_con(0, i);
		cout << (int)((unsigned char*)&g_normal_color.ColorTable[i])[0] << " " << (int)((unsigned char*)&g_normal_color.ColorTable[i])[1] << " " << (int)((unsigned char*)&g_normal_color.ColorTable[i])[2] << " " << endl;
	}
	cin.get();
	for (int a = 0; a < 16; a++) {
		g_normal_color.ColorTable[a] = invertcolor( colortable[a]);
	}

	SetConsoleScreenBufferInfoEx(artyk::g_output_handle, &g_normal_color);

	myscr.clear();
	for (int i = 0; i < 16; i++) {
		myscr.setcolor_con(i, i);
		cout << (int)((unsigned char*)&g_normal_color.ColorTable[i])[0] << " " << (int)((unsigned char*)&g_normal_color.ColorTable[i])[1] << " " << (int)((unsigned char*)&g_normal_color.ColorTable[i])[2] << " " << endl;
	}
	for (;;){

		timeend = getsystime;
		fElapsedTime = calculatetime(timestart,timeend);
		if (fElapsedTime > 1.0f) {
			fps = i - previ;
			previ = i;
			timestart = timeend;

			//mybp.makesound_async(660, 200);
			myscr.settitle("GFX FPS: " + to_string(AEGraphic::getfps()) + "|GAME FPS: " + to_string(fps));


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