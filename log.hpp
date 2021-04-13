#pragma once

#ifndef FILELOG
#define FILELOG

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
arr array
vec vector
ptr pointer

etc..
*/

//#include "include.hpp"//includes EVERYTHING that I need
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <string>

using std::vector;

class filelog {

public:
	bool mpb_bDev;//for debug information
	filelog();

	filelog(std::string ml_strPathtolog);
	//int changelogpath(std::string newlogpath);
	//filelog& operator=(std::string differentpath);

	~filelog() {
		if(mpr_bThreadstarted)
		stoplogging();

	}
	




	int writetolog(std::string messg = "", int type = 0, std::string prog_module = "main");
	int stoplogging();
	int startlogging();
	int mainthread();
	
private:
	//functions
	bool mpr_bStoplog;//flag that for stopping/opening logging session
	
	int movetoendSTR(std::string arr[], int n, int pos);
	int movetoendINT(int arr[], int n, int pos);

			//date/time
	const std::string currentDateTime();
	std::string logdate();
	int createdir(std::string pathtofile);
	int openfile();
	int closefile();
	
	//misc
	size_t objsize() {
		size_t temp = sizeof(*this) + mpr_vec_strMessg.capacity() + mpr_vec_iLogtype.capacity() + mpr_vec_strProg_module.capacity();
		return temp;
	}
	bool vectorcheck() {

		return (!mpr_vec_strMessg.empty() && !mpr_vec_iLogtype.empty() && !mpr_vec_strProg_module.empty());
	}

	//vars
	std::string mpr_strDatentime;//stores date/time data
	std::string mpr_strFilename;
	std::fstream mpr_fstFilestr;//file "editor"
	std::string mpr_strLogpath;//log path variable
	bool mpr_bThreadstarted;
	std::thread mpr_trdLogthread;
	

			
	//const int mpr_cst_iMaxmessgcount = 256;
// 	std::string mpr_arr_strMessg[256];

	//logging vars
	vector<std::string> mpr_vec_strMessg;// = vector<std::string>(1, "Starting new logging session");
// 	int mpr_arr_iLogtype[256];
//  	std::string mpr_arr_strProg_module[256];
	uint64_t mpr_ullMessgcount;
	vector<int> mpr_vec_iLogtype;// = vector<int>(1, 0);
 	vector <std::string> mpr_vec_strProg_module;// = vector<std::string>(1, "main");
	
	

	//misc
	long double mpr_ldEntrycount;
	//long double mpr_ldWriteiterat = 0;
};
inline  filelog gi_flgDeflogger;//default logger





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


