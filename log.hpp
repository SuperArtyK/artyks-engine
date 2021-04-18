#pragma once

#ifndef FILELOG
#define FILELOG

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

//#include "include.hpp"//includes EVERYTHING that I need
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
#include "screen.hpp"


using std::vector;
using std::string;
using std::atomic;


class filelog {

public:
	atomic<bool> m_bDev_cout;//for debug information; outputs the info to main console
	//WARN: MAY CAUSE MEMORY LEAK IF MORE THAN 1 THREADS ARE OUTPUTTING!
	atomic<bool> m_bDev_log;//for debug information; outputs debug filelog info to current log

	filelog();

	filelog(std::string l_strPathtolog);
	//int changelogpath(std::string newlogpath);
	//filelog& operator=(std::string differentpath);

	~filelog() {
		if(m_bThreadstarted)
		stoplogging();

	}
	void debugConOut();//outputs debug messages to console





	int writetolog(std::string l_strMessg = "", int l_iType = 0, std::string l_strProg_module = "main");
	int stoplogging();
	int startlogging();
	int mainthread();
	
private:
	//functions
	atomic<bool> m_bStoplog;//flag that for stopping/opening logging session
	
	

			//date/time
	const std::string currentDateTime();
	std::string logdate();
	int createdir(const char *pathtofile);
	int openfile();
	int closefile();
	
	//misc
	size_t objsize() {
		size_t temp = sizeof(*this) + m_strMessg.capacity() + m_iLogtype.capacity() + m_strProg_module.capacity();
		return temp;
	}
	bool vectorcheck() {

		return (!m_strMessg.empty() && !m_iLogtype.empty() && !m_strProg_module.empty());
	}

	//vars
	std::string m_strDatentime;//stores date/time data
	std::string m_strFilename;
	std::fstream m_fstFilestr;//file "editor"
	std::string m_strLogpath;//log path variable
	bool m_bThreadstarted;
	std::thread m_trdLogthread;
	

			
	//const int mpr_cst_iMaxmessgcount = 256;
// 	std::string mpr_arr_strMessg[256];

	//logging vars
	vector<std::string> m_strMessg;// = vector<std::string>(1, "Starting new logging session");
// 	int mpr_arr_iLogtype[256];
//  	std::string mpr_arr_strProg_module[256];
	uint64_t m_ullMessgcount;
	vector<int> m_iLogtype;// = vector<int>(1, 0);
 	vector <std::string> m_strProg_module;// = vector<std::string>(1, "main");
	
	

	//misc
	long double m_ldEntrycount;
	//long double mpr_ldWriteiterat = 0;
};
inline  filelog g_flgDeflogger;//default logger





#endif


//     example how to use :
//     
//     filelog mylog("path/to/log");//this creates an obj from log class
//     //with the path to create the log file in "path/to/log"
//     //even on windows, use forward slash for the subdirectory dividing
//     
//     filelog mylog2;
//     //without any std::string value("std::string"), it will default to log path ./logs/
//     //in the same directory as executable
//     //again it will type in log what you feed it
//     
//     mylog.writetolog("Log is working!");//0 or any other number(or it's absense will result in INFO log entry
//     mylog.writetolog("This is warning", 1);
//     mylog.writetolog("This is error", 2);
//     mylog.writetolog("This is fatal error", 3);
//     
//     mylog2.writetolog("Log is working!", 0, "Logger");//3rd argument is optional, module of the program that message is being sent from
//     //but 2nd argument is required to be passed
//     //not passing(not leaving it blank!) will result in default "main" to be put in the module section
//     mylog2.writetolog("This is warning", 1, "YourTeacher");
//     mylog2.writetolog("This is error", 2, "GNU C/C++ Compiler");
//     mylog2.writetolog("This is fatal error", 3, "Game engine");
//     
//     
//     will result to this in the 1st log file in "/path/to/log" subdirectory
//     
//     [2021 - 04 - 07.09:55 : 32][INFO][main]: Log is working!
//     [2021 - 04 - 07.09:55 : 32][WARN][main] : This is warning
//     [2021 - 04 - 07.09:55 : 32][ERROR][main] : This is error
//     [2021 - 04 - 07.09:55 : 32][FATAL ERROR][main] : This is fatal error
//     
//     Will result to this in the 2nd log file in "/logs/" subdirectory
//     
//     [2021 - 04 - 07.09:55 : 32][INFO][Logger] : Log is working!
//     [2021 - 04 - 07.09:55 : 32][WARN][YourTeacher] : This is warning
//     [2021 - 04 - 07.09:55 : 32][ERROR][GNU C / C++ Compiler] : This is error
//     [2021 - 04 - 07.09:55 : 32][FATAL ERROR][Game engine] : This is fatal error
// 


