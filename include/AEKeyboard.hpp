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

/** @file include/AEKeyboard.hpp
 *  This file contains the keyboard scanning code and struct for the keys.  
 *  I would like to thank the David "JavidX9" Barr for giving the idea
 *  of how it should be done.  
 *  
 *  Should not cause everything to break.  
 */

#pragma once

#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP

#include "func_utils.hpp"
#include "global_vars.hpp"
#include "AELog.hpp"
#include "AETimer.hpp"
#include "AEBaseClass.hpp"
#include <Windows.h>

using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;

///data type to store keys and their data  
struct AEKBKey
{
	///name of the key
	const char* m_name;
	///the GetAsyncKeyState() key state
	short m_state;
	///flag if key is pressed
	bool m_isPressed;
	///flag if key is released
	bool m_isReleased;
	///flag if key is held
	bool m_isHeld;
	///flag if key is used at all
	bool m_isUsed;
	///stores the key id
	smalluint m_keyid;
	
};


/// \brief This module is for scanning keyboard and mouse for keystrokes and other key events.  
/// It creates separated thread for the scanning, but it is one thread on all modules.  
/// It uses winapi's GetAsyncKeyState() so expect all bugs with this module, as with GetAsyncKeyState().  
/// It is based on __AEBaseClass.  
class AEKeyboard: public __AEBaseClass {
public:
	///this function initialises all stuff the engine needs, before it starts.
	///marked as friend function, so it can init stuff from this class
	friend int initenginestuff();
	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="enablelog">flag to check if we need to log stuff at all</param>
	/// <param name="useGlobLog">flag to check if to use gloabal_logger or not(if it exists)</param>
	/// <param name="enablemouse">flag to check if to enable mouse support, enabling it is permament for current engine instance</param>
	AEKeyboard(bool enablelog = true, bool useGlobLog = false, bool enablemouse = false);
	~AEKeyboard() override;



	
	
	///returns AEKBKey struct of the key, with id you pass
	inline AEKBKey GetKey(smalluint keyid) const { return m_keys[keyid]; }
	///same as AEKBKey::GetKey() but limited to 6 mouse buttons
	inline AEKBKey GetMouse(smalluint mousebutton_id) const { return m_keys[mousebutton_id % 6]; }
	///returns vector of AEKBKey's of all used(pressed or held)
	vector<AEKBKey> GetUsedKeys(void);

	///returns mouse x position, if the mouse scanning is enabled. Otherwise it wll be 0
	inline int GetMouseX(void) const { return m_mousepos[0]; }
	///returns mouse y position, if the mouse scanning is enabled. Otherwise it wll be 0
	inline int GetMouseY(void) const { return m_mousepos[1]; }
	///returns AEKBKey of the mouse key, with id you pass
	
	///returns key id of the key with given key name
	int GetKeyID(const char* keyname);
	///returns key name of the key with given key id
	const char* GetKeyName(smalluint keyid);
	
	
	///returns pressed status of key of given key name
	bool IsKeyPressed(const string& keyname);//looks at key list, and checks if given key has m_isPressed flag on
	///returns pressed status of key of given key id
	bool IsKeyPressed(smalluint keyid);
	///returns held status of key of given key name
	bool IsKeyHeld(const string& keyname);//looks at key list, and checks if given key has m_isHeld flag on
	///returns held status of key of given key id
	bool IsKeyHeld(smalluint keyid);
	///returns released status of key of given key name
	bool IsKeyReleased(const string& keyname);//looks at key list, and checks if given key has m_isReleased flag on
	///returns released status of key of given key id
	bool IsKeyReleased(smalluint keyid);
	///returns used status of key of given key name
	bool IsKeyUsed(const string& keyname);//looks at key list, and checks if given key has m_isPressed or m_isHeld flag on
	///returns used status of key of given key id
	bool IsKeyUsed(smalluint keyid);

#ifdef AE_EXPERIMENTAL
	/// uses all utils of module
	///@see __AEBaseClass::benchmark()
	void benchmark(void) override {
		GetKey(1);
		GetMouse(1);
		GetMouseX();
		GetMouseY();
		GetKeyID("space");
		GetKeyName(1);
		GetUsedKeys();
		IsKeyPressed(1);
		IsKeyPressed("space");
		IsKeyHeld(1);
		IsKeyHeld("space");
		IsKeyReleased(1);
		IsKeyReleased("space");
		IsKeyUsed(1);
		IsKeyUsed("space");
		getmodulename();
		getmodulenum();

	}
#endif

	


private:
	///the function of the thread itself
	void mainthread_keys(void);
	///opens the thread
	void startrd(void);
	///closes the thread
	void closetrd(void);
	///virtual key array, to be scanned with GetAsyncKeyState()
	static AEKBKey m_keys[256];
	///array of our key codes and names
	static const std::array<std::pair<const char*, const smalluint>, 173> m_keycodes;
	///previous state of the keys, data is from GetAsyncKeyState()
	static short m_keyOld[256];
	///thread of the module
	std::thread m_kbtrd;
	///variable to store the module index number
	static atomic<biguint> m_globalmodulenum;
	///mouse position
	static short m_mousepos[2];
	///is the thread on
	static bool m_threadon;
	///do we need to stop the thread
	bool m_bstoptrd;
	///is mouse enabled
	bool m_enablemouse;
	

};

#ifdef AE_GLOBALMODULE
///global keyboard module
inline AEKeyboard global_keyboard(true,true
#ifdef AE_GLOBALKB_ENABLE_MOUSE
	,true
#endif // AE_GLOBALKB_ENABLE_MOUSE

);
#endif



#endif // !KEYBOARD_HPP