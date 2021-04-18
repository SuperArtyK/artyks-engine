////////////Sound.cpp
//#include "include.hpp"


//var name cheatsheet
/*
<scope>_<datatype><Name>

EX:
m_bDev          |g_flgDeflogger|m_strMessg|l_strPathtolog
m:member;b:bool;|g:global;     |vec:vector|l:local

b bool
i int
c char
ll long long (int)
ull unsigned long long (int)/uint64_t
ld long double
trd thread
fst fstream
cst const
flg filelog
arr array
vec vector
ptr pointer
lpvd lpvoid


etc..
*/



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



