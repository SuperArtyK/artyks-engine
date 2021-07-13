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

/** @file include/engine/AETimer.hpp
 *  This file contains the code for engine's timer.  
 *  
 *  Should not cause everything to break.  
 */

#pragma once
#ifndef TIMER_HPP
#define TIMER_HPP

#include "AEFrame.hpp"
#include "func_system.hpp"

using std::string;
using std::to_string;

/// \brief this is a engine timer, it starts to count up ticks as soon as module is created.  
/// It starts separate thread that increases the internal timer time.  
/// Useful for scheduling the delays or calculating time.  
class AETimer {
public:
	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="delay">delay of the timer between ticks</param>
	explicit AETimer(float delay = GAME_FPS) :  m_fr(delay), m_modulename("AETimer"), m_time(0), m_delay(delay), stopthread(false) {
		//m_time = 0;
		t1 = std::thread(&AETimer::incrtimer, this);
		if (!t1.joinable()) {
			//MessageBoxA(GetConsoleWindow(), "Could not start AETimer thread!", "FATAL ERROR: Could not start Timer!(AETimer)", MB_OK | MB_ICONERROR);
			//exit(5);
			artyk::utils::FError("Could not start AETimer thread!", m_modulename, GET_FULL_DBG_INFO, artyk::exitcodes::ERROR_STARTING_TIMER);
		}
	}


	~AETimer() { 
		stopthread = true;
		if (t1.joinable()) {
			t1.join();
		}
		if (m_time < TIMER_WAIT) {
			m_time = TIMER_WAIT;
		}

	}
	///returns the time of timer in ticks
	inline bigint gettime(void) const { return m_time;}
	///returns the approximate time of timer in whole seconds
	inline bigint getworldtime(void) const { return m_time/ m_delay;}
	///returns AEFrame of timer
	inline AEFrame getframerater(void) const { return m_fr; }
	///Returns the module name 
	///@see __AEBaseClass::getmodulename()
	inline string getmodulename(void) const{ return m_modulename; }
#ifdef AE_EXPERIMENTAL
	/// uses all utils for class
	///@see similar thing as __AEBaseClass::benchmark()
	void benchmark(void) const {
		gettime();
		getworldtime();
		getmodulename();
		getframerater().benchmark();

	}
#endif



private:
	///tick delayer
	AEFrame m_fr;
	///thread of the timer
	std::thread t1;
	///string containing the module name
	const string m_modulename;
	///variable to store time
	bigint m_time;
	///stores delay of timer
	float m_delay;
	///flag to stop thread
	bool stopthread;
	///function of the timer to increase time
	
	void incrtimer(void) {
#ifdef _WIN32
#ifdef _MSC_VER 
		SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
#endif // _MSC_VER 
#endif // _WIN32

		while(!stopthread){
			m_fr.sleep();
			m_time++;

		}

	}
};
/// our global timer
inline AETimer global_timer;


namespace artyk::utils {

	/// <summary>
	/// waits for the tick amount of the timer
	/// </summary>
	/// <param name="mytimer">timer you want to wait for</param>
	/// <param name="waitticks">how much of timer ticks to wait</param>
	inline void waittick(const AETimer& mytimer = global_timer, bigint waitticks = TIMER_WAIT) {
		AEFrame fr(mytimer.getframerater().getframerate() * 2);
		waitticks += mytimer.gettime();

		while (mytimer.gettime() < waitticks) {
			fr.sleep();
		}
	}
	/// <summary>
	/// waits for the specific tick of the timer
	/// </summary>
	/// <param name="mytimer">timer you want to wait for</param>
	/// <param name="waitticks">specific tick of the timer to wait for</param>
	inline void waitfortick(const AETimer& mytimer = global_timer, bigint waitticks = TIMER_WAIT) {
		AEFrame fr(mytimer.getframerater().getframerate() * 2);

		while (mytimer.gettime() < waitticks) {
			fr.sleep();
		}
	}
	/// <summary>
	/// waits for the tick amount of the timer, converted to milliseconds
	/// </summary>
	/// <param name="mytimer">timer you want to wait for</param>
	/// <param name="waitms">how much of milliseconds you want to wait</param>
	/// @warning If the time you ask to wait is less than the delay of the timer, then it will be rounded to nearest tick
	inline void waittime(const AETimer& mytimer = global_timer, bigint waitms = 3000) {
		waittick(mytimer, std::round(waitms / mytimer.getframerater().getdelay()));
	}

	/// <summary>
	/// waits for the specific tick of the timer, converted to milliseconds, probably useless, but here it is
	/// </summary>
	/// <param name="mytimer">timer you want to wait for</param>
	/// <param name="waitms">specific tick of the timer to wait for, converted to milliseconds</param>
	/// @warning If the time you ask to wait is less than the delay of the timer, then it will be rounded to nearest tick
	inline void waitfortime(const AETimer& mytimer = global_timer, bigint waitms = 3000) {
		waitfortick(mytimer, std::round(waitms / mytimer.getframerater().getdelay()));
	}
}







#endif // !TIMER_HPP
