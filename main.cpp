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


	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="palette">Palette color array to set. Leave blank for default engine palette to be applied</param>
	AEPalette(const std::array<uint, 16>& palette = cp_default) {
		for (int i = 0; i < 16; i++) {
			colorpal[i] = palette[i];
		}
	}
	///returns rgb value of color from given color number in palette
	uint getcolor(const smalluint colornum) const {
		return colorpal[colornum];
	}
	
	///sets color in palette to certain rgb value
	void setcolor(const smalluint colornum, const uint col) {
		colorpal[colornum] = col;
	}

	std::array<uint, 16> getpalette(const smalluint tpe = 0) const {
		switch (tpe)
		{
		case 1:
			return cp_default;
			break;

		case 2:
			return cp_conclassic;
			break;

		default:
			return colorpal;
			break;
		}
	}
	

private:
	///default engine palette
	constexpr static std::array<uint, 16> cp_default{
		//dark
		artyk::color::rgbtoint(0,   0,   0),  //black 
		artyk::color::rgbtoint(0,   0,   128),//blue  
		artyk::color::rgbtoint(0,   128, 0),  //green 
		artyk::color::rgbtoint(0,   128, 128),//cyan  
		artyk::color::rgbtoint(128, 0,   0),  //red   
		artyk::color::rgbtoint(128, 0,   128),//violet
		artyk::color::rgbtoint(128, 128, 0),  //yellow
		artyk::color::rgbtoint(172, 172, 172),//white 
		//light
		artyk::color::rgbtoint(86,  86,  86), //black 
		artyk::color::rgbtoint(0,   0,   255),//blue  
		artyk::color::rgbtoint(0,   255, 0),  //green 
		artyk::color::rgbtoint(0,   255, 255),//cyan  
		artyk::color::rgbtoint(255, 0,   0),  //red   
		artyk::color::rgbtoint(255, 0,   255),//violet
		artyk::color::rgbtoint(255, 208, 0),  //yellow
		artyk::color::rgbtoint(255, 255, 255),//white 
	};
	///classic console palette
	constexpr static std::array<uint, 16> cp_conclassic{
		//dark
		artyk::color::rgbtoint(12,  12,  12), //black 
		artyk::color::rgbtoint(0,   55,  218),//blue  
		artyk::color::rgbtoint(19,  161, 14), //green 
		artyk::color::rgbtoint(58,  150, 221),//cyan  
		artyk::color::rgbtoint(197, 15,  31), //red   
		artyk::color::rgbtoint(136, 23,  152),//violet
		artyk::color::rgbtoint(193, 156, 0),  //yellow
		artyk::color::rgbtoint(204, 204, 204),//white 
		//light
		artyk::color::rgbtoint(118, 118, 118),//black 
		artyk::color::rgbtoint(59,  120, 255),//blue  
		artyk::color::rgbtoint(22,  198, 12), //green 
		artyk::color::rgbtoint(97,  214, 214),//cyan  
		artyk::color::rgbtoint(231, 72,  86), //red   
		artyk::color::rgbtoint(180, 0,   158),//violet
		artyk::color::rgbtoint(249, 241, 165),//yellow
		artyk::color::rgbtoint(242, 242, 242),//white 
	};

	///color pallete of 16 colors
	std::array<uint, 16> colorpal;

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
	constexpr aevector<int, 2> tmp = { 23,3 };
	constexpr double temp2 = tmp.length();
	vector<int> a;
	

	CONSOLE_SCREEN_BUFFER_INFOEX g_normal_color;
	g_normal_color.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(artyk::g_output_handle, &g_normal_color);

	cout << "\n" << g_normal_color.dwSize.X << " " << g_normal_color.dwSize.Y << "\n";

	for (int i = 0; i < 16; i++) {
		myscr.setcolor_con(0, i);
		cout << (int)((unsigned char*)&g_normal_color.ColorTable[i])[0] << " " << (int)((unsigned char*)&g_normal_color.ColorTable[i])[1] << " " << (int)((unsigned char*)&g_normal_color.ColorTable[i])[2] << " " << endl;
	}
	SetConsoleScreenBufferInfoEx(artyk::g_output_handle, &g_normal_color);

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