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
#include "timer.hpp"
//#include "screen.hpp"

using std::vector;
using std::string;
using std::atomic;

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

class filelog {
public:
	atomic<bool> m_bDev_cout;//for debug information; outputs the info to main console
	//WARN: MAY CAUSE MEMORY LEAK IF MORE THAN 1 THREADS ARE OUTPUTTING TO CONSOLE!!
	atomic<bool> m_bDev_log;//for debug information; outputs debug filelog info to current log

	filelog();

	filelog(std::string l_strPathtolog);
//	this just breaks logging for no reason
// 	filelog(int devenable);
// 	filelog(std::string l_strPathtolog, int devenable);


	//int changelogpath(std::string newlogpath);
	//filelog& operator=(std::string differentpath);

	~filelog();
	

	int writetolog(std::string l_strMessg = "", int l_iType = 0, std::string l_strProg_module = "Logger", bool debuglog = false);
	int addtologqueue(std::string l_strMessg = "", int l_iType = 0, std::string l_strProg_module = "Logger", bool debuglog = false);//this is for safe logging, during important part of log itself(to avoid inf recursion)
	int stoplogging(bool closeprog = false);//flag is if we are closing the program.
	int startlogging();
	int mainthread();
	std::string getmodulename() { return m_modulename; }
private:
	
	frame_rater<LOG_FRAMERATE> m_fr;//for out empty delay
	frame_rater<GAME_FPS> m_fr2;//for out filling delay

	//functions
	atomic<bool> m_bStoplog;//flag that for stopping/opening logging session
	

	//date/time
	const std::string currentDateTime();
	std::string logdate();
	int createdir(const char* pathtofile);
	int openfile();
	int closefile();
	int openfile_tmp();//this is for the real-time log reading in future
	int closefile_tmp();//this is for the real-time log reading in future
// 	void selfdestruct() {
// 
// 
// 
// 		cout << "\nclosing.....";
// 		writetolog("Stopping modules...", LOG_INFO, m_modulename);
// 		//code here
// 		writetolog("Cleaning up...", LOG_INFO, m_modulename);
// 		//code here
// 		writetolog("Closed \"" + artyk::app_name + "\". Version: " + artyk::app_version + " Build: " + to_string(artyk::app_build), LOG_SUCCESS, "Engine");
// 		this->~filelog();
// 		artyk::closing_app++;
// 	}


	//misc
	inline size_t GetObjSize() {
		size_t temp = getVectorSize(m_strMessg) + getVectorSize(m_iLogtype) + getVectorSize(m_strProg_module) +//vectors
			sizeof(bool)* 4 +//bools
			sizeof(std::string)*(m_strDatentime.length() + m_strFilename.length() + m_strLogpath.length()+m_strTmpFilename.length()+m_strTmpLogpath.length())+//strings
			sizeof(m_fstFilestr)+sizeof(m_fstTmpFilestr)+sizeof(m_trdLogthread)+sizeof(m_ldEntrycount);//misc
		return temp;
	}
	inline bool vectorcheck() {
		//bool temp = ((!m_strMessg.empty() && !m_iLogtype.empty()) && !m_strProg_module.empty()) && ;
		return ((!m_strMessg.empty() && !m_iLogtype.empty()) && !m_strProg_module.empty());//checks if 3 main vectors are empty
	}
	inline size_t getVectorSize(vector<short>& vec) {
		size_t temp = sizeof(std::vector<short>) + (sizeof(short) * vec.size());
		return temp;
	}
	inline size_t getVectorSize(vector<std::string>& vec) {
		size_t temp = sizeof(std::vector<std::string>) + (sizeof(std::string) * vec.size());
		for (uint64_t i = 0; i < vec.size(); i++) {
			temp += vec[i].length();
		}
		return temp;
	}
	const char* checktype(int l_type);//checks for message type and returns word representation that corresponds to type

	//vars
	std::string m_strDatentime;//stores date/time data
	std::string m_strFilename;
	std::string m_strLogpath;//log path variable
	std::string m_strTmpFilename;
	std::string m_strTmpLogpath;//temp log path variable

	std::fstream m_fstFilestr;//file "editor"
	std::fstream m_fstTmpFilestr;//temp file "editor"
	bool m_bThreadstarted;
	std::thread m_trdLogthread;


	std::mutex mylock;
	
	//const int mpr_cst_iMaxmessgcount = 256;
// 	std::string mpr_arr_strMessg[256];

	//logging vars
	vector<std::string> m_strMessg;// = vector<std::string>(1, "Starting new logging session");
// 	int mpr_arr_iLogtype[256];
//  	std::string mpr_arr_strProg_module[256];
	uint64_t m_ullMessgcount;
	vector<short> m_iLogtype;// = vector<int>(1, 0);
	vector <std::string> m_strProg_module;// = vector<std::string>(1, "main");

	std::string m_modulename = "Logger";



	//misc
	uint64_t m_ldEntrycount;
	//long double mpr_ldWriteiterat = 0;
};
inline  filelog g_flgDeflogger;//default logger
//wont be moved to global_vars.hpp
//will probably cause infinite recursion for compiler
//I dont want that

#endif

