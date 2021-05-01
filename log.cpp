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
// this file contains the logger functions' definitions(refer to log.hpp)
// should not cause everything to break
//////////////////////////////////////////////////////////////////////////

#include "timer.hpp"
#include "log.hpp"
#include "screen.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <windows.h>
#include <conio.h>
//#include "screen.hpp"
//using namespace std;
using std::vector;
using std::cout;
using std::cin;//if I'll need it sometime
using std::endl;

//functions for global usage
//moved to global_functions.hpp file
// const char* BoolToString(bool b);
// int movetoendSTR(std::string l_strArr[], int l_iN, int l_iPos);
// int movetoendINT(int l_iArr[], int l_iN, int l_iPos);



//important stuff
Filelog::Filelog(){

	m_strDatentime.clear();
	m_strLogpath = "logs/";
	m_strTmpLogpath = m_strLogpath + "tmp/";
	m_bThreadstarted = false;
	m_bStoplog = false;
	m_ullMessgcount = 0;
	m_ullEntrycount = 0;
	//m_bDev_log = true;


	
	createdir(m_strLogpath.c_str());
	createdir(m_strTmpLogpath.c_str());

	m_strFilename = m_strLogpath + "LOG_" + logdate() + ".log";
	m_strTmpFilename = m_strTmpLogpath + "LOG_" + logdate() + ".tmp";
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:copyconstruct1" << std::endl;
	//m_strLogEntry.reserve(10000);
}
Filelog::Filelog(std::string l_strPathtolog = "logs/"){
	
	m_strDatentime.clear();
	m_strLogpath = l_strPathtolog;
	m_bThreadstarted = false;
	m_bStoplog = false;
	m_ullMessgcount = 0;
	m_ullEntrycount = 0;
	m_bDev_log = false;
	
	
		
	if (m_strLogpath[m_strLogpath.length() - 2] != '/' || m_strLogpath[m_strLogpath.length() - 2] != '\\') {
		m_strLogpath += '/';
	}
	m_strTmpLogpath = m_strLogpath + "tmp/";
	createdir(m_strLogpath.c_str());
	createdir(m_strTmpLogpath.c_str());
	m_strFilename = m_strLogpath + "LOG_" + logdate() + ".log";
	m_strFilename = m_strTmpLogpath + "LOG_" + logdate() + ".tmp";

	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:copyconstruct2" << std::endl;
	
	//m_strLogEntry.reserve(10000);
}

Filelog::~Filelog() {
	if (m_bThreadstarted)
		stoplogging();

	//cout << "Breaking file logger\n";
}


//logging itself
int Filelog::writetolog(std::string l_strMessg, int l_iType, std::string l_strProg_module, bool debuglog) {
	if (!m_bThreadstarted) {
		startlogging();

	}
	m_ullEntrycount++;
	
	//static std::string tempstr;
	//tempstr.clear();
	
	if (l_strMessg.empty()) {
		mylock.lock();
		m_strLogEntry.push_back("[ " + currentDateTime() + " ] [" + checktype(LOG_INFO) + "] [" + m_modulename + "]: Sample entry. This logger object uses " + std::to_string(objsize) + " bytes and has made " + std::to_string(m_ullEntrycount) + " log entries\n");
		m_ullMessgcount++;
		mylock.unlock();
		return 0;
	}
	
	mylock.lock();
	m_strLogEntry.push_back("[ " + currentDateTime() + " ] [" + checktype(l_iType) + "] [" + l_strProg_module + "]: " + l_strMessg + "\n");
	m_ullMessgcount++;
	mylock.unlock();
	
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:gonna write to log \"" << l_strMessg <<"\""<< std::endl;
	
// 	//if (m_bDev_cout) {
// 		debugConOut(l_strMessg, l_iType, l_strProg_module);
// 
// 	}
	//mainthread();//for single thread debugging
	return 0;
}

int Filelog::addtologqueue(std::string l_strMessg, int l_iType, std::string l_strProg_module, bool debuglog){
	m_ullEntrycount++;
	
	int logoffset = 0;
	std::string temp;
	if (!log_started) {
		m_strLogEntry.insert(m_strLogEntry.begin(), "[ " + currentDateTime() + " ] [" + checktype(LOG_SUCCESS) + "] [Engine]: Started \"" + artyk::app_name + "\". Version: " + artyk::app_version + " Build: " + to_string(artyk::app_build) + "\n");

		m_ullMessgcount++;
		//m_bCleanUp.push_back(false);
		//m_bCleanUp.insert(m_bCleanUp.begin() + logoffset, false);
		log_started = true;
		logoffset++;
		
	}
	
	if (l_strMessg == "Opening new logging session...") {
		m_strLogEntry.insert(m_strLogEntry.begin()+logoffset, "[ " + currentDateTime() + " ] [" + checktype(l_iType) + "] [" + m_modulename + "]: " + l_strMessg + "\n");

		m_ullMessgcount++;
		//m_bCleanUp.insert(m_bCleanUp.begin() + logoffset, false);
		logoffset++;
		return 0;
	}
	else {
		if (l_strMessg.empty()) {
			temp = "[ " + currentDateTime() + " ] [" + checktype(LOG_INFO) + "] [" + m_modulename + "]: Sample entry. This logger object uses " + std::to_string(objsize) + " bytes and has made " + std::to_string(m_ullEntrycount) + " log entries\n";
			m_strLogEntry.push_back(temp);
			return 0;
		}
	}


	m_strLogEntry.push_back("[ " + currentDateTime() + " ] [" + checktype(l_iType) + "] [" + l_strProg_module + "]: " + l_strMessg + "\n");
	//m_bCleanUp.push_back(false);
	m_ullMessgcount++;
	return 0;
}
int Filelog::stoplogging(bool closeprog) {
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:stopping writing thread" << std::endl;
	addtologqueue("Closing current logging session...", LOG_WARN, m_modulename);

	if (m_bThreadstarted) {
		m_bStoplog = true;
		m_bThreadstarted = false;
		m_trdLogthread.join();
		m_trdObjSizeThread.join();

		return 0;
	}
	
	return 1;
}
int Filelog::startlogging() {
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:creating thread (again?)" << std::endl;
	m_bStoplog = false;
	m_bThreadstarted = true;
	m_trdLogthread = std::thread(&Filelog::mainthread, this);
	m_trdObjSizeThread = std::thread(&Filelog::mainthread_objsize, this);

// 	if (!log_started) {
// 		addtologqueue("Started \"" + artyk::app_name+ "\". Version: "+artyk::app_version+" Build: "+to_string(artyk::app_build), LOG_SUCCESS, "Engine");
// 	}
	addtologqueue("Opening new logging session...", LOG_WARN, m_modulename);
	return 0;
}
int Filelog::mainthread() {
	frame_rater<GAME_FPS/4> fr;
	Screen myscr;
	openfile();
	//myscr.settitle_val("entered log mtrd");
	//_getch();
	while (!m_bStoplog || m_ullMessgcount > 0)
	{

		if (vectorcheck()) {

			
			openfile_tmp();
			m_fstFilestr.write(m_strLogEntry[0].c_str(), m_strLogEntry[0].length());
			//m_fstTmpFilestr.write(m_strLogEntry[0].c_str(), m_strLogEntry[0].length());
			
			//closefile_tmp();
			myscr.settitle_ref(m_strLogEntry[0]);
			mylock.lock();
			m_strLogEntry.erase(m_strLogEntry.begin());
			m_ullMessgcount--;
			mylock.unlock();
			//myscr.settitle_ref(to_string(m_ullMessgcount));
			
			//
		}
		else
		{
			closefile();
			openfile();
			//to update the file, if we're sleeping
			fr.sleep();

		}

	}

	closefile();
	return 0;
}

//utils
const std::string Filelog::currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buff[80];
	//tstruct = *localtime(&now);
	localtime_s(&tstruct, &now);
	strftime(buff, sizeof(buff), "%Y-%m-%d.%X", &tstruct);

	return buff;
}

std::string Filelog::logdate() {
	if (m_bDev_log)addtologqueue("Getting Date and Time for the Log file...", LOG_INFO, m_modulename);
	std::string temp = currentDateTime();
	std::string temp2;
	short i = 0;
	while (temp[i] != '.') {
		temp2 += temp[i];
		i++;
	}
	
	if (m_bDev_log)addtologqueue("OK! Got the log file date: "+temp2, LOG_OK, m_modulename);
	return temp2;
}

int Filelog::createdir(const char* pathtofile) {//creates directories of the path if they dont exist
	if (m_bDev_log)addtologqueue("Creating directory for log file "+*pathtofile, LOG_INFO, m_modulename);
	bool result;
	result = std::filesystem::create_directories(pathtofile);
// 	if (result) {
// 		if (m_bDev_log)addtologqueue("SUCCESS! Successfully created directory for log file" +*pathtofile, LOG_SUCCESS, modulename);
// 	}
// 	else
// 	{
// 		if (m_bDev_log)addtologqueue("ERROR! Could not create directory for log file" + *pathtofile, LOG_ERROR, modulename);
// 	}

	return result;
}

int Filelog::openfile() {
	//a crutch to show the data in file instantly, as std::fstream has some kind of buffer
	//so you need to close file after writing to it, so the data will update
	//cout << filenam << endl;
	//if (m_bDev_cout)std::cout << "opening file "+ *m_strFilename.c_str() << std::endl;
	//if (m_bDev_log)addtologqueue("Opening log file \"" + m_strFilename + "\n...", LOG_INFO, modulename);
	m_fstFilestr.open(m_strFilename.c_str(), std::fstream::out | std::fstream::app);
	if (m_fstFilestr.is_open()) {
		//if (m_bDev_log)addtologqueue("SUCCESS! Opened file \"" + m_strFilename + "\n for logging", LOG_SUCCESS, modulename);
		return 0;

	}
	
		//MessageBoxA(0, "FATAL ERROR:Could not open file for logging!\nThe program will now exit.", "FATAL ERROR:Filelog", MB_OK | MB_ICONERROR);
		//if (m_bDev_log)addtologqueue("FATAL ERROR! Could not create log file" + m_strFilename, LOG_FERROR, modulename);//wont be written, but 
		//exit(1);
	
	return 1;
}
int Filelog::closefile() {
	//if (m_bDev_cout)std::cout << "	closing file" << std::endl;
	if (m_fstFilestr.is_open()) {
		m_fstFilestr.close();
		return 0;
	}
	//if (m_bDev_cout) {MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Logger(filelog)", MB_OK | MB_ICONWARNING);}
	return 1;
}

int Filelog::openfile_tmp()
{//a crutch to show the data in file instantly, as std::fstream has some kind of buffer
	//so you need to close file after writing to it, so the data will update
	//cout << filenam << endl;
	//if (m_bDev_cout)std::cout << "opening file "+ *m_strFilename.c_str() << std::endl;
	//if (m_bDev_log)addtologqueue("Opening log file \"" + m_strFilename + "\n...", LOG_INFO, modulename);
	//cout << m_strTmpFilename << endl;
	m_fstTmpFilestr.open(m_strTmpFilename.c_str(), std::fstream::out | std::fstream::trunc);
	if (m_fstTmpFilestr.is_open()) {
		//if (m_bDev_log)addtologqueue("SUCCESS! Opened file \"" + m_strFilename + "\n for logging", LOG_SUCCESS, modulename);
		return 0;

	}

	//MessageBoxA(0, "FATAL ERROR:Could not open file for logging!\nThe program will now exit.", "FATAL ERROR:Filelog", MB_OK | MB_ICONERROR);
	//if (m_bDev_log)addtologqueue("FATAL ERROR! Could not create log file" + m_strFilename, LOG_FERROR, modulename);//wont be written, but 
	//exit(1);

	return 1;
	
}

int Filelog::closefile_tmp()
{
	//if (m_bDev_cout)std::cout << "	closing file" << std::endl;
	if (m_fstTmpFilestr.is_open()) {
		m_fstTmpFilestr.close();
		return 0;
	}
	//if (m_bDev_cout) {MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Logger(filelog)", MB_OK | MB_ICONWARNING);}
	return 1;
}

void Filelog::cleanup_vectors()
{
}



const char* Filelog::checktype(int l_type) {

	switch (l_type)
	{

	case LOG_INFO:
		return "INFO";
		break;


	case LOG_WARN:
		return "WARN";
		break;

	case LOG_ERROR:
		return "ERROR";
		break;

	case LOG_FERROR:
		return "FATAL_ERROR";
		break;

	case LOG_SWARN:
		return "SEVERE_WARN";
		break;

	case LOG_OK:
		return "OK";
		break;

	case LOG_SUCCESS:
		return "SUCCESS";
		break;

	default:
		return "INFO";//default will be info, 
		break;
	}

}










//definitions of global functions
//moved to global_functions.hpp
//int movetoendSTR(std::string l_strarr[], int l_in, int l_ipos);
//int movetoendINT(int l_iArr[], int l_iN, int l_iPos);
//const char* BoolToString(bool b);


///////////////////////////////////////////////////////////////
//	Commented out code moved to logger_old_code.txt
///////////////////////////////////////////////////////////////




