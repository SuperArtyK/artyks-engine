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
// this file contains the logger class, for logging data to file.
// should not cause everything to break
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef FILELOG
#define FILELOG

//#include "include.hpp"//includes EVERYTHING that I need
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include <mutex>
#include "timer.hpp"
//#include "screen.hpp"

using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;

//logger types
#define LOG_INFO 0 
#define LOG_WARN 1
#define LOG_ERROR 2
#define LOG_FERROR 3
#define LOG_SWARN 4
#define LOG_OK 5
#define LOG_SUCCESS 6
//or
#define LOG_FATAL_ERROR 3
#define LOG_SEVERE_WARNING 4

#define LOG_FRAMERATE GAME_FPS/4 //4x slower than game framerate

inline bool log_started = false;//for marking the exe start in logs, inside filelog

class Filelog {
public:
	atomic<bool> m_bDev_cout;//for debug information; outputs the info to main console
	//WARN: MAY CAUSE MEMORY LEAK IF MORE THAN 1 THREADS ARE OUTPUTTING TO CONSOLE!!
	atomic<bool> m_bDev_log;//for debug information; outputs debug filelog info to current log

	Filelog();

	Filelog(std::string l_strPathtolog);
//	this just breaks logging for no reason
// 	filelog(int devenable);
// 	filelog(std::string l_strPathtolog, int devenable);


	//int changelogpath(std::string newlogpath);
	//filelog& operator=(std::string differentpath);

	~Filelog();
	

	int writetolog(std::string l_strMessg = "", int l_iType = LOG_INFO, std::string l_strProg_module = "Logger", bool debuglog = false);
	int addtologqueue(std::string l_strMessg = "", int l_iType = 0, std::string l_strProg_module = "Logger", bool debuglog = false);//this is for safe logging, during important part of log itself(to avoid inf recursion)
	int stoplogging(bool closeprog = false);//flag is if we are closing the program.
	int startlogging();
	int mainthread();
	
	
	std::string getmodulename() { return m_modulename; }
private:
	
	//variables

	//bool's -- 4
	bool m_bStoplog;//flag that for stopping/opening logging session
//	atomic<bool> m_bDev_cout; for reference, it is in public section
//	atomic<bool> m_bDev_log; for reference, it is in public section
	bool m_bThreadstarted;

	//int's -- 2
	uint64_t m_ullMessgcount;
	uint64_t m_ullEntrycount;

	//string's -- 6
	std::string m_strDatentime;//stores date/time data
	std::string m_strFilename;
	std::string m_strLogpath;//log path variable
	std::string m_strTmpFilename;
	std::string m_strTmpLogpath;//temp log path variable
	const std::string m_modulename = "Logger";

	//fstream's -- 2
	std::fstream m_fstFilestr;//file "editor"
	std::fstream m_fstTmpFilestr;//temp file "editor"

	//thread's -- 3
	std::thread m_trdLogthread;
	std::thread m_trdCleanLogthread;
	std::thread m_trdObjSizeThread;

	
	//vectors
	vector<std::string> m_strLogEntry;

	//misc
	std::mutex donelock;
	std::condition_variable donecond;
	std::mutex mylock;
// 	frame_rater<LOG_FRAMERATE> m_fr;//for our empty delay
// 	frame_rater<GAME_FPS> m_fr2;//for our filling delayW
	atomic<size_t> objsize;
	//frame_rater<LOG_FRAMERATE/2> m_fr3;//for our cleaning delay(we dont want our cleaner to be another performance killer

	//functions
	
	inline void mainthread_objsize() {
		frame_rater<GAME_FPS / 2> fr;
		while (!m_bStoplog) {
			objsize =
				(sizeof(bool) * 4) +//bools
				(sizeof(uint64_t) * 2) +//ints
				(sizeof(std::string) * 6) + (m_strDatentime.capacity() + m_strFilename.capacity() + m_strLogpath.capacity() + m_strTmpFilename.capacity() + m_strTmpLogpath.capacity() + m_modulename.capacity()) +//strings
				(sizeof(std::fstream) * 2) +//fstream
				(sizeof(std::thread) * 3) +//threads
				getVectorSize(m_strLogEntry) +//vectors
				(sizeof(std::mutex) * 2) + (sizeof(std::condition_variable)) +//misc
				0;
			fr.sleep();
			
		}


		
		
	}
	
	



	//date/time
	const std::string currentDateTime();
	std::string logdate();
	int createdir(const char* pathtofile);
	int openfile();
	int closefile();
	int openfile_tmp();//this is for the real-time log reading in future(normal english future, not std::)
	int closefile_tmp();//this is for the real-time log reading in future(normal english future, not std::)

	void cleanup_vectors();//to clean our log vectors
	



	//misc
	
	inline bool vectorcheck() {
		//bool temp = ((!m_strMessg.empty() && !m_iLogtype.empty()) && !m_strProg_module.empty()) && ;
		return !m_strLogEntry.empty();//checks if 3 main vectors are empty
	}
	inline size_t getVectorSize(vector<short>& vec) {
		size_t temp = sizeof(std::vector<short>) + (sizeof(short) * vec.capacity());
		return temp;
	}
	inline size_t getVectorSize(vector<std::string>& vec) {
		size_t temp = sizeof(std::vector<std::string>) + (sizeof(std::string) * vec.capacity());
		for (uint64_t i = 0; i < vec.size(); i++) {
			temp += vec[i].capacity();
		}
		return temp;
	}
	
	

	const char* checktype(int l_type);//checks for message type and returns word representation that corresponds to type
	
};
inline  Filelog g_flgDeflogger;//default logger, will be used if no logger is provided
//wont be moved to global_vars.hpp
//will probably cause infinite recursion for compiler
//I dont want that

#endif

