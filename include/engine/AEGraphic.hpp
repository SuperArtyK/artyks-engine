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
#include "custom_types.hpp"





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

	void copybuffer(const CHAR_INFO* mych, int buffsize, const vec2int& myvec2 = { 0,0 }) {
		m_settingscreen = true;//atomic bool, blocks the drawing thread untill false
		if (buffsize > (m_screensize.X * m_screensize.Y)) {//precaution, if buffsize is more than screen size
			buffsize = m_screensize.X * m_screensize.Y;
		}
		memcpy(m_screendata, mych, buffsize * sizeof(CHAR_INFO));//copying data...
		m_settingscreen = false;//removing blocking
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


private:
	
	///FIXME:rewrite the declarations to remove byte padding
	void startrd(void);
	void closetrd(void);
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




