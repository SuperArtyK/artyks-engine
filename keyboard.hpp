/*
	ArtyK's Console (Game) Engine. Console engine for apps and games
	Copyright (C) 2021  Artemii Kozhemiak

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//////////////////////////////////////////////////////////////////////////
// this file contains the keyboard scanning code.
// should not cause everything to break
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef KEYBOARD
#define KEYBOARD

#include "global_functions.hpp"
#include "global_vars.hpp"
#include "timer.hpp"
#include <windows.h>
using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;


//kst -- keystate
struct sKeyState
{
	int m_keyid;
	//const char* m_name;
	bool m_bPressed;
	bool m_bReleased;
	bool m_bHeld;
	short m_state;//the state itself
};

class Keyboard {
public:

	Keyboard();
	~Keyboard();

	void startrd();
	void closetrd();

	sKeyState GetKey(uint8_t nKeyID) { return m_kstKeys[nKeyID]; }
	int GetMouseX() { return g_iMousePosX; }
	int GetMouseY() { return g_iMousePosY; }
	sKeyState GetMouse(uint8_t nMouseButtonID) { if (artyk::inrange(1, 6, nMouseButtonID)) { return g_kstMouse[nMouseButtonID]; }  return g_kstMouse[1]; }
	int GetKeyID(std::string keyname);

	const char* GetKeyName(uint8_t keyid);
	bool IsFocused() { return m_bConsoleInFocus; }//are we in focus right now?
												  //will be updated by graphics engine

	vector<sKeyState> GetPressedKeys();
	std::string getmodulename() { return m_modulename; }
	bool IsKeyPressed(string keyname);//looks at the keynames[][] and checks if given key is pressed; if not found returns 0;
	bool IsKeyPressed(uint8_t keyid);//looks at the m_kstKeys[] directly and checks if given key is pressed; if not found returns 0;
	bool IsKeyHeld(string keyname);//looks at the keynames[][] and checks if given key is held; if not found returns 0;
	bool IsKeyHeld(uint8_t keyid);//looks at the m_kstKeys[] directly and checks if given key is held; if not found returns 0;
	bool IsKeyReleased(string keyname);//looks at the keynames[][] directly and checks if given key is released; if not found returns 0;
	bool IsKeyReleased(uint8_t keyid);//looks at the m_kstKeys[] directly and checks if given key is released; if not found returns 0;
	bool IsKeyUsed(string keyname);//looks at the keynames[][] directly and checks if given key is used(pressed/held); if not found returns 0;
	bool IsKeyUsed(uint8_t keyid);//looks at the m_kstKeys[] directly and checks if given key is used(pressed/held); if not found returns 0;



	//bool IsKeyPressed(char keyname);//checks 
// 	void setdelay(int milis) {
// 		processdelay = milis;
// 		
// 	}


private:
	frame_rater<GAME_FPS> m_fr;

	int processdelay;
	std::string m_modulename = "Keyboard";
	sKeyState m_kstKeys[256], g_kstMouse[5];
	bool m_bConsoleInFocus = true;//first frame we will be always in focus
//idk how you can use mouse in console, but should be handy sometime
	bool m_mouseOldState[5] = { 0 };
	bool m_mouseNewState[5] = { 0 };

	//int
	int g_iMousePosX;
	int g_iMousePosY;
	//short
	short m_keyOldState[256] = { 0 };
	short m_keyNewState[256] = { 0 };

	//thread
	std::thread kbtrd;
	int pausetrd;
	bool stoptrd;//debug
	bool trdstarted;

	void checkkeystates();
};

#endif // !KEYBOARD