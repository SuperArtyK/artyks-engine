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

	Keyboard() : stoptrd(false), trdstarted(false) {
		std::memset(m_keyOldState, 0, 256 * sizeof(short));
		std::memset(m_keyNewState, 0, 256 * sizeof(short));
		std::memset(m_kstKeys, 0, 256 * sizeof(sKeyState));
		std::memset(m_mouseOldState, 0, 5 * sizeof(bool));
		std::memset(m_mouseNewState, 0, 5 * sizeof(bool));
		g_iMousePosX = 0;
		g_iMousePosY = 0;
		startrd();
	}
	~Keyboard() { closetrd(); }

	void startrd() {
		kbtrd = thread(&Keyboard::checkkeystates, this);
		trdstarted = true;
	}
	void closetrd() {
		if (trdstarted) {
			stoptrd = true;
			kbtrd.join();
			trdstarted = false;
		}
	}

	sKeyState GetKey(int nKeyID) { return m_kstKeys[nKeyID]; }
	int GetMouseX() { return g_iMousePosX; }
	int GetMouseY() { return g_iMousePosY; }
	sKeyState GetMouse(int nMouseButtonID) { if (inrange(1, 6, nMouseButtonID)) { return g_kstMouse[nMouseButtonID]; }  return g_kstMouse[1]; }
	int GetKeyID(std::string keyname) {
		strtolower(keyname);
		///
		for (int i = 0; i < (sizeof keynames / sizeof keynames[0]); i++) {
			if (keyname == keynames[i][0]) {
				return stoi(keynames[i][1]);
			}
		}

		return 0;
	}

	const char* GetKeyName(int keyid) {
		for (int i = 0; i < (sizeof keynames / sizeof keynames[0]); i++) {
			if (keyid == stoi(keynames[i][1])) {
				return keynames[i][0].c_str();
			}
		}

		return 0;
	}
	bool IsFocused() { return m_bConsoleInFocus; }//are we in focus right now?
												  //will be updated by graphics engine

	vector<sKeyState> GetPressedKeys() {
		int presskeys = 0;
		vector<sKeyState> keystat;
		pausetrd = 1;
		// 		while (pausetrd != 2) {
		//
		// 		}
		for (int i = 0; i < (sizeof(m_kstKeys) / sizeof(m_kstKeys[0])); i++) {
			if (m_kstKeys[i].m_bHeld || m_kstKeys[i].m_bPressed) {
				keystat.push_back(m_kstKeys[i]);
			}
		}
		pausetrd = 0;

		return keystat;
	}

private:

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

	void checkkeystates() {
		//keyboard

		while (!stoptrd) {//for stopping the thread
			//another while for pausing
			//while (pausetrd == 0)
			{
				for (int i = 0; i < 256; i++)
				{
					if (stoptrd) {
						pausetrd = true;//so we wont get inf loop when stoptrd is active
					}

					m_keyNewState[i] = GetAsyncKeyState(i);

					m_kstKeys[i].m_bPressed = false;
					m_kstKeys[i].m_bReleased = false;
					m_kstKeys[i].id = i;
					m_kstKeys[i].name = GetKeyName(i);
					if (m_keyNewState[i] != m_keyOldState[i])
					{
						if (m_keyNewState[i] & 0x8000)
						{
							m_kstKeys[i].m_bPressed = !m_kstKeys[i].m_bHeld;
							m_kstKeys[i].m_bHeld = true;
						}
						else
						{
							m_kstKeys[i].m_bReleased = true;
							m_kstKeys[i].m_bHeld = false;
						}
					}

					m_keyOldState[i] = m_keyNewState[i];
				}
			}
		}
		if (pausetrd == 1) {
			pausetrd = 2;
		}
	}
};

//'Get' functions

#endif // !KEYBOARD