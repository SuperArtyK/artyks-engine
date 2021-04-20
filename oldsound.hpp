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
// this file contains the old sound system code.
// useful if you just need the simple beeping
// should not cause everything to break
//////////////////////////////////////////////////////////////////////////

////////////////////Sound.h
#pragma once
//#include "include.hpp"

#ifndef SOUND
#define SOUND

#define _AFXDLL
#include <afx.h>
#include <afxwin.h>
#include "log.hpp"
#include <string>
int g_iFrequency, g_iDur;
class Sound
{
public:
	filelog* m_flgLogptr = NULL;
	bool m_bDev = 0;
	Sound();
	Sound(filelog& logarg);
	~Sound();
	void MakeSound(int freq, int time);
	bool m_bQuit = false;
	bool m_bMkSound = false;

};


UINT SoundThread(LPVOID l_lpvdPParam)
{
	Sound* l_sndSoundptr = (Sound*)l_lpvdPParam;
	while (!l_sndSoundptr->m_bQuit)
	{
		if (l_sndSoundptr->m_bMkSound)
		{
			Beep(g_iFrequency, g_iDur);
			l_sndSoundptr->m_bMkSound = false;
		}
	}
	l_sndSoundptr->m_flgLogptr->writetolog("Exiting thread", 0, "Sound System");
	ExitThread(0);
}
Sound::Sound()
{
	if (m_flgLogptr == NULL) {
		m_flgLogptr = &g_flgDeflogger;//use default global logger, so we get at least something
		//comment out all writetolog lines to remove logging
	}
	m_flgLogptr->writetolog("Creating different thread", 0, "Sound System");
	AfxBeginThread(SoundThread, this);
	
}

Sound::Sound(filelog& l_flgLogarg)
{
	m_flgLogptr = &l_flgLogarg;
	m_flgLogptr->writetolog("Creating different thread", 0, "Sound System");
	AfxBeginThread(SoundThread, this);

}

void Sound::MakeSound(int l_iFreq, int l_iTime)
{
	m_flgLogptr->writetolog("Making sound " + std::to_string(l_iFreq) + " " + std::to_string(l_iTime), 0, "Sound System");
	m_bMkSound = true;
	g_iFrequency = l_iFreq;
	g_iDur = l_iTime;
}

Sound::~Sound()
{
	m_bQuit = true;
}




#endif // SOUND



