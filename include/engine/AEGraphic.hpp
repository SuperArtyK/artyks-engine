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
	
	inline void drawscreen() const;
	inline void clearscreen();
	inline void setpixel(short x, short y, wchar_t mych, smalluint color);
	inline void setpixel(short x, short y, const CHAR_INFO& mych = artyk::gfx::PX_BLOCK);
	constexpr static inline smalluint calcColor(smalluint bgr = artyk::color::DEF_BGR, smalluint fgr = artyk::color::DEF_FGR){
		return bgr * 16 + fgr;
	}
	static inline int getfps(void) {
		return graph_fps;
	}

private:
	///FIXME:rewrite the declarations to remove byte padding
	void startrd(void);

	void closetrd(void);

	void mainthread(void);
	bool m_bstoptrd;
	AEScreen m_myscr;
	atomic<CHAR_INFO*> m_screendata;
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

#endif // !AEGRAPHIC_HPP




