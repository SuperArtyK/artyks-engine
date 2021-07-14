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

/** @file src/engine/AEBeep.cpp
 *  This file contains the old sound system's function definitions.
 *  Useful if you just need the simple beeping.
 * 
 *  Should not cause everything to break.
 */

#include "AEBeep.hpp"

atomic<biguint> AEBeep::m_globalmodulenum = 0;


AEBeep::AEBeep(bool enablelog, bool useGlobLog) :  __AEBaseClass("AEBeep",++m_globalmodulenum), m_fr(GAME_FPS * 2), playset{ 0,0 }, m_bstop(true), m_makesound(false)
{

#ifdef AE_LOG_ENABLE
	if (enablelog) {
		if (useGlobLog) {
#ifdef AE_GLOBALMODULE
			m_logptr = &global_logger;
			artyk::utils::normal_log(m_logptr, "Using global logger", LOG_INFO, m_modulename);
#else
			m_logptr = new AELog(DEF_LOG_PATH, "SND");
			artyk::utils::debug_log(m_logptr, "You have set the useGlobLog flag on, but AE_GLOBALMODULE is not defined!", LOG_SWARN, m_modulename);
			artyk::utils::debug_log(m_logptr, "Falling back to default:Not using global modules", LOG_SWARN, m_modulename);
#endif
		}
		else
		{
			m_logptr = new AELog(DEF_LOG_PATH, "SND");
		}
	}

#endif // AE_LOG_DISABLE








	artyk::utils::normal_log(m_logptr, "Started AEBeep module!", LOG_SUCCESS, m_modulename);


}

AEBeep::~AEBeep() {
	closethread();
	artyk::utils::normal_log(m_logptr, "Closed Sound module", LOG_SUCCESS, m_modulename);
	if (
#ifdef AE_GLOBALMODULE
		m_logptr != &global_logger &&
#endif // !AE_GLOBALMODULE
		m_logptr) {
		delete m_logptr;
	}
}

void AEBeep::makesound_async(short freq, short dur) {

	if (freq < 25 || dur < 30) {
		return;
	}

	if (m_bstop) {
		openthread();
	}
	playset[0] = freq;
	playset[1] = dur;
	m_makesound = true;
	artyk::utils::debug_log(m_logptr, "(async)Ordered sound: " + to_string(playset[0]) + " " + to_string(playset[1]), LOG_OK, m_modulename);

}

void AEBeep::makesound_block(short freq, short dur) {
	if (playset[1] < 30) {
		return;
	}
	if (playset[0] > 25) {
		Sleep(playset[1]);
		artyk::utils::debug_log(m_logptr, "Waited ms:" + to_string(playset[1]), LOG_OK, m_modulename);
	}
	Beep(freq, dur);
	artyk::utils::debug_log(m_logptr, "(block)Played sound: " + to_string(freq) + " " + to_string(dur), LOG_OK, m_modulename);

}


void AEBeep::openthread() {
	m_bstop = false;
	artyk::utils::debug_log(m_logptr, "Creating sound playing thread", LOG_WARN, m_modulename);
	m_thread = std::thread(&AEBeep::mainthread, this);
	if (m_thread.joinable()) {
		artyk::utils::debug_log(m_logptr, "Created sound playing thread", LOG_OK, m_modulename);
		
	}
	else {
		artyk::utils::debug_log(m_logptr, "Could not create sound playing thread!", LOG_ERROR, m_modulename);
	}
}


void AEBeep::closethread() {
	m_bstop = true;
	artyk::utils::debug_log(m_logptr, "Stopping sound playing thread", LOG_WARN, m_modulename);
	if (m_thread.joinable()) {
		m_thread.join();
		artyk::utils::debug_log(m_logptr, "Stopped sound playing thread", LOG_OK, m_modulename);
	}
	else {
		artyk::utils::debug_log(m_logptr, "Could not stop sound playing thread: thread was not started!", LOG_ERROR, m_modulename);
	}
}


void AEBeep::mainthread() {
	artyk::utils::debug_log(m_logptr, "Started sound creating thread", LOG_OK, m_modulename);
	while (!m_bstop)
	{
		if (m_makesound) {
			m_makesound = false;
			if (playset[1]<30) {
				continue;
			}
			if (playset[0]< 25) {
				Sleep(playset[1]);
				artyk::utils::debug_log(m_logptr, "(async)Waited ms:" + to_string(playset[1]), LOG_OK, m_modulename);
				continue;
			}
			Beep(playset[0], playset[1]);
			
			artyk::utils::debug_log(m_logptr, "(async)Played sound: " + to_string(playset[0]) + " " + to_string(playset[1]), LOG_OK, m_modulename);
		}
#ifdef AE_BP_DELAY
		if (!m_bstop) {
			m_fr.sleep();
		}
#endif // AE_BP_DELAY

		
	}
	artyk::utils::debug_log(m_logptr, "Exiting sound creating thread", LOG_WARN, m_modulename);
}
