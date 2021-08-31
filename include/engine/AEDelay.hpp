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

/** @file include/engine/AEDelay.hpp
 *  This file contains code for the AEDelay module code
 *	This module does the normal Sleep() function, but better
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef AEDELAY_HPP
#define AEDELAY_HPP

#include <thread>
#include <chrono>
#include "global_vars.hpp"
#include "typedefs.hpp"

/// \brief This module is used to create custom but accurate delays, unlike AEFrame that has only fixed delay(unless called setfps() to change it).
/// This module is not inherited from the base class, as it needs to be very fast, faster than the base class even,
/// and without much of dependencies.
class AEDelay
{
public:
	/// <summary>
	/// Class constructor
	/// </summary>
	explicit AEDelay() : tp(getsystime)
	{}

	///resets the time point time to current system time
	void resettime() {
		tp = getsystime;
	}

	///sleeps for given number of milliseconds(default is 1.0/GAME_FPS)
	void sleep(const double timems = 1.0 / GAME_FPS) {
		// add to time point
		if (timems < 0.000001f) {
			return;
		}
		tp += tduration<double>(microsec(biguint(timems * 1000)));
		// and sleep until that time point
		sleepuntil(tp);
	}

private:
	///the timepoint, that sets time when to wake up the thread
	timepoint<std::chrono::system_clock, tduration<double>> tp;
	///module name
	inline static const std::string m_modulename = "AEDelay";

};

#endif // !AEDELAY_HPP
