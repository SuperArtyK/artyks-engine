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

#include "include.hpp"
#include "global_functions.hpp"
#include "global_vars.hpp"
using namespace std;

//kst -- keystate
struct sKeyState
{
	int id;
	const char* name;
	bool m_bPressed;
	bool m_bReleased;
	bool m_bHeld;
};

class Keyboard {
public:

	Keyboard();
	~Keyboard();

	void startrd();
	void closetrd();

	sKeyState GetKey(int nKeyID) { return m_kstKeys[nKeyID]; }
	int GetMouseX() { return g_iMousePosX; }
	int GetMouseY() { return g_iMousePosY; }
	sKeyState GetMouse(int nMouseButtonID) { if (artyk::inrange(1, 6, nMouseButtonID)) { return g_kstMouse[nMouseButtonID]; }  return g_kstMouse[1]; }
	int GetKeyID(std::string keyname);

	const char* GetKeyName(int keyid);
	bool IsFocused() { return m_bConsoleInFocus; }//are we in focus right now?
												  //will be updated by graphics engine

	vector<sKeyState> GetPressedKeys();
	std::string getmodulename() { return modulename; }
	bool IsKeyPressed(string keyname);//looks at the keynames[][] and checks if given key is pressed; if not found returns 0;
	bool IsKeyPressed(int keyid);//looks at the m_kstKeys[] directly and checks if given key is pressed; if not found returns 0;
	//bool IsKeyPressed(char keyname);//checks 
	void setdelay(int milis) {
		processdelay = milis;
	}


private:


	int processdelay;
	std::string modulename = "Keyboard";
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
	thread kbtrd;
	int pausetrd;
	bool stoptrd;//debug
	bool trdstarted;

	void checkkeystates();
};

#endif // !KEYBOARD