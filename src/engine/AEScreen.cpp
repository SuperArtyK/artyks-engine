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

/** @file src/engine/AEScreen.cpp
 *  This file contains the screen module functions definitions.
 * 
 *  Should not cause everything to break.
 */

#include "include/engine/AEScreen.hpp" 
#include "include/engine/func_system.hpp"
using std::string;
using std::atomic;
using std::to_string;	
using namespace artyk;

biguint AEScreen::m_globalmodulenum = 0;
COORD AEScreen::m_screenres;//our screen resolution
CONSOLE_SCREEN_BUFFER_INFOEX AEScreen::g_rgb_color;
CONSOLE_SCREEN_BUFFER_INFOEX AEScreen::g_normal_color;
CONSOLE_SCREEN_BUFFER_INFO AEScreen::g_csbi;
CONSOLE_FONT_INFOEX AEScreen::g_cfi;
SMALL_RECT AEScreen::g_rectWindow = { 0,0,0,0 };



//constructors/destructors
AEScreen::AEScreen(bool enablelog, bool useGlobLog) :__AEBaseClass("AEScreen",++m_globalmodulenum)
//, interruptscroll(false)
{
#ifdef AE_LOG_ENABLE
	if (enablelog) {
		if (useGlobLog) {
#ifdef AE_GLOBALMODULE
			m_logptr = &global_logger;

#else
			//std::cout << "using not global logger";
			m_logptr = new AELog(DEF_LOG_PATH, "SCRN");
			artyk::utils::debug_log(m_logptr,"You have set the useGlobLog flag on, but AE_GLOBALMODULE is not defined!", LOG_SWARN, m_modulename);
			artyk::utils::debug_log(m_logptr,"Falling back to default: Not using global logger", LOG_SWARN, m_modulename);

#endif
		}
		else
		{
			m_logptr = new AELog(DEF_LOG_PATH, "SCRN");

		}
		
		
	}
	artyk::utils::normal_log(m_logptr, "Started AEScreen module!", LOG_SUCCESS, m_modulename);

#endif // AE_LOG_DISABLE



#ifdef AE_GLOBALMODULE
	if (m_modulenum != 1) {
		checkhandles();
	}
#else
	checkhandles();
#endif // AE_GLOBALMODULE

	
	
	m_screenres = g_csbi.dwSize;

	//setcolor_con(AESColor::DEF_BGR, AESColor::DEF_FGR);
	
}

AEScreen::~AEScreen() { 
	
	artyk::utils::normal_log(m_logptr, "Closed Screen module!", LOG_SUCCESS, m_modulename);
	if (
#ifdef AE_GLOBALMODULE
		m_logptr != &global_logger &&
#endif // !AE_GLOBALMODULE
		m_logptr) {
		delete m_logptr;
	}

}

//other stuff

void AEScreen::setcursor(short x, short y)
{//sets the output cursor to x/y pos of the screen
	m_screenres = g_csbi.dwSize;
	if (y < 0) {
		y = 0;
	}
	if (x < 0) {
		x = 0;
	}

	//std::cout.flush();
	SetConsoleCursorPosition(artyk::g_output_handle, { x, y });
	artyk::utils::debug_log(m_logptr, "Put cursor to: " + to_string(x) + " " + to_string(y), LOG_INFO, m_modulename);

}

void AEScreen::setBufferSize(short x, short y) {//sets console buffer size
	//units are character cells

	COORD coord = { x,y };
	SMALL_RECT Rect = { 0,0,short(x - 1),short(y - 1) };
	m_screenres = coord;

	SetConsoleScreenBufferSize(artyk::g_output_handle, coord);
	SetConsoleWindowInfo(artyk::g_output_handle, TRUE, &Rect);
	artyk::utils::debug_log(m_logptr, "Set console buffer size to: " + to_string(x) + "x" + to_string(y), LOG_INFO, m_modulename);
}

void AEScreen::setScreen(short swidth, short sheight, short fonth , short fontw)
{
	using namespace artyk;
	m_screenres = {swidth, sheight};

	//change console visual size to a minimum so ScreenBuffer can shrink
	//below the actual visual size
	artyk::utils::debug_log(m_logptr, "----------START SCREEN SETTING----------", LOG_INFO, m_modulename);
	g_rectWindow = { 0, 0, 1, 1 };
	if (!SetConsoleWindowInfo(g_output_handle, TRUE, &g_rectWindow)) {
		artyk::utils::FError_log(m_logptr, "Could not SetConsoleWindowInfo!\nFunction error code: " + to_string(GetLastError()), m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_SETTING_SCREEN);
	}
	artyk::utils::debug_log(m_logptr, "Set console window info to: 0,0,1,1", LOG_OK, m_modulename);
	//set the size of the screen buffer
	if (!SetConsoleScreenBufferSize(g_output_handle, {swidth, sheight})) {
		artyk::utils::debug_log(m_logptr, "Could not SetConsoleScreenBufferSize!\nFunction error code: " + to_string(GetLastError()), LOG_ERROR,m_modulename);
	}
	// the reason I took it out of the fatal error call, is that it somehow fails and sets the size anyways
	// we only care about the size setting
	//SetConsoleScreenBufferSize(g_output_handle, { swidth, sheight });
	//setBufferSize(swidth, sheight);

	artyk::utils::debug_log(m_logptr, "Set console buffer to: " +to_string(swidth)+"x"+to_string(sheight), LOG_OK, m_modulename);
	//assign screen buffer to the console
	if (!SetConsoleActiveScreenBuffer(g_output_handle)) {
		artyk::utils::FError_log(m_logptr,"Could not SetConsoleActiveScreenBuffer!\nFunction error code: " + to_string(GetLastError()), m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_SETTING_SCREEN);
	}
	artyk::utils::debug_log(m_logptr, "Set console's active screen buffer to: "+utils::addrtostr(g_output_handle), LOG_OK, m_modulename);
	artyk::utils::debug_log(m_logptr, "Setting console font", LOG_INFO, m_modulename);

	//set the font info
	g_cfi.cbSize = sizeof(g_cfi);
	g_cfi.nFont = 0;
	g_cfi.dwFontSize.X = (fontw < 2) ? 2 : fontw;
	g_cfi.dwFontSize.Y = (fonth < 2) ? 2 : fonth;
	g_cfi.FontFamily = FF_DONTCARE;
	g_cfi.FontWeight = FW_NORMAL;

	//set the name of font
	wcscpy_s(g_cfi.FaceName, L"Lucida Console");
	//assign font info to console
	if (!SetCurrentConsoleFontEx(g_output_handle, false, &g_cfi)) {
		artyk::utils::FError_log(m_logptr, "Could not SetCurrentConsoleFontEx!\nFunction error code: " + to_string(GetLastError()), m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_SETTING_SCREEN);
	}
	artyk::utils::debug_log(m_logptr, "Set console font to: Consolas " + to_string(fontw) + "x" + to_string(fonth), LOG_OK, m_modulename);

	artyk::utils::debug_log(m_logptr, "Assigned the console font", LOG_OK, m_modulename);

	//getting buffer info for further checking
	if (!GetConsoleScreenBufferInfo(g_output_handle, &g_csbi)) {
		artyk::utils::FError_log(m_logptr, "Could not GetConsoleScreenBufferInfo!\nFunction error code: " + to_string(GetLastError()), m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_SETTING_SCREEN);
	}

	artyk::utils::debug_log(m_logptr, "Got console screen buffer information", LOG_OK, m_modulename);


	//checking if asked console size is in max allowed range
	if (sheight > g_csbi.dwMaximumWindowSize.Y) {//checking it first, as people mostly mess this one up more often
		artyk::utils::FError_log(m_logptr,
			"Window size.\n"
			"The console height you set is too big for this screen! Decrease it or something!\n"
			"\nWith the font set: " + to_string(fontw) + "x" + to_string(fonth) +
			"\nThe Allowed size is: " + to_string(g_csbi.dwMaximumWindowSize.X) + "x" + to_string(g_csbi.dwMaximumWindowSize.Y) +
			"\nWhile Asked for: " + to_string(swidth) + "x" + to_string(sheight) +
			"\nFunction error code : " + to_string(GetLastError()),
			m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_SETTING_SCREEN);
	}

	artyk::utils::debug_log(m_logptr, "Asked height is in range of dwMaximumWindowSize.Y of: "+to_string(g_csbi.dwMaximumWindowSize.Y), LOG_INFO, m_modulename);

	if (swidth > g_csbi.dwMaximumWindowSize.X) {
		artyk::utils::FError_log(m_logptr,
			"Window size.\n"
			"The console height you set is too big for this screen! Decrease it or something!\n"
			"\nWith the font set: " + to_string(fontw) + "x" + to_string(fonth) +
			"\nThe Allowed size is: " + to_string(g_csbi.dwMaximumWindowSize.X) + "x" + to_string(g_csbi.dwMaximumWindowSize.Y) +
			"\nWhile Asked for: " + to_string(swidth) + "x" + to_string(sheight) +
			"\nFunction error code : " + to_string(GetLastError()),
			m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_SETTING_SCREEN);
	}

	artyk::utils::debug_log(m_logptr, "Asked width is in range of dwMaximumWindowSize.X of: " + to_string(g_csbi.dwMaximumWindowSize.X), LOG_INFO, m_modulename);
	artyk::utils::debug_log(m_logptr, "Console screen resolution is in range", LOG_OK, m_modulename);


	//setting the size itself
	g_rectWindow = { 0, 0, short(swidth - 1), short(sheight - 1) };
	if (!SetConsoleWindowInfo(g_output_handle, TRUE, &g_rectWindow)) {
		artyk::utils::FError_log(m_logptr, "Could not SetConsoleWindowInfo!\nFunction error code: " + to_string(GetLastError()), m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_SETTING_SCREEN);

	}
	artyk::utils::debug_log(m_logptr, "Window set successfully to: "+to_string(swidth)+"x"+to_string(sheight) + " with font: Consolas " + to_string(fontw) + "x" + to_string(fonth), LOG_INFO, m_modulename);
	
	artyk::utils::debug_log(m_logptr, "----------END SCREEN SETTING----------", LOG_INFO, m_modulename);
	//our beloved flags, probably will be useful
	// 	   --Past ArtyK
// 	if (!SetConsoleMode(g_hStdIn, ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)) {
// 		artyk::Error("SetConsoleMode", "Could not SetConsoleMode!");
// 	}
	//future ArtyK here -- it will be useful only(!) for the full blown graphics engine or keyboard scanner(so we can read the mouse pos)
	//not our "simple screen manipulation" class



// 	GetWindowRect(artyk::screen::g_console, &m_r);
// 	//MoveWindow(artyk::screen::g_console, m_r.left, m_r.top, x, y, TRUE);
	
}

void AEScreen::settitle(const string& title) {//speaks for itself
		//sets app title
	using namespace artyk::utils;
	SetConsoleTitleA((
#ifdef AE_ADD_APP_TITLE

#ifdef AE_ADD_APP_VERSION

	((!findinstr(title, artyk::app_name_full)) ? app_name_full : "")
		+
#else
	((!findinstr(title, artyk::app_name)) ? app_name : "")
		+
#endif // AE_ADD_APP_TITLE
		" | "+

#endif // AE_ADD_APP_TITLE




	title).c_str());
	//artyk::utils::debug_log(m_logptr, "Set window title to: " + title, LOG_INFO, m_modulename);

	
}

void AEScreen::setFont(short y, short x, const string& fontname) {//sets font size

	//ZeroMemory(&m_cfi, sizeof(m_cfi));
	g_cfi.cbSize = sizeof(g_cfi);
	g_cfi.nFont = 0;
	g_cfi.dwFontSize.X = (x<2)?2:x;                   // Width of each character in the font
	g_cfi.dwFontSize.Y = (y < 2) ? 2 : y;                  // Height
	g_cfi.FontFamily = FF_DONTCARE;
	g_cfi.FontWeight = FW_NORMAL;
	
	wcscpy_s(g_cfi.FaceName, artyk::utils::widen_str(fontname).c_str()); // Choose your font
	SetCurrentConsoleFontEx(artyk::g_output_handle, true, &g_cfi);

	artyk::utils::debug_log(m_logptr, "Set font to: " + fontname +" " +to_string(y)+"x"+to_string(x), LOG_OK, m_modulename);

}

void AEScreen::clear(void) {//clears screen; much faster than system() but still slow for "draw frame, clear, draw again" scheme

	COORD topLeft = { 0, 0 };
	DWORD written;

	//GetConsoleScreenBufferInfo(artyk::screen::g_hStdOut, &g_csbi);
	FillConsoleOutputCharacter(
		artyk::g_output_handle, ' ', g_csbi.dwSize.X * g_csbi.dwSize.Y, topLeft, &written
	);
	FillConsoleOutputAttribute(
		artyk::g_output_handle, AEScreen::BLACK,
		g_csbi.dwSize.X * g_csbi.dwSize.Y, topLeft, &written
	);
	setcursor(0, 0);

	artyk::utils::debug_log(m_logptr, "Cleared console screen", LOG_INFO, m_modulename);

}

void AEScreen::setcolor_con(const smalluint back, const smalluint fore) {//sets color of text out of 16 color pallete


	//SetConsoleScreenBufferInfoEx(artyk::screen::g_hStdOut, &g_normal_color); // this is for setcolor_rgb() but completely breaks the pallete
	SetConsoleTextAttribute(artyk::g_output_handle, back * 16 + fore);
	artyk::utils::debug_log(m_logptr, "Set console color to:" + to_string(back * 16 + fore), LOG_INFO, m_modulename);

}


#ifdef AE_EXPERIMENTAL
void AEScreen::scrolltitle(const string& title, short delay, short scrolloffset) {

	//scrolls title, doesnt really work right now, so dont use it
	//FIXME:Remove thread blocking

	//if (title.empty()) {
	//	return;
	//}
	//interruptscroll = false;
	//string temp = title;
	//int i = 0;
	//while (!interruptscroll) {
	//	if (i >= temp.length()) {
	//		i = 0;
	//	}
	//	temp.erase(0, 1);
	//	temp += title[i];
	//	settitle(temp);
	//	Sleep(delay);
	//	i++;
	//}


}


void AEScreen::setcolor_rgb(smalluint fred, smalluint fgreen, smalluint fblue) {
	//doesnt work as I wanted it to(set rgb color to the chars separately, not limited by 16 color pallete)
	//if you'll fork this engine, plz find a way to fix this
	//it changes the whole pallete list for the console, kind of like CGA graphics
	//so you cant have all different colors at the same time, only 16

//  fred - foreground red
//  fgreen - foreground green
//  fblue - foreground blue

	g_rgb_color.ColorTable[1] = RGB(fred, fgreen, fblue);

	SetConsoleScreenBufferInfoEx(artyk::g_output_handle, &g_rgb_color);
	SetConsoleTextAttribute(artyk::g_output_handle, 1);

	artyk::utils::debug_log(m_logptr, "(rgb_pallete)Setting console color to:" + to_string(fred) + " " + to_string(fgreen) + " " + to_string(fblue), LOG_INFO, m_modulename);

}
#endif

bool AEScreen::checkhandles(void) {


	artyk::utils::debug_log(m_logptr, "Checking handles", LOG_INFO, m_modulename);

	if (!IsWindow(g_console_hwnd)) {
		artyk::utils::FError_log(m_logptr, "IsWindow(g_console)\n\"g_console\" is not a valid window handle!", m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::BAD_SCREEN_HANDLE);
		return false;
	}


	if (artyk::g_input_handle == INVALID_HANDLE_VALUE) {
		artyk::utils::FError_log(m_logptr, "m_hStdIn is INVALID_HANDLE_VALUE!", m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::BAD_SCREEN_HANDLE);
		return false;
	}
	if (artyk::g_output_handle == INVALID_HANDLE_VALUE) {
		artyk::utils::FError_log(m_logptr, "m_hStdOut is INVALID_HANDLE_VALUE!", m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::BAD_SCREEN_HANDLE);
		return false;
	}



	artyk::utils::debug_log(m_logptr, "These are some good handles", LOG_INFO, m_modulename);


	return true;
}


