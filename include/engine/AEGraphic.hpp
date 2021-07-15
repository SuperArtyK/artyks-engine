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

/** @file include/engine/AEGraphic.hpp
 *  This file contains the code for graphics engine.
 *  Very big thanks to David "JavidX9" Barr, for helping write the code and for inspiration!
 *  Inspired by his olcConsoleGameEngine.
 *  https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef AEGRAPHIC_HPP
#define AEGRAPHIC_HPP

#include "AEBaseClass.hpp"
#include "AEScreen.hpp"

class AEGraphic : public __AEBaseClass
{
public:
	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="enablelog">flag to check if we need to log stuff at all</param>
	/// <param name="useGlobLog">flag to check if to use gloabal_logger or not(if it exists)</param>
	AEGraphic(short sizex = 128, short sizey = 128, short fonth = 5, short fontw = 5, int fpsdelay = GAME_FPS, bool enablelog = true, bool useGlobLog = false);
	~AEGraphic() override;
	
	void drawscreen();
	void clearscreen();
	void setpixel(short x, short y, wchar_t mych, smalluint color);
	void setpixel(short x, short y, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	void fill(short x1, short y1, short x2, short y2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);

	void fill(short x1, short y1, short x2, short y2, wchar_t mych, smalluint color);

	void fillrandom(short x1, short y1, short x2, short y2, wchar_t mych);

	CHAR_INFO getpixel(short x, short y) const {
		return m_screendata[m_screensize.X * y + x];
	}

	void drawLine(int x1, int y1, int x2, int y2, wchar_t mych, smalluint color) {
		const int 
			deltaX = abs(x2 - x1), 
			deltaY = abs(y2 - y1), 
			signX = x1 < x2 ? 1 : -1, 
			signY = y1 < y2 ? 1 : -1;
		if (deltaX == 0) {
			for (int i = 0; i < deltaY; i++) {
				y1 += signY;
				setpixel(x1, y1, mych, color);
			}
			return;
		}
		if (deltaY == 0) {
			for (int i = 0; i < deltaX; i++) {
				x1 += signX;
				setpixel(x1, y1, mych, color);
			}
			return;
		}
		if (deltaY == deltaX) {
			for (int i = 0; i < deltaX; i++) {
				x1 += signX;
				y1 += signY;
				setpixel(x1, y1, mych, color);
			}
			return;
		}

		int error = deltaX - deltaY;
		setpixel(x2, y2, mych, color);
		while (x1 != x2 || y1 != y2)
		{
			setpixel(x1, y1, mych, color);
			int error2 = error * 2;
			if (error2 > -deltaY)
			{
				error -= deltaY;
				x1 += signX;
			}
			if (error2 < deltaX)
			{
				error += deltaX;
				y1 += signY;
			}
		}

	}

	void setscreen(short swidth = 128, short sheight = 128, short fonth = 14, short fontw = 7) {
		bool m_settingscreen = true;
		clearscreen();
		m_myscr.setScreen(swidth, sheight, fonth, fontw);
		if(m_screendata)
			delete[] m_screendata;
		m_screendata = new CHAR_INFO[swidth * sheight];
		m_settingscreen = false;
		
	}

	void setRenderType(int rtype) {
		switch (rtype)
		{

		case 0:
			startrd();
			break;
		case 1:
			closetrd();
			break;

		default:
			break;
		}
	}

	constexpr static inline smalluint calcColor(smalluint bgr = artyk::color::DEF_BGR, smalluint fgr = artyk::color::DEF_FGR){
		return bgr * 16 + fgr;
	}
	static int getfps(void) {
		return graph_fps;
	}

private:
	///FIXME:rewrite the declarations to remove byte padding
	void startrd(void);
	void closetrd(void);
	void mainthread(void);
	AEScreen m_myscr;
	AEFrame m_fr;
	std::thread m_thread;
	_SMALL_RECT m_rtwindow;
	///variable to store the module index number
	static atomic<biguint> m_globalmodulenum;
	atomic<CHAR_INFO*> m_screendata;
	COORD m_screensize;
	static int graph_fps;
	static bool m_threadon;
	bool m_bstoptrd;
	atomic<bool> m_settingscreen;
};

#endif // !AEGRAPHIC_HPP




