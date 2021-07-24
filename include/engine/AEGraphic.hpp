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
 *  
 *  Credits:
 *  Very big thanks to David "JavidX9" Barr, for helping write the code and for inspiration!
 *  Inspired by his olcConsoleGameEngine.
 *  https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
 *  Ravbug -- for giving ideas and pointing out errors
 *  Check him out: https://www.ravbug.com/
 *  
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef AEGRAPHIC_HPP
#define AEGRAPHIC_HPP

#include "AEBaseClass.hpp"
#include "AEScreen.hpp"
#include "custom_types.hpp"
#include "engine_math.hpp"




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
	
	///draws the screen, using data from m_screendata
	void drawscreen();
	///draws the current selected buffer directly, without copying
	void drawbuffer();
	///clears the m_screendata buffer(screen buffer)
	void clearscreen();
	///clears the current selected buffers(the one that setPixel() uses)
	void clearbuffer();
	/// <summary>
	/// copies given buffer to the the screen buffer
	/// </summary>
	/// <param name="mych">the buffer itself; </param>
	/// <param name="buffsize">size of the buffer, in array members; default to m_screensize</param>
	/// @note if size is bigger than current screen buffer size, it will be truncated
	void copybuffer(const CHAR_INFO* mych = m_currentbuffer, int buffsize = m_screensize.X*m_screensize.Y);
	///sets current buffer
	void setbuffer(CHAR_INFO* mych) {
		m_currentbuffer = mych;
	}

	void setscreen(short swidth = 128, short sheight = 128, short fonth = 14, short fontw = 7, bool preservedata = false);

	void setPixel(const vec2int& myvec2, wchar_t mych, smalluint color);
	void setPixel(const vec2int& myvec2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);

	void fill(const vec2int& myvec2_1, const vec2int& myvec2_2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	void fill(const vec2int& myvec2_1, const vec2int& myvec2_2, wchar_t mych, smalluint color);

	void fillRandom(const vec2int& myvec2_1, const vec2int& myvec2_2, wchar_t mych);

	void drawLine(const vec2int& myvec2_1, const vec2int& myvec2_2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	void drawLine(const vec2int& myvec2_1, const vec2int& myvec2_2, wchar_t mych, smalluint color);

	void drawTriangle(const vec2int& myvec2_1, const vec2int& myvec2_2, const vec2int& myvec2_3, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	void drawTriangle(const vec2int& myvec2_1, const vec2int& myvec2_2, const vec2int& myvec2_3, wchar_t mych, smalluint color);

	void drawRegPoly(const vec2int& myvec2, const short radius, const short sideamount = 5) {
		const float angleincrement = 360.0f / sideamount;
		vec2int pointpos;
		for (int i = 0; i < sideamount; i++) {
			pointpos.x = artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(angleincrement * i - 90) * radius);
			pointpos.y = artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(angleincrement * i - 90) * radius);
			setPixel(pointpos);
		}

	}
	
	void drawCircle(const vec2int& myvec2, const short radius, const vec2& distortion = { 1.0f,1.0f }) {
		vec2int pointpos;
		const float increment = 360.0f/(3.1416f * radius * radius);
		for (float i = 0; i < 360.0f; i += increment) {
			pointpos.x = artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(i) * (radius-1));
			pointpos.y = artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(i) * (radius-1));
			setPixel(pointpos);
		}

	}
	void drawCircle2(const vec2int& myvec2, short radius, vec2 distortion = { 1.0f,1.0f }) {
		vec2int pointpos;
		pointpos.x = artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(0) * (radius-1));
		pointpos.y = artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(0) * (radius-1));
		vec2int prevpos = pointpos;
		for (int i = 1; i < 360; i += 1) {
			pointpos.x = artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(i) * (radius - 1));
			pointpos.y = artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(i) * (radius - 1));
			drawLine(prevpos,pointpos);
			prevpos = pointpos;
		}

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
			break;
		case 3:
			m_clrscr = false;
			break;

		case 4:
			
			break;

		default:
			break;
		}
	}

	CHAR_INFO getpixel(const vec2int& myvec2) const;

	constexpr static inline smalluint getattrib(smalluint bgr = AEGraphic::DEF_BGR, smalluint fgr = AEGraphic::DEF_FGR) {
		return bgr * 16 + fgr;
	}
	static int getfps(void) {
		return graph_fps;
	}

	
	///\brief console colors for the graphics engine
	/// took them from the color namespace
	/// @see artyk::color
	static constexpr unsigned char
		BLACK = artyk::color::BLACK,
		D_BLUE = artyk::color::D_BLUE,    //DARK
		D_GREEN = artyk::color::D_GREEN,   //DARK
		D_CYAN = artyk::color::D_CYAN,    //DARK
		D_RED = artyk::color::D_RED,     //DARK
		D_MAGENTA = artyk::color::D_MAGENTA, //DARK
		D_YELLOW = artyk::color::D_YELLOW,  //DARK
		D_WHITE = artyk::color::D_WHITE,   //DARK
		GRAY = artyk::color::GRAY,
		B_BLUE = artyk::color::B_BLUE,    //BRIGHT
		B_GREEN = artyk::color::B_GREEN,  //BRIGHT
		B_CYAN = artyk::color::B_CYAN,   //BRIGHT
		B_RED = artyk::color::B_RED,    //BRIGHT
		B_PURPLE = artyk::color::B_PURPLE, //BRIGHT
		B_YELLOW = artyk::color::B_YELLOW, //BRIGHT
		WHITE = artyk::color::WHITE,

		//default colors
		DEF_BGR = artyk::color::DEF_BGR,
		DEF_FGR = artyk::color::DEF_BGR,
		DEF_ATTR = artyk::color::DEF_BGR * 16 + artyk::color::DEF_FGR;

	void startrd(void);
	void closetrd(void);
private:
	
	///FIXME:rewrite the declarations to remove byte padding
	void mainthread(void);
	void convertScreenType(char screentype) {

		

	}
	AEScreen m_myscr;
	AEFrame m_fr;
	std::thread m_thread;
	_SMALL_RECT m_rtwindow;
	///variable to store the module index number
	static atomic<biguint> m_globalmodulenum;
	///the buffer we made
	static CHAR_INFO* m_screendata;
	///the buffer our utilities will use, usually m_screendata
	///otherwise, if its set to other buffer, copy it with copybuffer()
	static CHAR_INFO* m_currentbuffer;

	static COORD m_screensize;
	static int graph_fps;
	///amount of graphics modules currently active
	static atomic<int> moduleamt;
	static atomic<bool> m_settingscreen;
	static bool m_threadon;
	bool m_bstoptrd;
	bool m_clrscr;
	bool m_monochrome;
	
};

#endif // !AEGRAPHIC_HPP




