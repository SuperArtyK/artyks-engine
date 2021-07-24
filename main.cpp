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
#include "AEGraphic.hpp"
using namespace std;


//void benchmark();

const short nMapWidth = 18;				// World Dimensions
const short nMapHeight = 18;

float fPlayerX = 15.0f;			// Player Start Position
float fPlayerY = 5.0f;
float fPlayerA = 0.0f;			// Player Start Rotation
const float fFOV = 1.2f;	// Field of View
const float fDepth = 32.0f;			// Maximum rendering distance
const float fSpeed = 6.0f;			// Walking Speed
const float fRotationSpd = fSpeed/4;




char gamemap[nMapWidth + 1][nMapHeight + 1]{
		{".................."},
		{".#########........"},
		{".#................"},
		{".#.......########."},
		{".#..............#."},
		{".#......##......#."},
		{".#......##......#."},
		{".#..............#."},
		{".###............#."},
		{".##.............#."},
		{".#......####..###."},
		{".#......#.......#."},
		{".#......#.......#."},
		{".#..............#."},
		{".#......#########."},
		{".#..............#."},
		{".################."},
		{".................."},
};

CHAR_INFO objdistance[]{
		{artyk::gfx::CH_100,   AEGraphic::getattrib(AEGraphic::B_GREEN, AEGraphic::B_GREEN) },
		{artyk::gfx::CH_75,    AEGraphic::getattrib(AEGraphic::D_GREEN, AEGraphic::B_GREEN) },
		{artyk::gfx::CH_50,    AEGraphic::getattrib(AEGraphic::D_GREEN, AEGraphic::B_GREEN) },
		{artyk::gfx::CH_25,    AEGraphic::getattrib(AEGraphic::D_GREEN, AEGraphic::B_GREEN) },
		{artyk::gfx::CH_100,   AEGraphic::getattrib(AEGraphic::BLACK,   AEGraphic::D_GREEN) },
		{artyk::gfx::CH_75,    AEGraphic::getattrib(AEGraphic::BLACK,   AEGraphic::D_GREEN) },
		{artyk::gfx::CH_50,    AEGraphic::getattrib(AEGraphic::BLACK,   AEGraphic::D_GREEN) },
		{artyk::gfx::CH_25,    AEGraphic::getattrib(AEGraphic::BLACK,   AEGraphic::D_GREEN) },


};


inline bool colliding() {
	return gamemap[(int)fPlayerX][(int)fPlayerY] == '#' && (fPlayerX < nMapWidth&& fPlayerX >0 && fPlayerY < nMapHeight&& fPlayerY>0);
}

int main()
{
	artyk::init_main(false, false, false);
//your code goes below...
	int i = 0;
	int previ = 0;
	systime timestart = std::chrono::system_clock::now();
	systime timeend;
	float fElapsedTime;
	int fps = 0;
	AEScreen myscr;
	short screenx = 256, screeny = 180, fontw = 2, fonth = 2;
	myscr.setFont(14, 7);
	cout << "Input the screen width: ";
	cin >> screenx;
	cout << "Input the screen height: ";
	cin >> screeny;
	cout << "Input the font width: ";
	cin >> fontw;
	cout << "Input the font height: ";
	cin >> fonth;

	AEGraphic mygx(screenx, screeny, fontw, fonth,-1);
	mygx.setRenderType(1);
	AEKeyboard mykb;
	AEFrame myfr;
	CHAR_INFO* mybuffer = new CHAR_INFO[screenx*screeny];
	
	



	const short distsize = (sizeof(objdistance) / sizeof(objdistance[0]));
	float fDistanceToWall_center;


	// Create Map of world space # = wall block, . = space
	float lastcycletime;

	//cout << (int)map[1][16];
	//Sleep(10000);
	float prevposx = fPlayerX, prevposy = fPlayerY;

	systime cyclestart = timestart;
	systime cycleend;
	
	while (1)
	{
		i++;
		cycleend = cyclestart;
		cyclestart = getsystime;
		lastcycletime = std::chrono::duration<float>(cyclestart - cycleend).count();

		float prevfill = screeny, currfill = screeny - screeny / 4;
		for (int a = 1; a < distsize; a++) {

			mygx.fill({ 0,int(currfill) }, { int(screenx),int(prevfill) }, objdistance[a]);
			prevfill = currfill;
			currfill -= currfill / (7 * (a));
		}
		//rotation
		if (mykb.GetKey(VK_LEFT).m_isUsed) {
			fPlayerA -= fRotationSpd* lastcycletime;

		}
		if (mykb.GetKey(VK_RIGHT).m_isUsed) {
			fPlayerA += fRotationSpd* lastcycletime;
		}
		//strafing/movement
		if (mykb.GetKey(0x57).m_isUsed)
		{
			fPlayerX += sinf(fPlayerA) * fSpeed*lastcycletime;
			fPlayerY += cosf(fPlayerA) * fSpeed*lastcycletime;
			if (colliding())
			{
				fPlayerX = prevposx;
				fPlayerY = prevposy;
			}
		}
		if (mykb.GetKey(0x53).m_isUsed)
		{
			fPlayerX -= sinf(fPlayerA) * fSpeed*lastcycletime;
			fPlayerY -= cosf(fPlayerA) * fSpeed*lastcycletime;
			if (colliding())
			{
				fPlayerX = prevposx;
				fPlayerY = prevposy;
			}
		}
		if (mykb.GetKey(0x44).m_isUsed)
		{
			fPlayerX += cosf(fPlayerA) * fSpeed*lastcycletime;
			fPlayerY -= sinf(fPlayerA) * fSpeed*lastcycletime;
			if (colliding())
			{
				fPlayerX = prevposx;
				fPlayerY = prevposy;
			}
		}
		if (mykb.GetKey(0x41).m_isUsed)
		{
			fPlayerX -= cosf(fPlayerA) * fSpeed*lastcycletime;
			fPlayerY += sinf(fPlayerA) * fSpeed*lastcycletime;
			if (colliding())
			{
				fPlayerX = prevposx;
				fPlayerY = prevposy;
			}
		}
		for (int x = 0; x < screenx; x++)
		{
			// For each column, calculate the projected ray angle into world space
			float fRayAngle = (fPlayerA - fFOV / 2.0f) + ((float)x / (float)screenx) * fFOV;

			// Find distance to wall
			float fStepSize = 0.1f;		  // Increment size for ray casting, decrease to increase										
			float fDistanceToWall = 0.0f; //                                      resolution

			bool bHitWall = false;		// Set when ray hits wall block
			bool bBoundary = false;		// Set when ray hits boundary between two wall blocks

			float fEyeX = sinf(fRayAngle); // Unit vector for ray in player space
			float fEyeY = cosf(fRayAngle);

			while (!bHitWall && fDistanceToWall < fDepth)
			{
				fDistanceToWall += fStepSize;
				
				int nTestX = (int)(fPlayerX + fEyeX * fDistanceToWall);
				int nTestY = (int)(fPlayerY + fEyeY * fDistanceToWall);

				if (nTestX >= 0 && nTestX < nMapWidth && nTestY >= 0 && nTestY < nMapHeight)
				{
					if (gamemap[nTestX][nTestY] == '#')
					{
						bHitWall = true;
					}
				}
			}
			int nCeiling = (float)(screeny / 2.0) - screeny / ((float)fDistanceToWall);
			int nFloor = screeny - nCeiling;
			
			if (x == screenx / 2) {
				fDistanceToWall_center = fDistanceToWall;
			}
			CHAR_INFO nShade;
			nShade = artyk::gfx::PX_EMPTY;
			int indx = (fDistanceToWall / fDepth) * distsize;
			if (fDistanceToWall <= fDepth) {
				nShade = objdistance[(indx <distsize-1)? indx+1: indx];
			}
			if (fDistanceToWall < 1) {
				nShade = objdistance[0];
			}
			mygx.drawLine({ x,0 }, { x,nCeiling }, artyk::gfx::PX_EMPTY);
			mygx.drawLine({ x,nCeiling }, { x,nFloor }, nShade);
			if (bHitWall) {
				mygx.setPixel({ x,nFloor }, artyk::gfx::CH_25, AEGraphic::getattrib(mygx.B_GREEN, mygx.WHITE));
				mygx.setPixel({ x,nCeiling }, artyk::gfx::CH_50, AEGraphic::getattrib(mygx.D_GREEN, mygx.BLACK));
			}
		}

		mygx.setPixel({ screenx / 2,screeny / 2 }, artyk::gfx::CH_100, 15);
		mygx.setPixel({ screenx / 2+1,screeny / 2 }, artyk::gfx::CH_100, 15);
		mygx.setPixel({ screenx / 2-1,screeny / 2 }, artyk::gfx::CH_100, 15);
		mygx.setPixel({ screenx / 2,screeny / 2+1 }, artyk::gfx::CH_100, 15);
		mygx.setPixel({ screenx / 2,screeny / 2-1 }, artyk::gfx::CH_100, 15);
		mygx.drawscreen();

		prevposx = fPlayerX, prevposy = fPlayerY;
		timeend = getsystime;
		fElapsedTime = std::chrono::duration<float>(timeend - timestart).count();

		if (fElapsedTime > 1.0f) {
			fps = i - previ;
			previ = i;
			timestart = timeend;
			//mybp.makesound_async(660, 200);
			myscr.settitle("GFX FPS: " + to_string(AEGraphic::getfps()) + "|GAME FPS: " + to_string(fps) + "| posx:"+to_string(fPlayerX) + " posx:" + to_string(fPlayerY));

		}
		
		//myfr.sleep();
	}

	

	delete[] mybuffer;



	artyk::closing_app = 1;

	if (artyk::closing_app) {
		AELog mylog;
		mylog.writetolog("Closed the \"" + artyk::app_name + "\". Version: " + artyk::app_version + " Build: " + to_string(artyk::app_build), LOG_INFO);
	}
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