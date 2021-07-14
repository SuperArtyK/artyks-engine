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
	void drawscreen() {
		WriteConsoleOutput(artyk::g_output_handle, m_screendata, m_screensize, { 0,0 }, &m_myscr.g_rectWindow);
	}
	inline void clearscreen()  {
		memset(m_screendata, 0, m_screensize.X * m_screensize.Y * sizeof(CHAR_INFO));
	}
	void setpixel(short x, short y, wchar_t mych, smalluint color) {
		m_screendata[m_screensize.X * y + x].Char.UnicodeChar = mych;
		m_screendata[m_screensize.X * y + x].Attributes = color;
	}


	constexpr static inline smalluint calcColor(smalluint bgr = artyk::color::DEF_BGR, smalluint fgr = artyk::color::DEF_FGR){
		return bgr * 16 + fgr;
	}

private:
	void startrd() {
		if (m_threadon) {
			return;
		}
		m_thread = std::thread(&AEGraphic::mainthread, this);
		if (m_thread.joinable()) {
			m_threadon = true;
			artyk::utils::debug_log(m_logptr, "Started drawing thread.", LOG_OK, m_modulename);
		}
		else {
			artyk::utils::debug_log(m_logptr, "Could NOT start drawing thread:reason unknown", LOG_ERROR, m_modulename);

		}
	}

	void closetrd(void) {
		if (m_thread.joinable()) {
			m_bstoptrd = true;
			m_thread.join();
			m_threadon = false;
			artyk::utils::debug_log(m_logptr, "Closed keyscanning thread.", LOG_OK, m_modulename);
		}
		else {
			artyk::utils::debug_log(m_logptr, "Could not close drawing thread:it wasn't started", LOG_ERROR, m_modulename);
		}


	}

	void mainthread();
	bool m_bstoptrd;
	AEScreen m_myscr;
	CHAR_INFO* m_screendata;
	COORD m_screensize;
	///frame limiter of the graphic thread
	AEFrame m_fr;
	///thread itself
	std::thread m_thread;
	///variable to store the module index number
	static atomic<biguint> m_globalmodulenum;
	static int graph_fps;
	static bool m_threadon;
};

namespace artyk {
	
	//const wchar_t
	//	FILLING[5]{
	//		L' ',L'░',L'▒',L'▓',L'█'
	//}
	//
	//;
	namespace gfx {
		constexpr wchar_t
			CH_EMPTY = L'\0',//no difference in drawing, but a difference in calculating, use this to indicate empty/transparent stuff
			CH_SPACE = L' ',
			CH_25 = L'░',
			CH_50 = L'▒',
			CH_75 = L'▓',
			CH_100 = L'█',
			CH_BLOCK = CH_100,
			CH_BOX1[]{
				L'┌', L'┬', L'┐', L'┤', L'┘', L'┴', L'└', L'├', L'┼', L'─', L'│',
			},
			CH_BOX2[]{
				L'╒', L'╤', L'╕', L'╡', L'╛', L'╧', L'╘', L'╞', L'╪', L'═', L'│',
			},
			CH_BOX3[]{
				L'╓', L'╥', L'╖', L'╢', L'╜', L'╨', L'╙', L'╟', L'╫', L'─', L'║',
			},
			CH_BOX4[]{
				L'╔', L'╦', L'╗', L'╣', L'╝', L'╩', L'╚', L'╠', L'╬', L'═', L'║',
			}

			;
		constexpr CHAR_INFO
			PX_EMPTY = { L'\0', 0},
			PX_25 = { L' ', artyk::color::DEF_ATTR },
			PX_50 = { L' ', artyk::color::DEF_ATTR },
			PX_75 = { L' ', artyk::color::DEF_ATTR },
			PX_100 = { L' ', artyk::color::DEF_ATTR },
			PX_BLOCK = PX_100,
			PX_BOX1[]{
				{L'┌', artyk::color::DEF_ATTR}, 
				{L'┬', artyk::color::DEF_ATTR}, 
				{L'┐', artyk::color::DEF_ATTR}, 
				{L'┤', artyk::color::DEF_ATTR}, 
				{L'┘', artyk::color::DEF_ATTR}, 
				{L'┴', artyk::color::DEF_ATTR}, 
				{L'└', artyk::color::DEF_ATTR}, 
				{L'├', artyk::color::DEF_ATTR}, 
				{L'┼', artyk::color::DEF_ATTR}, 
				{L'─', artyk::color::DEF_ATTR},
				{L'│', artyk::color::DEF_ATTR}, 
			},
			PX_BOX2[]{
				{L'╒', artyk::color::DEF_ATTR},
				{L'╤', artyk::color::DEF_ATTR},
				{L'╕', artyk::color::DEF_ATTR},
				{L'╡', artyk::color::DEF_ATTR},
				{L'╛', artyk::color::DEF_ATTR},
				{L'╧', artyk::color::DEF_ATTR},
				{L'╘', artyk::color::DEF_ATTR},
				{L'╞', artyk::color::DEF_ATTR},
				{L'╪', artyk::color::DEF_ATTR},
				{L'═', artyk::color::DEF_ATTR},
				{L'│', artyk::color::DEF_ATTR},
			},
			PX_BOX3[]{
				{L'╓', artyk::color::DEF_ATTR},
				{L'╥', artyk::color::DEF_ATTR},
				{L'╖', artyk::color::DEF_ATTR},
				{L'╢', artyk::color::DEF_ATTR},
				{L'╜', artyk::color::DEF_ATTR},
				{L'╨', artyk::color::DEF_ATTR},
				{L'╙', artyk::color::DEF_ATTR},
				{L'╟', artyk::color::DEF_ATTR},
				{L'╫', artyk::color::DEF_ATTR},
				{L'─', artyk::color::DEF_ATTR},
				{L'║', artyk::color::DEF_ATTR},
			},
			PX_BOX4[]{
				{L'╔', artyk::color::DEF_ATTR},
				{L'╦', artyk::color::DEF_ATTR},
				{L'╗', artyk::color::DEF_ATTR},
				{L'╣', artyk::color::DEF_ATTR},
				{L'╝', artyk::color::DEF_ATTR},
				{L'╩', artyk::color::DEF_ATTR},
				{L'╚', artyk::color::DEF_ATTR},
				{L'╠', artyk::color::DEF_ATTR},
				{L'╬', artyk::color::DEF_ATTR},
				{L'═', artyk::color::DEF_ATTR},
				{L'║', artyk::color::DEF_ATTR},
			}


			;


	}
	




}


#endif // !AEGRAPHIC_HPP




