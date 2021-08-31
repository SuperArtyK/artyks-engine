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
 *  Credits:
 *  Very big thanks to David "JavidX9" Barr, for helping write the code and for inspiration!
 *  Inspired by his olcConsoleGameEngine.
 *  https://github.com/OneLoneCoder/videos/blob/master/olcConsoleGameEngine.h
 *  Ravbug -- for giving ideas and pointing out errors
 *  Check him out: https://www.ravbug.com/
 * 
 *  Should not cause everything to break.
 */

#include "AEGraphic.hpp"
#include "func_utils.hpp"
#include "AETimer.hpp"

atomic<biguint> AEGraphic::m_globalmodulenum;
int AEGraphic::graph_fps = 0;
bool AEGraphic::m_threadon  =false;
CHAR_INFO* AEGraphic::m_screendata = nullptr;
CHAR_INFO* AEGraphic::m_currentbuffer = nullptr;
COORD AEGraphic::m_screensize;
atomic<int> AEGraphic::moduleamt = 0;
atomic<bool> AEGraphic::m_settingscreen = false;
bool AEGraphic::m_bstoptrd = false;
std::thread AEGraphic::m_thread;
_SMALL_RECT AEGraphic::m_rtwindow;
bool AEGraphic::m_clrscr;
AEScreen* AEGraphic::m_myscr = nullptr;
AEFrame AEGraphic::m_fr(GAME_FPS);



//FIXME:rewrite the initialisation to match variable declaration
AEGraphic::AEGraphic(short sizex, short sizey, short fonth, short fontw, int fpsdelay,bool enablelog, bool useGlobLog) : __AEBaseClass("AEGraphic", ++m_globalmodulenum)
{
	m_fr.setfps(fpsdelay);
	if (moduleamt == 0) {
		m_screensize = { sizex, sizey };
		m_screendata = new CHAR_INFO[sizex * sizey];
		memset(m_screendata, 0, sizex * sizey * PIXELSIZE);
		m_currentbuffer = m_screendata;
		//this is very very bad, but making m_myscr a variable, breaks the engine
		//with error code "bad handle". And the cause is the global variable declaration order
		m_myscr = new AEScreen;
	}
	moduleamt++;
	
	
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
	if (m_threadon) {
		setscreen(sizex, sizey, fonth, fontw);
	}
	else
	{
		m_myscr->setScreen(sizex, sizey, fonth, fontw);
	}
	
	m_rtwindow = { 0, 0,
		short(m_myscr->GetScreenRes().X-1), short(m_myscr->GetScreenRes().Y-1)};
	artyk::utils::normal_log(m_logptr, "Started Graphics module!", LOG_SUCCESS, m_modulename);
	startrd();
	
} 

AEGraphic::~AEGraphic(){
	moduleamt--;
	if (moduleamt == 0) {
		closetrd();
		if (m_screendata) {
			delete[] m_screendata;
			m_screendata = nullptr;
		}
		if (m_currentbuffer) {
			delete[] m_screendata;
			m_currentbuffer = nullptr;
		}
		if (m_myscr) {
			delete[] m_myscr;
			m_myscr = nullptr;
		}

	}
	
	artyk::utils::normal_log(m_logptr, "Closed Graphics module", LOG_SUCCESS, m_modulename);
	if(
#ifdef AE_GLOBALMODULE
		m_logptr != &global_logger &&
#endif // !AE_GLOBALMODULE
		m_logptr) {
		delete m_logptr;
	}
	

}

void AEGraphic::drawscreen() {
	SMALL_RECT temp = m_rtwindow;

	while (m_settingscreen)
	{
	}
	WriteConsoleOutputW(artyk::g_output_handle, m_screendata, m_screensize, { 0,0 }, &temp);
}

void AEGraphic::drawbuffer() {
	SMALL_RECT temp = m_rtwindow;

	while (m_settingscreen)
	{
	}
	WriteConsoleOutputW(artyk::g_output_handle, m_currentbuffer, m_screensize, { 0,0 }, &temp);
}

void AEGraphic::clearscreen() {
	m_settingscreen = true;
	memset(m_screendata, 0, m_screensize.X * m_screensize.Y * PIXELSIZE);
	m_settingscreen = false;
}

void AEGraphic::clearbuffer() {
	if (m_currentbuffer == nullptr) return;
	m_settingscreen = true;
	memset(m_currentbuffer, 0, m_screensize.X * m_screensize.Y * PIXELSIZE);
	m_settingscreen = false;
}

void AEGraphic::copybuffer(const CHAR_INFO* mych, int buffsize) {
	//check if our buffer is equals to our data buffer
	//(if nothing was passed and m_currentbuffer is equals to m_screendata

	if (mych == m_screendata) return;
	m_settingscreen = true;//atomic bool, blocks the drawing thread untill false
	if (buffsize > (m_screensize.X * m_screensize.Y)) {//precaution, if buffsize is more than screen size
		buffsize = m_screensize.X * m_screensize.Y;
	}
	memcpy(m_screendata, mych, buffsize * PIXELSIZE);//copying data...
	m_settingscreen = false;//removing blocking
}

void AEGraphic::createTripleBuffering(void) {
	if (m_currentbuffer && m_currentbuffer != m_screendata) {
		return;
	}
	m_currentbuffer = new CHAR_INFO[m_screensize.X * m_screensize.Y];
	memset(m_currentbuffer, 0, m_screensize.X * m_screensize.Y * PIXELSIZE);
}

void AEGraphic::removeTripleBuffering(void) {
	if (m_currentbuffer && m_currentbuffer != m_screendata) {
		return;
	}
	delete[] m_currentbuffer;
	m_currentbuffer = m_screendata;
}

CHAR_INFO* AEGraphic::getTripleBufferPtr(void) {
	if (m_currentbuffer && m_currentbuffer != m_screendata)
		return m_currentbuffer;
	else
		return nullptr;
}

void AEGraphic::setscreen(short swidth, short sheight, short fonth, short fontw) {
	
	
	
	m_settingscreen = true;
	m_myscr->setScreen(swidth, sheight, fonth, fontw);

	if (swidth != m_screensize.X || sheight != m_screensize.Y) {
		if (m_currentbuffer && m_currentbuffer != m_screendata) {
			delete[] m_currentbuffer;
			m_currentbuffer = new CHAR_INFO[swidth * sheight];
		}
		if (m_screendata) {
			delete[] m_screendata;
		}
		m_screendata = new CHAR_INFO[swidth * sheight];
		clearscreen();
		clearbuffer();
	}
	
	m_settingscreen = false;

}

void AEGraphic::setPixel(const vec2int& myvec2, const wchar_t mych, const smalluint color) {
	setPixel(myvec2, PIXEL(mych,color));
}
void AEGraphic::setPixel(const vec2int& myvec2, const CHAR_INFO& mych) {

#ifdef AE_GFX_ENABLE_WRAPPING
	m_currentbuffer[m_screensize.X * abs(myvec2.y % m_screensize.Y) + abs(myvec2.x % m_screensize.X)] = mych;
#else
	if (myvec2.x >= 0 && myvec2.x < m_screensize.X && myvec2.y >= 0 && myvec2.y < m_screensize.Y) {
		m_currentbuffer[m_screensize.X * myvec2.y + myvec2.x] = mych;
	}
#endif // !AE_GFX_ENABLE_WRAPPING
	//drawscreen();

}

void AEGraphic::fill(const vec2int& myvec2_1, const vec2int& myvec2_2, const CHAR_INFO& mych) {


	for (short y = myvec2_1.y; y < myvec2_2.y; y++) {
		for (short x = myvec2_1.x; x < myvec2_2.x; x++) {
			setPixel(vec2int{ x,y }, mych);
		}
	}
}

void AEGraphic::fill(const vec2int& myvec2_1, const vec2int& myvec2_2, const wchar_t mych, const smalluint color) {
	fill(myvec2_1, myvec2_2, PIXEL(mych, color));
}


void AEGraphic::fillRandom(const vec2int& myvec2_1, const vec2int& myvec2_2, const wchar_t mych) {
	for (short y = myvec2_1.y; y < myvec2_2.y; y++) {
		for (short x = myvec2_1.x; x < myvec2_2.x; x++) {
			setPixel(vec2int{ x,y }, mych,rand());
		}
	}
}


CHAR_INFO AEGraphic::getpixel(const vec2int& myvec2) const {

	if (m_screendata && myvec2.x > 0 && myvec2.x < m_screensize.X && myvec2.y > 0 && myvec2.y < m_screensize.Y) {
		return m_screendata[m_screensize.X * myvec2.y + myvec2.x];
	}
	return artyk::gfx::PX_EMPTY;
}

//just copied the bresenham's line algorithm
//added only the vertical, horisontal and diagonal(45deg) lines
void AEGraphic::drawLine(const vec2int& myvec2_1, const vec2int& myvec2_2, const CHAR_INFO& mych) {
	
	if (myvec2_1.x == myvec2_2.x && myvec2_1.y == myvec2_2.y) {
		setPixel(myvec2_1, mych);
		return;
	}

	const int
		deltaX = abs(myvec2_2.x - myvec2_1.x),
		deltaY = abs(myvec2_2.y - myvec2_1.y),
		signX = myvec2_1.x < myvec2_2.x ? 1 : -1,
		signY = myvec2_1.y < myvec2_2.y ? 1 : -1;
	vec2int temp = myvec2_1;
	if (deltaX == 0) {
		for (int i = 0; i <= deltaY; i++) {
			setPixel(temp, mych);
			temp.y += signY;
		}
		return;
	}
	if (deltaY == 0) {
		for (int i = 0; i <= deltaX; i++) {
			setPixel(temp, mych);
			temp.x += signX;
		}
		return;
	}
	if (deltaY == deltaX) {
		for (int i = 0; i <= deltaX; i++) {
			setPixel(temp, mych);
			temp.x += signX;
			temp.y += signY;
		}
		return;
	}

	int error = deltaX - deltaY;
	setPixel(myvec2_2, mych);
	while (temp.x != myvec2_2.x || temp.y != myvec2_2.y)
	{
		setPixel(temp, mych);
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

void AEGraphic::drawLine(const vec2int& myvec2_1, const vec2int& myvec2_2, const wchar_t mych, const smalluint color) {
	drawLine(myvec2_1, myvec2_2, PIXEL(mych, color));
}

void AEGraphic::drawTriangle(const vec2int& myvec2_1, const vec2int& myvec2_2, const vec2int& myvec2_3, const CHAR_INFO& mych) {
	drawLine(myvec2_1, myvec2_2, mych);
	drawLine(myvec2_2, myvec2_3, mych);
	drawLine(myvec2_3, myvec2_1, mych);
}

void AEGraphic::drawTriangle(const vec2int& myvec2_1, const vec2int& myvec2_2, const vec2int& myvec2_3, const wchar_t mych, const smalluint color) {
	drawTriangle(myvec2_1, myvec2_2, myvec2_3, PIXEL(mych, color));
}

void AEGraphic::drawPoly(const vec2int parr[], const unsigned int polyarrsize, const CHAR_INFO& mych){
        if(polyarrsize == 0 || parr == nullptr) return;
        vec2int prevpos{parr[0]};
        for(unsigned int i = 1; i < polyarrsize; i++){
            drawLine(prevpos,parr[i],mych);
            prevpos = parr[i];
        }
		drawLine(prevpos, parr[0]);
    }

void AEGraphic::drawPoly(const vec2int parr[], const unsigned int polyarrsize, const wchar_t mych, const smalluint color) {
	drawPoly(parr, polyarrsize, PIXEL(mych, color));
}

void AEGraphic::drawRegPoly(const vec2int& myvec2, const int radius, const int sideamount, const CHAR_INFO& mych) {
	//its a point anyway
	if (radius == 1) {
		setPixel(myvec2, mych);
		return;
	}
	//or its a line
	if (sideamount < 3) {
		drawLine(vec2int{ myvec2.x, myvec2.y - radius }, vec2int{ myvec2.x, myvec2.y + radius }, mych);
		return;
	}
	const float angleincrement = 360.0f / sideamount;
	vec2int pointpos;
	vec2int prevpos{
		artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(-90) * radius),
		artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(-90) * radius)
	};
	for (float i = -90 + angleincrement; i < 360; i += angleincrement) {
		pointpos.x = artyk::math::roundtoint(myvec2.x + artyk::math::cosdeg(i) * radius);
		pointpos.y = artyk::math::roundtoint(myvec2.y + artyk::math::sindeg(i) * radius);
		drawLine(pointpos, prevpos, mych);
		prevpos = pointpos;
	}

}

void AEGraphic::drawRegPoly(const vec2int& myvec2, const int radius, const int sideamount, const wchar_t mych, const smalluint color) {
	drawRegPoly(myvec2, radius, sideamount, PIXEL(mych, color));
}

void AEGraphic::drawCircle(const vec2int& myvec2, const int radius, const CHAR_INFO& mych) {
	

	switch (radius)
	{
		//just dot
	case 0:
		return;
		break;

		//crosshair without middle dot
	case 1:
		setPixel({ myvec2.x - 1, myvec2.y }, mych);
		setPixel({ myvec2.x + 1, myvec2.y }, mych);
		setPixel({ myvec2.x, myvec2.y - 1 }, mych);
		setPixel({ myvec2.x, myvec2.y + 1 }, mych);
		return;
		break;

		//5x5 square without corner pixels
	case 2:
		//top line
		setPixel({ myvec2.x - 1, myvec2.y - 2 }, mych);
		setPixel({ myvec2.x, myvec2.y - 2 }, mych);
		setPixel({ myvec2.x + 1, myvec2.y - 2 }, mych);
		//bottom line
		setPixel({ myvec2.x - 1, myvec2.y + 2 }, mych);
		setPixel({ myvec2.x, myvec2.y + 2 }, mych);
		setPixel({ myvec2.x + 1, myvec2.y + 2 }, mych);
		//left line
		setPixel({ myvec2.x - 2, myvec2.y - 1 }, mych);
		setPixel({ myvec2.x - 2, myvec2.y}, mych);
		setPixel({ myvec2.x - 2, myvec2.y + 1 }, mych);
		//right line
		setPixel({ myvec2.x + 2, myvec2.y - 1 }, mych);
		setPixel({ myvec2.x + 2, myvec2.y }, mych);
		setPixel({ myvec2.x + 2, myvec2.y + 1 }, mych);
		return;
		break;
	default:
		int x = 0;
		int y = radius;
		int delta = 1 - 2 * radius;
		int error;

		while (y >= x) {
			setPixel({ myvec2.x + x, myvec2.y + y }, mych);
			setPixel({ myvec2.x + x, myvec2.y - y }, mych);
			setPixel({ myvec2.x - x, myvec2.y + y }, mych);
			setPixel({ myvec2.x - x, myvec2.y - y }, mych);
			setPixel({ myvec2.x + y, myvec2.y + x }, mych);
			setPixel({ myvec2.x + y, myvec2.y - x }, mych);
			setPixel({ myvec2.x - y, myvec2.y + x }, mych);
			setPixel({ myvec2.x - y, myvec2.y - x }, mych);
			error = 2 * (delta + y) - 1;
			if ((delta < 0) && (error <= 0)) {
				delta += 2 * ++x + 1;
				continue;
			}
			if ((delta > 0) && (error > 0)) {
				delta -= 2 * --y + 1;
				continue;
			}
			delta += 2 * (++x - --y);
		}
		break;
	}


}

void AEGraphic::drawCircle(const vec2int& myvec2, const int radius, const wchar_t mych, const smalluint color) {
	drawCircle(myvec2, radius, PIXEL(mych, color));
}


void AEGraphic::startrd(void) {
	if (m_threadon) {
		return;
	}
	m_bstoptrd = false;
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
		artyk::utils::debug_log(m_logptr, "Closed drawing thread.", LOG_OK, m_modulename);
	}
	else {
		artyk::utils::debug_log(m_logptr, "Could not close drawing thread:it wasn't started", LOG_ERROR, m_modulename);
	}


}

void AEGraphic::mainthread(void) {
	using namespace artyk::gfx;
	
	artyk::utils::debug_log(m_logptr, "Entered main drawing thread", LOG_SUCCESS, m_modulename);
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	
	float timeelapsed;
	int i = 0, previ = i;
	artyk::utils::waitfortick();
	systime timeend, timestart = std::chrono::system_clock::now();
	while (!m_bstoptrd)
	{
		
		drawscreen();
#ifdef AE_GFX_ALWAYS_CLEAR_AFTER_DRAW
		clearscreen();
#else
		if (m_clrscr)
			clearscreen();
#endif // AE_GFX_ALWAYS_CLEAR_AFTER_DRAW

		
		timeend = std::chrono::system_clock::now();
		timeelapsed = std::chrono::duration<float>(timeend - timestart).count();
		if (timeelapsed > 1.0f) {
			//i -= previ;
			graph_fps = i - previ;
			previ = i;
			timestart = timeend;
			
		}
		m_fr.sleep();
		i++;
	}
	artyk::utils::normal_log(m_logptr, "Closed main drawing thread", LOG_SUCCESS, m_modulename);
	graph_fps = 0;

}


