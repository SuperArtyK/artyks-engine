////////////Sound.cpp
//#include "include.hpp"


//var name cheatsheet
/*
<scope/type>_<const/array, if it is>_<datatype><Name>

EX:
mpb_bDev					 | gi_flgDeflogger	  | mpr_vec_strMessg	   | ml_strPathtolog
m:member; pb:public; b:bool; | g:global; i:inline | pr:private; vec:vector | l:local

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
	filelog* mpb_ptr_flgLogptr = NULL;
	bool mpb_bDev = 0;
	Sound();
	Sound(filelog& logarg);
	~Sound();
	void MakeSound(int freq, int time);
	bool mpb_bQuit = false;
	bool mpb_bMkSound = false;

};


UINT SoundThread(LPVOID pParam)
{
	Sound* mSound = (Sound*)pParam;
	while (!mSound->mpb_bQuit)
	{
		if (mSound->mpb_bMkSound)
		{
			Beep(g_iFrequency, g_iDur);
			mSound->mpb_bMkSound = false;
		}
	}
	mSound->mpb_ptr_flgLogptr->writetolog("Exiting thread", 0, "Sound System");
	ExitThread(0);
}
Sound::Sound()
{
	if (mpb_ptr_flgLogptr == NULL) {
		mpb_ptr_flgLogptr = &gi_flgDeflogger;
	}
	mpb_ptr_flgLogptr->writetolog("Creating different thread", 0, "Sound System");
	AfxBeginThread(SoundThread, this);
	
}

Sound::Sound(filelog& logarg)
{
	mpb_ptr_flgLogptr = &logarg;
	mpb_ptr_flgLogptr->writetolog("Creating different thread", 0, "Sound System");
	AfxBeginThread(SoundThread, this);

}

void Sound::MakeSound(int freq, int time)
{
	mpb_ptr_flgLogptr->writetolog("Making sound " + std::to_string(freq) + " " + std::to_string(time), 0, "Sound System");
	mpb_bMkSound = true;
	g_iFrequency = freq;
	g_iDur = time;
}

Sound::~Sound()
{
	mpb_bQuit = true;
}




#endif // SOUND



