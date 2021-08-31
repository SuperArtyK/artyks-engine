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
 *  Should not cause everything to break.
 */

#pragma once

#ifndef AEGRAPHIC_HPP
#define AEGRAPHIC_HPP

#include "AEBaseClass.hpp"
#include "AEScreen.hpp"
#include "custom_types.hpp"
#include "engine_math.hpp"
#include <array>
#include <vector>

///create CHAR_INFO with given character and color
#define PIXEL(...) _PIXEL(__VA_ARGS__)
///techincal side of PIXEL(), so we get exactly 2 arguments
#define _PIXEL(chr,clr) CHAR_INFO{chr, clr}
///(current) pixel size, in bytes
#define PIXELSIZE sizeof(CHAR_INFO)


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
	
	/// draws the screen, using data from m_screendata
	void drawscreen();
	
	/// draws the current selected buffer directly, without copying
	void drawbuffer();
	
	/// clears the m_screendata buffer(screen buffer)
	void clearscreen();
	
	/// clears the current selected buffers(the one that setPixel() uses)
	void clearbuffer();
	
	/// <summary>
	/// copies given buffer to the the screen buffer
	/// </summary>
	/// <param name="mych">the buffer itself; default is m_currentbuffer</param>
	/// <param name="buffsize">size of the buffer, in array members; default to m_screensize</param>
	/// @note if size is bigger than current screen buffer size, it will be truncated; and if it's lower
	/// @note and if size is lower than current screen buffer size, what was in buffer out of new buffer bounds will be left.
	/// @note if nothing is passed, the m_currentbuffer will be copied to the drawing buffer
	void copybuffer(const CHAR_INFO* mych = m_currentbuffer, int buffsize = m_screensize.X*m_screensize.Y);
	
	/// Creates third buffer(second AEGraphic buffer) to create frames and feed them entirely to drawing thread.
	/// Helps to remove flickering
	void createTripleBuffering(void);

	///removes third buffer and deallocates it(so be carefull with all those dangling pointers)
	void removeTripleBuffering(void);

	/// <summary>
	/// returns pointer to third pointer, if it exists. If it doesnt, returns nullptr
	/// </summary>
	/// @warning USE THIS WITH CARE! DON'T DELETE THE POINTER, OR GRAPHICS ENGINE WILL CRASH!
	CHAR_INFO* getTripleBufferPtr(void);

	/// <summary>
	/// Sets the console screen to given dimensions if possible, otherwise throws Fatal error.
	/// Also reallocates and clears buffers 
	/// </summary>
	/// <param name="swidth">screen width, in console character cells</param>
	/// <param name="sheight">screen height, in console character cells</param>
	/// <param name="fonth">character font height, in pixels, doesnt't go less than 2</param>
	/// <param name="fontw">character font width, in pixels, doesnt't go less than 2</param>
	void setscreen(short swidth = 128, short sheight = 128, short fonth = 14, short fontw = 7);

	/// <summary>
	/// sets pixel at given position with given CHAR_INFO
	/// </summary>
	/// <param name="myvec2">2d pixel position in console buffer</param>
	/// <param name="mych">character and attribute</param>
	void setPixel(const vec2int& myvec2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	
	/// <summary>
	/// sets pixel at given position with given character and attribute
	/// </summary>
	/// <param name="myvec2">2d pixel position in console buffer</param>
	/// <param name="mych">character to set</param>
	/// <param name="color">color/attribute to set</param>
	void setPixel(const vec2int& myvec2, const wchar_t mych, const smalluint color);

	/// <summary>
	/// fills rectangle between points with given CHAR_INFO
	/// </summary>
	/// <param name="myvec2_1">point 1 of rectangle</param>
	/// <param name="myvec2_2">point 2 of rectangle</param>
	/// <param name="mych">CHAR_INFO to set</param>
	void fill(const vec2int& myvec2_1, const vec2int& myvec2_2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);

	/// <summary>
	/// fills rectangle between points with given character and attribute
	/// </summary>
	/// <param name="myvec2_1">point 1 of rectangle</param>
	/// <param name="myvec2_2">point 2 of rectangle</param>
	/// <param name="mych">character to set</param>
	/// <param name="color">color/attribute to set</param>
	void fill(const vec2int& myvec2_1, const vec2int& myvec2_2, const wchar_t mych, const smalluint color);

	/// <summary>
	/// fills rectangle between points with given character; the attribute for each pixel is random
	/// </summary>
	/// <param name="myvec2_1">point 1 of rectangle</param>
	/// <param name="myvec2_2">point 2 of rectangle</param>
	/// <param name="mych">character to set</param>
	void fillRandom(const vec2int& myvec2_1, const vec2int& myvec2_2, const wchar_t mych);

	/// <summary>
	/// draws line between 2 points with given CHAR_INFO
	/// </summary>
	/// <param name="myvec2_1">point 1 of line</param>
	/// <param name="myvec2_2">point 2 of line</param>
	/// <param name="mych">CHAR_INFO to set</param>
	void drawLine(const vec2int& myvec2_1, const vec2int& myvec2_2, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);

	/// <summary>
	/// draws line between 2 points with given 
	/// </summary>
	/// <param name="myvec2_1">point 1 of line</param>
	/// <param name="myvec2_2">point 2 of line</param>
	/// <param name="mych">character to set</param>
	/// <param name="color">color/attribute to set</param>
	void drawLine(const vec2int& myvec2_1, const vec2int& myvec2_2, const wchar_t mych, const smalluint color);

	/// <summary>
	/// draws triangle between 3 points with given CHAR_INFO
	/// </summary>
	/// <param name="myvec2_1">point 1 of triangle</param>
	/// <param name="myvec2_2">point 2 of triangle</param>
	/// <param name="myvec2_3">point 3 of triangle</param>
	/// <param name="mych">CHAR_INFO to set</param>
	void drawTriangle(const vec2int& myvec2_1, const vec2int& myvec2_2, const vec2int& myvec2_3, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	
	/// <summary>
	/// draws triangle between 3 points with given char and color
	/// </summary>
	/// <param name="myvec2_1">point 1 of triangle</param>
	/// <param name="myvec2_2">point 2 of triangle</param>
	/// <param name="myvec2_3">point 3 of triangle</param>
	/// <param name="mych">character to set</param>
	/// <param name="color">color/attribute to set</param>
	void drawTriangle(const vec2int& myvec2_1, const vec2int& myvec2_2, const vec2int& myvec2_3, const wchar_t mych, const smalluint color);

	/// <summary>
	/// draws polygon from given vec2 array and size of array of vertices and CHAR_INFO
	/// </summary>
	/// <param name="parr">array of the vertices in 2d space</param>
	/// <param name="polyarrsize">array size</param>
	/// <param name="mych">CHAR_INFO to set</param>
    void drawPoly(const vec2int parr[], const unsigned int polyarrsize, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
    
	/// <summary>
	/// draws polygon from given vec2 array and size of array/amount of vertices and sets it to character and attribute you passed
	/// </summary>
	/// <param name="parr">array of the vertices in 2d space</param>
	/// <param name="polyarrsize">array size/length</param>
	/// <param name="mych">character to set</param>
	/// <param name="color">color/attribute to set</param>
	void drawPoly(const vec2int parr[], const unsigned int polyarrsize, const wchar_t mych, const smalluint color);

	/// <summary>
	/// draws regular polygon from given radius, position(vec2), side amount and CHAR_INFO
	/// </summary>
	/// <param name="myvec2">array of the vertices in 2d space</param>
	/// <param name="radius">array size</param>
	/// <param name="sideamount">amount of sides in polygon</param>
	/// <param name="mych">CHAR_INFO to set</param>
	void drawRegPoly(const vec2int& myvec2, const int radius, const int sideamount, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	
	/// <summary>
	/// draws regular polygon from given radius, position(vec2), side amount and char and attribute
	/// </summary>
	/// <param name="myvec2">array of the vertices in 2d space</param>
	/// <param name="radius">array size</param>
	/// <param name="sideamount">amount of sides in polygon</param>
	/// <param name="mych">character to set</param>
	/// <param name="color">color/attribute to set</param>
	void drawRegPoly(const vec2int& myvec2, const int radius, const int sideamount, const wchar_t mych, const smalluint color);
	
	/// <summary>
	/// draws circle with given radius and center point
	/// </summary>
	/// <param name="myvec2">center point</param>
	/// <param name="radius">radius of circle</param>
	/// <param name="mych">CHAR_INFO to set</param>
	void drawCircle(const vec2int& myvec2, const int radius, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);

	/// <summary>
	/// draws circle with given radius and center point
	/// </summary>
	/// <param name="myvec2">center point</param>
	/// <param name="radius">radius of circle</param>
	/// <param name="mych">character to set</param>
	/// <param name="color">color/attribute to set</param>
	void drawCircle(const vec2int& myvec2, const int radius, const wchar_t mych, const smalluint color);

	/// <summary>
	/// sets renderer's flags.
	/// </summary>
	/// <param name="rtype">flag number</param>
	/// <param name="val">bool to check</param>
	void renderflags(const int rtype, const bool val) {
		switch (rtype)
		{

		//auto/manual drawing
		case 0:
			if (val) {
				startrd();
			}
			else
			{
				closetrd();
			}
			break;
		//auto clear screen buffer after drawing
		case 2:
			m_clrscr = val;
			break;
		//triple buffering
		case 3:
			if (val) {
				createTripleBuffering();
			}
			else
			{
				removeTripleBuffering();
			}
			break;

		default:
			break;
		}
	}

    /// returns pixel at given location
	CHAR_INFO getpixel(const vec2int& myvec2) const;

	/// returns fps of the graphics thread(if running)
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
	
	//FIXME:rewrite the declarations to remove byte padding
	///the graphics thread code
	void mainthread(void);
	///opens and starts the drawing thread
	void startrd(void);
	///stops and closes the drawing thread
	void closetrd(void);
	///pointer AEScreen module for screen manipulations
	///needed as if to make it a variable, it would crash with "bad handle" error
	///because it creates the module before creating and assigning handles
	static AEScreen* m_myscr;
	///AEFrame module for thread drawing delay
	static AEFrame m_fr;
	///thread object to create drawing thread
	static std::thread m_thread;
	///variable to store window size
	static _SMALL_RECT m_rtwindow;
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




namespace artyk::utils {

	///returns color attribute of the cell from given backround and foreground
	constexpr inline smalluint getattrib(smalluint bgr = color::DEF_BGR, smalluint fgr = color::DEF_FGR) {
		return (bgr << 4) + fgr;
	}
}