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

/** @file src/engine/AEGraphic.cpp
 *  This file contains the graphics engine functions definitions.
 *
 *  Should not cause everything to break.
 */

#include "AEGraphic.hpp"
#include "func_utils.hpp"

atomic<biguint> AEGraphic::m_globalmodulenum;
int AEGraphic::graph_fps = 0;
bool AEGraphic::m_threadon  =false;


///FIXME:rewrite the initialisation to match variable declaration
AEGraphic::AEGraphic(short sizex, short sizey, short fonth, short fontw, int fpsdelay,bool enablelog, bool useGlobLog) : __AEBaseClass("AEGraphic", ++m_globalmodulenum), 
m_myscr(false), m_screendata(new CHAR_INFO[sizex * sizey]), m_fr(fpsdelay), m_screensize{ sizex, sizey }, m_bstoptrd(false), m_settingscreen(false)
{
	memset(m_screendata, 0, sizex* sizey * sizeof(CHAR_INFO));
#ifdef AE_LOG_ENABLE
	if (enablelog) {
		if (useGlobLog) {
#ifdef AE_GLOBALMODULE
			m_logptr = &global_logger;
			artyk::utils::normal_log(m_logptr, "Using global logger", LOG_INFO, m_modulename);
#else
			m_logptr = new AELog(DEF_LOG_PATH, "GFX");
			artyk::utils::debug_log(m_logptr, "You have set the useGlobLog flag on, but AE_GLOBALMODULE is not defined!", LOG_SWARN, m_modulename);
			artyk::utils::debug_log(m_logptr, "Falling back to default:Not using global modules", LOG_SWARN, m_modulename);
#endif
		}
		else
		{
			m_logptr = new AELog(DEF_LOG_PATH, "GFX");
		}
	}

#endif // AE_LOG_DISABLE
	m_myscr.setScreen(sizex, sizey, fonth, fontw);
	m_rtwindow = { 0, 0,
		short(m_myscr.GetScreenRes().X-1), short(m_myscr.GetScreenRes().Y-1)};
	artyk::utils::normal_log(m_logptr, "Started Graphics module!", LOG_SUCCESS, m_modulename);
	startrd();
	
} 

AEGraphic::~AEGraphic(){
	closetrd();
	artyk::utils::normal_log(m_logptr, "Closed Graphics module", LOG_SUCCESS, m_modulename);
	if(
#ifdef AE_GLOBALMODULE
		m_logptr != &global_logger &&
#endif // !AE_GLOBALMODULE
		m_logptr) {
		delete m_logptr;
	}
	if (m_screendata) {
		delete[] m_screendata;
	}

}

void AEGraphic::drawscreen() {
	SMALL_RECT temp = m_rtwindow;
	if(!m_settingscreen)
	WriteConsoleOutputW(artyk::g_output_handle, m_screendata, m_screensize, { 0,0 }, &temp);
}

void AEGraphic::clearscreen() {
	memset(m_screendata, 0, m_screensize.X * m_screensize.Y * sizeof(CHAR_INFO));
}

void AEGraphic::setpixel(short x, short y, wchar_t mych, smalluint color) {
	if(x >= 0 && x < m_screensize.X&& y >= 0 && y < m_screensize.Y){
		m_screendata[m_screensize.X * y + x].Char.UnicodeChar = mych;
		m_screendata[m_screensize.X * y + x].Attributes = color;
	}
}
void AEGraphic::setpixel(short x, short y, const CHAR_INFO& mych) {
	if (x >= 0 && x < m_screensize.X && y >= 0 && y < m_screensize.Y) {
		m_screendata[m_screensize.X * y + x] = mych;
	}
}

void AEGraphic::fill(short x1, short y1, short x2, short y2, const CHAR_INFO& mych) {
	for (short i = y1; i < y2; i++) {
		for (short a = x1; a < x2; a++) {
			setpixel(a, i, mych);
		}
	}
}

void AEGraphic::fill(short x1, short y1, short x2, short y2, wchar_t mych, smalluint color) {
	for (short i = y1; i < y2; i++) {
		for (short a = x1; a < x2; a++) {
			setpixel(a, i, mych, color);
		}
	}
}

void AEGraphic::fillrandom(short x1, short y1, short x2, short y2, wchar_t mych) {
	for (short i = y1; i < y2; i++) {
		for (short a = x1; a < x2; a++) {
			setpixel(a, i, mych, a);
		}
	}
}


void AEGraphic::startrd() {
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

void AEGraphic::closetrd(void) {
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

void AEGraphic::mainthread() {
	using namespace artyk::gfx;
	artyk::utils::debug_log(m_logptr, "Entered main drawing thread", LOG_SUCCESS, m_modulename);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	auto timestart = std::chrono::system_clock::now();
	auto timeend = std::chrono::system_clock::now();
	float timeelapsed;
	bigint i = 0, previ = i;
	
	while (!m_bstoptrd)
	{
		m_fr.sleep();

		drawscreen();
		timeend = std::chrono::system_clock::now();
		timeelapsed = std::chrono::duration<float>(timeend - timestart).count();
		if (timeelapsed > 1.0f) {
			//i -= previ;
			graph_fps = i - previ;
			previ = i;
			timestart = timeend;
			
		}
		i++;

	}
	artyk::utils::normal_log(m_logptr, "Closed main drawing thread", LOG_SUCCESS, m_modulename);
	graph_fps = 0;

}


