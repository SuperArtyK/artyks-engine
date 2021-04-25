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
// this file contains the keyboard scanning functions' definitions
// should not cause everything to break
//////////////////////////////////////////////////////////////////////////

#include "keyboard.hpp"

//constructors and destructors
Keyboard::Keyboard() : stoptrd(false), trdstarted(false) {
	std::memset(m_keyOldState, 0, 256 * sizeof(short));
	std::memset(m_keyNewState, 0, 256 * sizeof(short));
	std::memset(m_kstKeys, 0, 256 * sizeof(sKeyState));
	std::memset(m_mouseOldState, 0, 5 * sizeof(bool));
	std::memset(m_mouseNewState, 0, 5 * sizeof(bool));
	g_iMousePosX = 0;
	g_iMousePosY = 0;
	processdelay = 0;
	startrd();
}
Keyboard::~Keyboard() { closetrd(); }



//control functions
void Keyboard::startrd() {
	kbtrd = thread(&Keyboard::checkkeystates, this);
	trdstarted = true;
}
void Keyboard::closetrd() {
	if (trdstarted) {
		stoptrd = true;
		kbtrd.join();
		trdstarted = false;
	}
}

//get/set

int Keyboard::GetKeyID(std::string keyname) {
	artyk::tolower_ref(keyname);
	for (int i = 0; i < (sizeof artyk::keynames / sizeof artyk::keynames[0]); i++) {
		if (keyname == artyk::keynames[i][0]) {
			return stoi(artyk::keynames[i][1]);
		}
	}

	return 0;
}

const char* Keyboard::GetKeyName(int keyid) {
	for (int i = 0; i < (sizeof artyk::keynames / sizeof artyk::keynames[0]); i++) {
		if (keyid == stoi(artyk::keynames[i][1])) {
			return artyk::keynames[i][0].c_str();
		}
	}

	return 0;
}

//very bad implementation in my opinion
//but it will work for now
vector<sKeyState> Keyboard::GetPressedKeys() {
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

bool Keyboard::IsKeyPressed(string keyname)
{






	return false;
}

bool Keyboard::IsKeyPressed(int keyid)
{
	return false;
}



void Keyboard::checkkeystates() {
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



