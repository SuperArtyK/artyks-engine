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

/** @file include/AEFrame.hpp
 *  This file contains the delay module code.  
 *  Useful if you need to limit the execution time of the program. It makes
 *  sure that the loop(f.ex.) executes with same delay, if contents of loop
 *  processed faster than you need.  
 *  
 *  Should not cause everything to break.  
 */

#pragma once

#ifndef AEFRAME_HPP
#define AEFRAME_HPP

#include <fstream>
#include <atomic>
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include "global_vars.hpp"

//usage:
// create obj AEFrame with fps value
// AEFrame fr(30); -- creates frame_rater fr with 30fps delays
//
// after your stuff is done, call frame_rater::sleep();
//	while(true) {
//		std::cout << "Hello world\n";
//		fr.sleep();                   
//	}
// it outputs "Hello world" and waits the rest of time of 30fps delay(cout delay+rest = 0.0333...)
// 
// also you can change framerate on the fly with AEFrame::setfps(float myfps)

/// \brief This module is used for creating delays(in FPS format) in the engine.  
/// It is great if used in loops, as it makes them have delay of AEFrame, if body executes fasted than the delay time.  
/// This module is not inherited from the base class, as it needs to be very fast, faster than the base class even,
/// and without much of dependencies.  
class AEFrame {
public:
	
	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="fps">the delay in the format of framerate, defaults to the GAME_FPS</param>
	/// @note if you pass it 0 or negative number it will disable the delay
	explicit AEFrame(float fps = GAME_FPS) : time_between_frames((fps <= 0) ? std::chrono::microseconds(0) : std::chrono::microseconds(biguint(1000000 / fps))),
tp(std::chrono::system_clock::now()), m_modulename("AEFrame"), nodelay((fps <= 0))
	{
		
	}

	///resets the delay value to what you pass to it
	/// @note if you pass it 0 or negative number it will disable the delay
	/// @see AEFrame()
	void setfps(float fps = GAME_FPS) {
		if (fps <= 0) { nodelay = true; return; }
		time_between_frames = std::chrono::microseconds(biguint(1000000 / fps));
		tp = std::chrono::system_clock::now();
	}

	///makes the current thread sleep for a set delay
	void sleep(void) {
		// add to time point
		if (nodelay) {
			return;
		}
		tp += time_between_frames;
		// and sleep until that time point
		std::this_thread::sleep_until(tp);
		
	}
	///Returns the module name 
	///@see __AEBaseClass::getmodulename()
	inline string getmodulename(void) const { return m_modulename; }
	///returns framerate of AEFrame
	inline int getframerate(void) const { return 1 / time_between_frames.count(); }
	///returns the delay of AEFrame in seconds
	inline float getdelay(void) const { return time_between_frames.count(); }

private:
	
	///delay between seconds
	std::chrono::duration<double> time_between_frames;
	///the timepoint, that sets time when to wake up the thread
	std::chrono::time_point<std::chrono::system_clock, decltype(time_between_frames)> tp;
	///module name
	const string m_modulename;
	///flag if we don't need the delay
	bool nodelay;
};


#endif // !AEFRAME_HPP
