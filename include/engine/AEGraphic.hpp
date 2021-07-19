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


struct vec2 
{
	int x = 0;
	int y = 0;
};

struct vec3
{
	int x = 0;
	int y = 0;
	int z = 0;
};



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
	void setpixel(const vec2& myvec2, wchar_t mych, smalluint color);
	void setpixel(const vec2& myvec2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	void fill(const vec2& myvec2_1, const vec2& myvec2_2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);

	void fill(const vec2& myvec2_1, const vec2& myvec2_2, wchar_t mych, smalluint color);

	void fillrandom(const vec2& myvec2_1, const vec2& myvec2_2, wchar_t mych);

	CHAR_INFO getpixel(const vec2& myvec2) const {
		if (myvec2.x > 0 && myvec2.x < m_screensize.X && myvec2.y > 0 && myvec2.y < m_screensize.Y) {
			return m_screendata[m_screensize.X * myvec2.y + myvec2.x];
		}
		return {0,0};
	}




	void drawLine(const vec2& myvec2_1, const vec2& myvec2_2, wchar_t mych, smalluint color) {
		const int 
			deltaX = abs(myvec2_2.x - myvec2_1.x),
			deltaY = abs(myvec2_2.y - myvec2_1.y),
			signX = myvec2_1.x < myvec2_2.x ? 1 : -1,
			signY = myvec2_1.y < myvec2_2.y ? 1 : -1;
		vec2 temp = myvec2_1;
		if (deltaX == 0) {
			for (int i = 0; i < deltaY; i++) {
				temp.y += signY;
				setpixel(temp, mych, color);
			}
			return;
		}
		if (deltaY == 0) {
			for (int i = 0; i < deltaX; i++) {
				temp.x += signX;
				setpixel(temp, mych, color);
			}
			return;
		}
		if (deltaY == deltaX) {
			for (int i = 0; i < deltaX; i++) {
				temp.x += signX;
				temp.y += signY;
				setpixel(temp, mych, color);
			}
			return;
		}

		int error = deltaX - deltaY;
		setpixel(myvec2_2, mych, color);
		while (temp.x != myvec2_2.x || temp.y != myvec2_2.y)
		{
			setpixel(temp, mych, color);
			int error2 = error * 2;
			if (error2 > -deltaY)
			{
				error -= deltaY;
				temp.x += signX;
			}
			if (error2 < deltaX)
			{
				error += deltaX;
				temp.y += signY;
			}
		}

	}

	void setscreen(short swidth = 128, short sheight = 128, short fonth = 14, short fontw = 7) {
		m_settingscreen = true;
		m_myscr.setScreen(swidth, sheight, fonth, fontw);
		if(m_screendata)
			delete[] m_screendata;
		m_screendata = new CHAR_INFO[swidth * sheight];
		clearscreen();
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
		case 2:
			m_clrscr = true;
			startrd();
			break;
		case 3:
			m_clrscr = false;
			startrd();
			break;

		default:
			break;
		}
	}

	constexpr static inline smalluint getattrib(smalluint bgr = artyk::color::DEF_BGR, smalluint fgr = artyk::color::DEF_FGR){
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
	CHAR_INFO* m_screendata;
	COORD m_screensize;
	static int graph_fps;
	static bool m_threadon;
	bool m_bstoptrd;
	bool m_clrscr;
	atomic<bool> m_settingscreen;
};

#endif // !AEGRAPHIC_HPP




