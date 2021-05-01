/*
	ArtyK's Console (Game) Engine. Console engine for apps and games
	Copyright (C) 2021  Artemii Kozhemiak

	https://github.com/SuperArtyK/myengine

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
// this file contains the global timer and delay that can be used everywhere
// should not cause everything to redefine
//////////////////////////////////////////////////////////////////////////

#pragma once
#ifndef TIMER
#define TIMER

#include "global_vars.hpp"
using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;
//thanks stackoverflow!

//usage:
// create obj frame_rater with fps value
// frame_rater<30> fr; -- creates frame_rater fr with 30fps delays
//
// after your stuff is done, call frame_rater::sleep();
//	while(true) {
//		std::cout << "Hello world\n";
//		fr.sleep();                   
//	}
// it outputs "Hello world" and waits the rest of time of 30fps delay(0.033....)
//
//one fucking problem: we will need to restart the program if we want to change the fps
//at least for now

//someone, who understands this code I stole, please re-do this to dynamicly assigning fps
//so I can change fps delays on the fly

#define TIMER_FPS GAME_FPS/6 // by default is 10

template<long long FPS>
class frame_rater {
public:
	frame_rater() :                 // initialize the object keeping the pace
		time_between_frames{ 1 },     // std::ratio<1, FPS> seconds
		tp{ std::chrono::steady_clock::now() }
	{}

	void sleep() {
		// add to time point
		tp += time_between_frames;

		// and sleep until that time point
		std::this_thread::sleep_until(tp);
	}

private:
	// a duration with a length of 1/FPS seconds
	std::chrono::duration<double, std::ratio<1, FPS>> time_between_frames;

	// the time point we'll add to in every loop
	std::chrono::time_point<std::chrono::steady_clock, decltype(time_between_frames)> tp;
};



//now this is a global timer, will be useful for engine related delays, instead of framerate ones
class timer {
public:
	timer() : m_time(0){
		//m_time = 0;
		m_delay = 10000;
		stopthread = false;
		t1 = std::thread(&timer::incrtimer, this);
	}

	timer(long long delay) : m_time(0) {
		//m_time = 0;
		m_delay = delay;
		stopthread = false;
		t1 = std::thread(&timer::incrtimer, this);
	}

	~timer() { stopthread = true; t1.join(); }

	long double gettime() {

		return m_time;
	}



private:
	std::string m_modulename = "Timer";
	frame_rater<TIMER_FPS> m_fr;
	long long m_delay;
	std::atomic<long double> m_time;
	std::thread t1;
	bool stopthread;
	void incrtimer() {
		while(!stopthread){
			m_time = m_time + 1;
			m_fr.sleep();
		}

	}
};
inline timer global_timer;

#endif // !TIMER
