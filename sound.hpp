////////////Sound.cpp
//#include "include.hpp"



////////////////////Sound.h
#pragma once
//#include "include.hpp"

#ifndef SOUND
#define SOUND

#define _AFXDLL
#include <afx.h>
#include <afxwin.h>
#include "log.hpp"
int frequency, dur;
class Sound
{
public:
	filelog* logptr = NULL;
	bool dev = 0;
	Sound();
	Sound(filelog& logarg);
	~Sound();
	void MakeSound(int freq, int time);
	BOOL Quit = false;
	BOOL mkSound = false;

};


UINT SoundThread(LPVOID pParam)
{
	Sound* mSound = (Sound*)pParam;
	while (!mSound->Quit)
	{
		if (mSound->mkSound)
		{
			Beep(frequency, dur);
			mSound->mkSound = false;
		}
	}
	mSound->logptr->writetolog("Exiting thread", 0, "Sound System");
	ExitThread(0);
}
Sound::Sound()
{
	if (logptr == NULL) {
		logptr = &deflogger;
	}
	logptr->writetolog("Creating different thread", 0, "Sound System");
	AfxBeginThread(SoundThread, this);
	
}

Sound::Sound(filelog& logarg)
{
	logptr = &logarg;
	logptr->writetolog("Creating different thread", 0, "Sound System");
	AfxBeginThread(SoundThread, this);

}

void Sound::MakeSound(int freq, int time)
{
	logptr->writetolog("Making sound "+to_string(freq)+" "+to_string(time), 0, "Sound System");
	mkSound = true;
	frequency = freq;
	dur = time;
}

Sound::~Sound()
{
	Quit = true;
}




#endif // SOUND



