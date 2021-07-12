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

/** @file include/AEBeep.hpp
 *  This file contains the old sound system code.  
 *  Useful if you just need the simple beeping.  
 *  
 *  Should not cause everything to break.  
 */

#pragma once

#ifndef SOUND_HPP
#define SOUND_HPP
#include "AELog.hpp"
#include "AEBaseClass.hpp"

/// \brief This module is for simple sine wave beeping.  
/// It creates separated thread for the asynchronous beeping.  
/// It uses winapi's Beep() so expect all bugs with this module, as with Beep().  
/// It is based on __AEBaseClass.  
class AEBeep: public __AEBaseClass{
public:
	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="enablelog">flag to check if we need to log stuff at all</param>
	/// <param name="useGlobLog">flag to check if to use gloabal_logger or not(if it exists)</param>
	AEBeep(bool enablelog = true, bool useGlobLog = false);
	~AEBeep() override;

	///asks the separate thread to play sound with given values, works asynchronously
	void makesound_async(short freq, short dur);

	///plays sound directly, but blocks the thread
	void makesound_block(short freq, short dur);

#ifdef AE_EXPERIMENTAL
	/// uses all utils for class
	///@see __AEBaseClass::benchmark()
	void benchmark(void) override {

		makesound_async(0, 0);
		makesound_block(0, 0);
		getmodulename();
		getmodulenum();
	}

#endif // AE_EXPERIMENTAL

private:
	///opens the thread for async playing
	void openthread();
	///closes the thread
	void closethread();
	///the function of the thread itself
	void mainthread();
	///frame limiter of the thread
	AEFrame m_fr;
	///thread itself
	std::thread m_thread;
	///variable to store the module index number
	static atomic<biguint> m_globalmodulenum;
	///settings of the sound to play
	atomic<short> playset[2];
	///variable to stop the thread if we want to close
	atomic<bool> m_bstop;
	///variable to indicate if we want to play sound
	bool m_makesound;
	
};

#ifdef AE_GLOBALMODULE
///global beeper module
inline AEBeep global_beeper(true);
#endif



#endif // SOUND_HPP



