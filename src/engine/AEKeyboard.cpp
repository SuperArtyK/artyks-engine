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

/** @file src/engine/AEKeyboard.cpp
 *  This file contains the keyboard scanning functions' definitions.
 * 
 *  Should not cause everything to break.
 */

#include "include/engine/AEKeyboard.hpp"
#include <vector>


AEKBKey AEKeyboard::m_keys[256];
short AEKeyboard::m_keyOld[256];
std::thread AEKeyboard::m_kbtrd;
atomic<biguint> AEKeyboard::m_globalmodulenum = 0;
atomic<biguint> AEKeyboard::moduleamt = 0;
short AEKeyboard::m_mousepos[2] = { 0,0 };
bool AEKeyboard::m_threadon = false;
bool AEKeyboard::m_enablemouse;
bool AEKeyboard::m_bstoptrd = false;



//constructors and destructors
AEKeyboard::AEKeyboard(bool enablelog, bool useGlobLog, bool enablemouse) : __AEBaseClass("AEKeyboard",++m_globalmodulenum)
{
	
	m_enablemouse = enablemouse;
#ifdef AE_LOG_ENABLE
	if (enablelog) {
		if (useGlobLog) {
#ifdef AE_GLOBALMODULE
			m_logptr = &global_logger;
			artyk::utils::normal_log(m_logptr, "Using global logger", LOG_INFO, m_modulename);
#else
			//std::cout << "using not global logger";
			m_logptr = new AELog(DEF_LOG_PATH, "KB");
			artyk::utils::debug_log(m_logptr, "You have set the useGlobLog flag on, but AE_GLOBALMODULE is not defined!", LOG_SWARN, m_modulename);
			artyk::utils::debug_log(m_logptr, "Falling back to default: Not using global modules", LOG_SWARN, m_modulename);
#endif


		}
		else
		{
			m_logptr = new AELog(DEF_LOG_PATH, "KB");

		}

	}
	
	
artyk::utils::normal_log(m_logptr, "Started AEKeyboard module!", LOG_SUCCESS, m_modulename);

#endif // AE_LOG_DISABLE
if (enablemouse) {
		artyk::utils::normal_log(m_logptr, "Enabling mouse support", LOG_WARN, m_modulename);
		artyk::utils::normal_log(m_logptr, "ENABLINMG MOUSE SUPPORT REMOVES SELECT ABILITY", LOG_SWARN, m_modulename);

		if (!SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT) ) {
			artyk::utils::Warn("Could not SetConsoleMode!\nFunction error code: " + to_string(GetLastError()), m_modulename, true, GET_FULL_DBG_INFO);
		}
	}
	moduleamt++;
	if (moduleamt == 1) {
		startrd();
	}
	
}
AEKeyboard::~AEKeyboard() {

	moduleamt--;
	if(!moduleamt){
		closetrd();
	}
	
	artyk::utils::normal_log(m_logptr, "Closed AEKeyboard Module.", LOG_SUCCESS, m_modulename);
	if (
#ifdef AE_GLOBALMODULE
		m_logptr != &global_logger &&
#endif // !AE_GLOBALMODULE
		m_logptr) {
		delete m_logptr;
	}



}



//control functions
void AEKeyboard::startrd(void) {

	if (m_threadon) {
		return;
	}
	m_kbtrd = std::thread(&AEKeyboard::mainthread_keys, this);
	if (m_kbtrd.joinable()) {
		m_threadon = true;
		artyk::utils::debug_log(m_logptr, "Started keyscanning thread.", LOG_OK, m_modulename);
	}
	else {
		artyk::utils::debug_log(m_logptr, "Could NOT start keyscanning thread:reason unknown", LOG_ERROR, m_modulename);
		
	}
	
}
void AEKeyboard::closetrd(void) {
	if (m_kbtrd.joinable()) {
		m_bstoptrd = true;
		m_threadon = false;
		m_kbtrd.join();
		artyk::utils::debug_log(m_logptr, "Closed keyscanning thread.", LOG_OK, m_modulename);
	}
	else {
		artyk::utils::debug_log(m_logptr, "Could not close keyscanning thread:it wasn't started", LOG_ERROR, m_modulename);
	}
	
	
}

//get/set

int AEKeyboard::GetKeyID(const char* keyname) {
	artyk::utils::debug_log(m_logptr, "Getting key id for the key: " + string(keyname), LOG_INFO, m_modulename);

	for (short i = 0; i < (sizeof(m_keys) / sizeof(m_keys[0])); i++) {
		if (strcmp(keyname, m_keys[i].m_name) == 0) {
			return m_keys[i].m_keyid;
		}
	}

	return 0;
}

const char* AEKeyboard::GetKeyName(smalluint keyid) {
	artyk::utils::debug_log(m_logptr, "Getting key name for the key: " + to_string(keyid), LOG_INFO, m_modulename);

	return m_keys[keyid].m_name;
}

//very bad implementation in my opinion
//but it will work for now
std::vector<AEKBKey> AEKeyboard::GetUsedKeys(){
	artyk::utils::debug_log(m_logptr, "Getting current used keys", LOG_INFO, m_modulename);

	//int presskeys = 0;
	std::vector<AEKBKey> keystat;
	//pausetrd = 1;
	// 		while (pausetrd != 2) {
	//
	// 		}
	keystat.reserve(257);
	for (short i = 0; i < (sizeof(m_keys) / sizeof(m_keys[0])); i++) {
		if (m_keys[i].m_isUsed) {
			keystat.push_back(m_keys[i]);

		}
	}
	
	artyk::utils::debug_log(m_logptr, "Currently used keys: "+to_string(keystat.size()), LOG_INFO, m_modulename);
	return keystat;
}

bool AEKeyboard::IsKeyPressed(const string& keyname)
{//looks at the keynames[][] and checks if given key is pressed; if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" +keyname+" pressed:"+to_string(m_keys[GetKeyID(keyname.c_str())].m_isPressed), LOG_INFO, m_modulename);


	return m_keys[GetKeyID(keyname.c_str())].m_isPressed;
}

bool AEKeyboard::IsKeyPressed(smalluint keyid)
{//looks at the m_kstKeys[] directly and checks if given key is pressed; if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" + to_string(keyid) + " pressed:" + to_string(m_keys[keyid].m_isPressed), LOG_INFO, m_modulename);

	return m_keys[keyid].m_isPressed;
}

bool AEKeyboard::IsKeyHeld(const string& keyname)
{//looks at the keynames[][] and checks if given key is held; if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" + keyname + " held:" + to_string(m_keys[GetKeyID(keyname.c_str())].m_isHeld), LOG_INFO, m_modulename);

	return m_keys[GetKeyID(keyname.c_str())].m_isHeld;
}

bool AEKeyboard::IsKeyHeld(smalluint keyid)
{//looks at the m_kstKeys[] directly and checks if given key is held; if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" + to_string(keyid) + " held:" + to_string(m_keys[keyid].m_isHeld), LOG_INFO, m_modulename);

	return m_keys[keyid].m_isHeld;
}

bool AEKeyboard::IsKeyReleased(const string& keyname)
{//looks at the keynames[][] directly and checks if given key is released; if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" + keyname + " released:" + to_string(m_keys[GetKeyID(keyname.c_str())].m_isReleased), LOG_INFO, m_modulename);

	return m_keys[GetKeyID(keyname.c_str())].m_isReleased;
}

bool AEKeyboard::IsKeyReleased(smalluint keyid)
{//looks at the m_kstKeys[] directly and checks if given key is released; if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" + to_string(keyid) + " released:" + to_string(m_keys[keyid].m_isReleased), LOG_INFO, m_modulename);

	return m_keys[keyid].m_isReleased;
}

bool AEKeyboard::IsKeyUsed(const string& keyname)
{//looks at the keynames[][] directly and checks if given key is used(pressed/held); if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" + keyname + " used:" + to_string(m_keys[GetKeyID(keyname.c_str())].m_isUsed), LOG_INFO, m_modulename);

	return m_keys[GetKeyID(keyname.c_str())].m_isUsed;
}

bool AEKeyboard::IsKeyUsed(smalluint keyid)
{//looks at the m_kstKeys[] directly and checks if given key is used(pressed/held); if not found returns 0;
	artyk::utils::debug_log(m_logptr, "Is key:" + to_string(keyid) + " used:" + to_string(m_keys[keyid].m_isUsed), LOG_INFO, m_modulename);

	return m_keys[keyid].m_isUsed;
}

void AEKeyboard::mainthread_keys(void) {
	//keyboard
	AEFrame m_fr(GAME_FPS*2);
	DWORD m_event_amt = 0;
	INPUT_RECORD m_event_buffer[32];//32 is just enough
	artyk::utils::debug_log(m_logptr, "Started keyscanning sequence.", LOG_OK, m_modulename);
	

	artyk::utils::waitfortick();
	while (artyk::app_startstatus < 1) {
		m_fr.sleep();
	}
	m_fr.setfps(GAME_FPS);
	while (!m_bstoptrd) 
	{//for stopping the thread
#ifdef AE_KB_DELAY
		m_fr.sleep();//sleep frame if we are not in focus
#endif // AE_KB_DELAY
		//std::cout << "kbtrd\n";
		
		while (artyk::g_console_hwnd == GetForegroundWindow())
		{
			//std::cout << "scanning\n";
			//get keyboard state
			for (int i = 0; i < 256; i++)
			{

				m_keys[i].m_state = GetAsyncKeyState(i);
				

				//m_kstKeys[i].m_name = GetKeyName(i);
				//if (m_keys[i].m_state != m_keyOld[i])
				{
					if (m_keys[i].m_state != 0) {
						if (m_keyOld[i] == 0) {
							m_keys[i].m_isPressed = true;
							m_keys[i].m_isHeld = false;
							m_keys[i].m_isReleased = false;
							m_keys[i].m_isUsed = true;
							
						}
						else
						{
							m_keys[i].m_isPressed = false;
							m_keys[i].m_isHeld = true;
							m_keys[i].m_isReleased = false;
							m_keys[i].m_isUsed = true;
						}
					}
					else {
						if (m_keyOld[i] != 0) {
							m_keys[i].m_isPressed = false;
							m_keys[i].m_isHeld = false;
							m_keys[i].m_isReleased = true;
							m_keys[i].m_isUsed = true;
						}
						else {
							m_keys[i].m_isPressed = false;
							m_keys[i].m_isHeld = false;
							m_keys[i].m_isReleased = false;
							m_keys[i].m_isUsed = false;
						}
					}
				}
				//if (i == VK_SPACE) {
				//	std::cout << m_keys[i].m_state << " " << m_keyOld[i] << NLC;
				//}
				m_keyOld[i] = m_keys[i].m_state;
			}
			if (m_enablemouse) {//only if we want support for mouse
				//get mouse events
				
				GetNumberOfConsoleInputEvents(artyk::g_input_handle, &m_event_amt);
				
				if (m_event_amt > 0) {
					ReadConsoleInput(
						artyk::g_input_handle, m_event_buffer, 
						(m_event_amt>
							(sizeof(m_event_buffer)/ sizeof(m_event_buffer[0]))//doing this only for safety, if the input amt surpasses the size of the buffer
						) 
						?	(sizeof(m_event_buffer) / sizeof(m_event_buffer[0])) 
						: m_event_amt, &m_event_amt);
					//we pass the even amount as for "lpNumberOfEventsRead" for 2 things: 1 saves space, we don't allocate temp stuff; 2 we can see how many we read.
				}
				
				//check for handle events
				for (DWORD i = 0; i < m_event_amt; i++)
				{
					if (m_event_buffer[i].EventType == MOUSE_EVENT && m_event_buffer[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
						
						m_mousepos[0] = m_event_buffer[i].Event.MouseEvent.dwMousePosition.X;
						m_mousepos[1] = m_event_buffer[i].Event.MouseEvent.dwMousePosition.Y;
						
					}
				}

			}


			if (m_bstoptrd) {
				artyk::utils::debug_log(m_logptr, "Stopped keyscanning sequence.", LOG_OK, m_modulename);
				return;
			}
			else
			{
#ifdef AE_KB_DELAY
				m_fr.sleep();
#endif // AE_KB_DELAY
			}
		}
		
	
	}

	

}

//big stuff
const std::array<std::pair<const char*, const smalluint>, 173> AEKeyboard::m_keycodes = { {
{"lbutton", 1},
{"rbutton", 2},
{"cancel", 3},
{"mbutton", 4},
{"xbutton1", 5},
{"xbutton2", 6},
{"back", 8},
{"tab", 9},
{"clear", 12},
{"return", 13},
{"shift", 16},
{"control", 17},
{"menu", 18},
{"pause", 19},
{"capital", 20},
{"kana", 21},
{"hanguel", 21},
{"hangul", 21},
{"junja", 23},
{"final", 24},
{"hanja", 25},
{"kanji", 25},
{"escape", 27},
{"convert", 28},
{"nonconvert", 29},
{"accept", 30},
{"modechange", 31},
{"space", 32},
{"prior", 33},
{"next", 34},
{"end", 35},
{"home", 36},
{"left", 37},
{"up", 38},
{"right", 39},
{"down", 40},
{"select", 41},
{"print", 42},
{"execute", 43},
{"snapshot", 44},
{"insert", 45},
{"delete", 46},
{"help", 47},
{"0", 48},
{"1", 49},
{"2", 50},
{"3", 51},
{"4", 52},
{"5", 53},
{"6", 54},
{"7", 55},
{"8", 56},
{"9", 57},
{"a", 65},
{"b", 66},
{"c", 67},
{"d", 68},
{"e", 69},
{"f", 70},
{"g", 71},
{"h", 72},
{"i", 73},
{"j", 74},
{"k", 75},
{"l", 76},
{"m", 77},
{"n", 78},
{"o", 79},
{"p", 80},
{"q", 81},
{"r", 82},
{"s", 83},
{"t", 84},
{"u", 85},
{"v", 86},
{"w", 87},
{"x", 88},
{"y", 89},
{"z", 90},
{"lwin", 91},
{"rwin", 92},
{"apps", 93},
{"sleep", 95},
{"numpad0", 96},
{"numpad1", 97},
{"numpad2", 98},
{"numpad3", 99},
{"numpad4", 100},
{"numpad5", 101},
{"numpad6", 102},
{"numpad7", 103},
{"numpad8", 104},
{"numpad9", 105},
{"multiply", 106},
{"add", 107},
{"separator", 108},
{"subtract", 109},
{"decimal", 110},
{"divide", 111},
{"f1", 112},
{"f2", 113},
{"f3", 114},
{"f4", 115},
{"f5", 116},
{"f6", 117},
{"f7", 118},
{"f8", 119},
{"f9", 120},
{"f10", 121},
{"f11", 122},
{"f12", 123},
{"f13", 124},
{"f14", 125},
{"f15", 126},
{"f16", 127},
{"f17", 128},
{"f18", 129},
{"f19", 130},
{"f20", 131},
{"f21", 132},
{"f22", 133},
{"f23", 134},
{"f24", 135},
{"numlock", 144},
{"scroll", 145},
{"lshift", 160},
{"rshift", 161},
{"lcontrol", 162},
{"rcontrol", 163},
{"lmenu", 164},
{"rmenu", 165},
{"browserback", 166},
{"browserforward", 167},
{"browserrefresh", 168},
{"browserstop", 169},
{"browsersearch", 170},
{"browserfavorites", 171},
{"browserhome", 172},
{"volumemute", 173},
{"volumedown", 174},
{"volumeup", 175},
{"medianexttrack", 176},
{"mediaprevtrack", 177},
{"mediastop", 178},
{"mediaplaypause", 179},
{"launchmail", 180},
{"launchmediaselect", 181},
{"launchapp1", 182},
{"launchapp2", 183},
{"oem1", 186},
{"oemplus", 187},
{"oemcomma", 188},
{"oemminus", 189},
{"oemperiod", 190},
{"oem2", 191},
{"oem3", 192},
{"oem4", 219},
{"oem5", 220},
{"oem6", 221},
{"oem7", 222},
{"oem8", 223},
{"oem102", 226},
{"processkey", 229},
{"packet", 231},
{"attn", 246},
{"crsel", 247},
{"exsel", 248},
{"ereof", 249},
{"play", 250},
{"zoom", 251},
{"noname", 252},
{"pa1", 253},
{"oemclear", 254},
} };
