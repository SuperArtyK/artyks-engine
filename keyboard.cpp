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
	kbtrd = std::thread(&Keyboard::checkkeystates, this);
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

const char* Keyboard::GetKeyName(uint8_t keyid) {
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
	//int presskeys = 0;
	vector<sKeyState> keystat;
	//pausetrd = 1;
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

bool Keyboard::IsKeyPressed(const string keyname)
{//looks at the keynames[][] and checks if given key is pressed; if not found returns 0;
	return m_kstKeys[GetKeyID(keyname)].m_bPressed;
}

bool Keyboard::IsKeyPressed(uint8_t keyid)
{//looks at the m_kstKeys[] directly and checks if given key is pressed; if not found returns 0;
	return m_kstKeys[keyid].m_bPressed;
}

bool Keyboard::IsKeyHeld(const string keyname)
{//looks at the keynames[][] and checks if given key is held; if not found returns 0;
	return m_kstKeys[GetKeyID(keyname)].m_bHeld;
}

bool Keyboard::IsKeyHeld(uint8_t keyid)
{//looks at the m_kstKeys[] directly and checks if given key is held; if not found returns 0;
	return m_kstKeys[keyid].m_bHeld;
}

bool Keyboard::IsKeyReleased(const string keyname)
{//looks at the keynames[][] directly and checks if given key is released; if not found returns 0;
	return m_kstKeys[GetKeyID(keyname)].m_bReleased;
}

bool Keyboard::IsKeyReleased(uint8_t keyid)
{//looks at the m_kstKeys[] directly and checks if given key is released; if not found returns 0;
	return m_kstKeys[keyid].m_bReleased;
}

bool Keyboard::IsKeyUsed(const string keyname)
{//looks at the keynames[][] directly and checks if given key is used(pressed/held); if not found returns 0;
	return (m_kstKeys[GetKeyID(keyname)].m_bHeld|| m_kstKeys[GetKeyID(keyname)].m_bPressed);
}

bool Keyboard::IsKeyUsed(uint8_t keyid)
{//looks at the m_kstKeys[] directly and checks if given key is used(pressed/held); if not found returns 0;
	return (m_kstKeys[keyid].m_bHeld || m_kstKeys[keyid].m_bPressed);
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
				m_kstKeys[i].m_keyid = i;
				//m_kstKeys[i].m_name = GetKeyName(i);
				if (m_keyNewState[i] != m_keyOldState[i])
				{
					if (m_keyNewState[i] & 0x8000)
					{
						m_kstKeys[i].m_bPressed = false;
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
			m_fr.sleep();//not collect the keystrokes, while game is on pause for next frame
		}


	}
	if (pausetrd == 1) {
		pausetrd = 2;
	}
}



