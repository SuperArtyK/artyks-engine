#pragma once

#ifndef KEYBOARD
#define KEYBOARD

#include "include.hpp"
#include "global_functions.hpp"
using namespace std;

//kst -- keystate
struct sKeyState
{
	bool m_bPressed;
	bool m_bReleased;
	bool m_bHeld;
} g_kstKeys[256], g_kstMouse[5];


//variables
//bool
bool m_bConsoleInFocus = true;//first frame we will be always in focus
//idk how you can use mouse in console, but should be handy sometime
bool m_mouseOldState[5] = { 0 };
bool m_mouseNewState[5] = { 0 };
bool stoptrd = false;//debug
//int
int g_iMousePosX;
int g_iMousePosY;
//short
short m_keyOldState[256] = { 0 };
short m_keyNewState[256] = { 0 };




//'Get' functions
sKeyState GetKey(int nKeyID) { return g_kstKeys[nKeyID]; }
int GetMouseX() { return g_iMousePosX; }
int GetMouseY() { return g_iMousePosY; }
sKeyState GetMouse(int nMouseButtonID) { if (inrange(1, 6, nMouseButtonID)) { return g_kstMouse[nMouseButtonID]; }  return g_kstMouse[1]; }


int GetKeyID(string keyid) {

	strtolower(keyid);
	///





	return 0;
}


bool IsFocused() { return m_bConsoleInFocus; }//are we in focus right now?
//will be updated by graphics engine

void checkkeystates() {
	static bool clearedmem = false;
	if (!clearedmem) {
		std::memset(m_keyOldState, 0, 256 * sizeof(short));
		std::memset(m_keyNewState, 0, 256 * sizeof(short));
		std::memset(g_kstKeys, 0, 256 * sizeof(sKeyState));
		g_iMousePosX = 0;
		g_iMousePosY = 0;
	}
	//keyboard

	while(!stoptrd) {
		for (int i = 0; i < 256; i++)
		{
			m_keyNewState[i] = GetAsyncKeyState(i);

			g_kstKeys[i].m_bPressed = false;
			g_kstKeys[i].m_bReleased = false;

			if (m_keyNewState[i] != m_keyOldState[i])
			{
				if (m_keyNewState[i] & 0x8000)
				{
					g_kstKeys[i].m_bPressed = !g_kstKeys[i].m_bHeld;
					g_kstKeys[i].m_bHeld = true;
				}
				else
				{
					g_kstKeys[i].m_bReleased = true;
					g_kstKeys[i].m_bHeld = false;
				}
			}

			m_keyOldState[i] = m_keyNewState[i];
		}

	}

	
}



#endif // !KEYBOARD
