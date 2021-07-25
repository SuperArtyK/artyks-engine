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

    void drawPoly(const vec2int parr[], const unsigned int arrsize){
        if(arrsize == 0) return;
        vec2int prevpos{parr[0]};
        for(unsigned int i = 1; i < arrsize; i++){
            drawLine(prevpos,parr[i]);
            prevpos = parr[i];
        }
    }

	void drawRegPoly(const vec2int& myvec2, const int radius, const int sideamount = 5) {
		const float angleincrement = 360.0f / sideamount;
		vec2int pointpos;
		vec2int prevpos{
		    artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(-90) * radius),
		    artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(-90) * radius)
	    };
		for (int i = 1; i < sideamount+1; i++) {
			pointpos.x = artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(angleincrement * i - 90) * radius);
			pointpos.y = artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(angleincrement * i - 90) * radius);
			drawLine(pointpos, prevpos);
			prevpos = pointpos;
		}

	}
	
	void drawRegPoly2(const vec2int& myvec2, const int radius, const int sideamount = 5) {
		const float angleincrement = 360.0f / sideamount;
		vec2int pointpos;
		vec2int prevpos{
		    artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(-90) * radius),
		    artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(-90) * radius)
	    };
		for (float i = -90+angleincrement; i < 360; i+=angleincrement) {
			pointpos.x = artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(i) * radius);
			pointpos.y = artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(i) * radius);
			drawLine(pointpos, prevpos);
			prevpos = pointpos;
		}

	}

	void drawCircle(const vec2int& myvec2, const int radius, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);

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
    ///returns pixel value at given location
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
    ///opens and starts the drawing thread
	void startrd(void);
	///stops and closes the drawing thread
	void closetrd(void);
private:
	
	//FIXME:rewrite the declarations to remove byte padding
	///the graphics thread code
	void mainthread(void);
	///pointer AEScreen module for screen manipulations
	///needed as if to make it a variable, it would crash with "bad handle" error
	///because it creates the module before creating and assigning handles
	static AEScreen* m_myscr;
	///AEFrame module for thread drawing delay
	static AEFrame m_fr;
	///thread object to create drawing thread
	static std::thread m_thread;
	///variable to store window size
	_SMALL_RECT m_rtwindow;
	///variable to store the module index number
	static atomic<biguint> m_globalmodulenum;
	///the screen buffer that module draws
	static CHAR_INFO* m_screendata;
	///the buffer our utilities will use, usually m_screendata
	///otherwise, if its set to other buffer, copy it with copybuffer()
	static CHAR_INFO* m_currentbuffer;
    ///stores screen size, in character cells
	static COORD m_screensize;
	///the thread fps amount, get it using getfps(void)
	static int graph_fps;
	///amount of graphics modules currently active
	///(to close thread and dealloc memory when needed)
	static atomic<int> moduleamt;
	///bool to indicate if we are doing something with console screen or buffer
	///(prevents write if true)
	static atomic<bool> m_settingscreen;
	///bool to indicate if the thread is working currently
	static bool m_threadon;
	///bool to indicate if we need to stop the drawing thread
	static bool m_bstoptrd;
	///bool to indicate if graphics thread needs to clear buffer after drawing
	///(not used if AE_GFX_ALWAYS_CLEAR_AFTER_DRAW is on) 
	static bool m_clrscr;
};

#endif // !AEGRAPHIC_HPP




