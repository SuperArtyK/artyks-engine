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
filelog::filelog(){

	m_strDatentime.clear();
	m_strLogpath = "logs/";
	m_strTmpLogpath = m_strLogpath + "tmp/";
	m_bThreadstarted = false;
	m_bStoplog = false;
	m_ullMessgcount = 0;
	m_ldEntrycount = 0;
	//m_bDev_log = true;


	
	createdir(m_strLogpath.c_str());
	createdir(m_strTmpLogpath.c_str());

	m_strFilename = m_strLogpath + "LOG_" + logdate() + ".log";
	m_strTmpFilename = m_strTmpLogpath + "LOG_" + logdate() + ".tmp";
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:copyconstruct1" << std::endl;
	
}
filelog::filelog(std::string l_strPathtolog = "logs/"){
	
	m_strDatentime.clear();
	m_strLogpath = l_strPathtolog;
	m_bThreadstarted = false;
	m_bStoplog = false;
	m_ullMessgcount = 0;
	m_ldEntrycount = 0;
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
	

}
//this just breaks logging for no reason
// filelog::filelog(int devenable)//>=0 is off, 1 is m_bDev_log, 2+ is m_bDev_log and m_bDev_cout
// {
// 	m_strDatentime.clear();
// 	m_strLogpath = "logs/";
// 	m_bThreadstarted = false;
// 	m_bStoplog = false;
// 	m_ullMessgcount = 0;
// 	m_ldEntrycount = 0;
// 	if (devenable == 1) {
// 
// 		m_bDev_log = true;
// 	}
// 	else
// 	{
// 		if (devenable>=2)
// 		{
// 			m_bDev_log = true;
// 			m_bDev_cout = true;//WARNING, WILL CAUSE MEM LEAK IF USED CONCURRENTLY!
// 		}
// 		else {
// 			m_bDev_log = false;
// 			m_bDev_cout = false;
// 		}
// 	}
// 
// 
// 	
// 	createdir(m_strLogpath.c_str());
// 	m_strFilename = m_strLogpath + "LOG_" + logdate() + ".log";
// 
// 
// }
// 
// filelog::filelog(std::string l_strPathtolog, int devenable)
// {
// 	m_strDatentime.clear();
// 	m_strLogpath = l_strPathtolog;
// 	m_bThreadstarted = false;
// 	m_bStoplog = false;
// 	m_ullMessgcount = 0;
// 	m_ldEntrycount = 0;
// 	if (devenable == 1) {
// 
// 		m_bDev_log = true;
// 	}
// 	else
// 	{
// 		if (devenable >= 2)
// 		{
// 			m_bDev_log = true;
// 			m_bDev_cout = true;//WARNING, WILL CAUSE MEM LEAK IF USED CONCURRENTLY!
// 		}
// 		else {
// 			m_bDev_log = false;
// 			m_bDev_cout = false;
// 		}
// 	}
// 
// 	
// 
// 	if (m_strLogpath[m_strLogpath.length() - 2] != '/' || m_strLogpath[m_strLogpath.length() - 2] != '\\') {
// 		m_strLogpath += '/';
// 	}
// 	createdir(m_strLogpath.c_str());
// 	m_strFilename = m_strLogpath + "LOG_" + logdate() + ".log";
// 
// 
// }

filelog::~filelog() {
	if (m_bThreadstarted)
		stoplogging();

	//cout << "Breaking file logger\n";
}


//logging itself
int filelog::writetolog(std::string l_strMessg, int l_iType, std::string l_strProg_module, bool debuglog) {
	if (!m_bThreadstarted) {
		startlogging();

	}
	
	m_ldEntrycount++;
	
	
	
	if (l_strMessg == "Opening new logging session...") {
		m_strMessg.insert(m_strMessg.begin(), "Opening new logging session...");
		m_iLogtype.insert(m_iLogtype.begin(), LOG_WARN);
		m_strProg_module.insert(m_strProg_module.begin(), m_modulename);
		m_bCleanUp.insert(m_bCleanUp.begin(), false);
		m_ullMessgcount++;
		return 0;
	}
	else {
		if (l_strMessg.empty()) {
			l_strMessg = "Sample entry. This logger object uses " + std::to_string(GetObjSize()) + " bytes and has made " + std::to_string(m_ldEntrycount) + " log entries";
			l_iType = LOG_INFO;
			l_strProg_module = m_modulename;
		}
	}
	
	mylock.lock();
	m_strMessg.push_back(l_strMessg);
	m_iLogtype.push_back(l_iType);
	m_strProg_module.push_back(l_strProg_module);
	m_bCleanUp.push_back(false);
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

int filelog::w2logtest(std::string l_strMessg, int l_iType, std::string l_strProg_module, bool debuglog)
{
	static std::string temp;
	if (!m_bThreadstarted) {
		startlogging();

	}

	m_ldEntrycount++;

	

	if (l_strMessg == "Opening new logging session...") {
		temp = "[ " + currentDateTime() + " ] [" + checktype(l_iType) + "] [" + l_strProg_module + "]: " + l_strMessg + "\n";
		strvec1.insert(strvec1.begin(), temp);
		m_ullMessgcount++;
		return 0;
	}
	else {
		if (l_strMessg.empty()) {
			temp = "[ " + currentDateTime() + " ] [" + checktype(LOG_INFO) + "] [" + m_modulename + "]: " + "Sample entry. This logger object uses " + std::to_string(GetObjSize()) + " bytes and has made " + std::to_string(m_ldEntrycount) + " log entries" + "\n";
			//mylock.lock();
			strvec1.push_back(temp);
			//mylock.unlock();
			return 0;
		}
	}
	temp = "[ " + currentDateTime() + " ] [" + checktype(l_iType) + "] [" + l_strProg_module + "]: " + l_strMessg + "\n";
	//mylock.lock();
	strvec1.push_back( temp);
	//mylock.unlock();

	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:gonna write to log \"" << l_strMessg <<"\""<< std::endl;

// 	//if (m_bDev_cout) {
// 		debugConOut(l_strMessg, l_iType, l_strProg_module);
// 
// 	}
	//mainthread();//for single thread debugging
	return 0;
}

int filelog::addtologqueue(std::string l_strMessg, int l_iType, std::string l_strProg_module, bool debuglog){
	m_ldEntrycount++;
	
	int logoffset = 0;
	if (!log_started) {
		m_iLogtype.insert(m_iLogtype.begin() + logoffset, LOG_SUCCESS);
		m_strProg_module.insert(m_strProg_module.begin() + logoffset, "Engine");
		m_strMessg.insert(m_strMessg.begin() + logoffset, "Started \"" + artyk::app_name + "\". Version: " + artyk::app_version + " Build: " + to_string(artyk::app_build));
		m_ullMessgcount++;
		//m_bCleanUp.push_back(false);
		m_bCleanUp.insert(m_bCleanUp.begin() + logoffset, false);
		log_started = true;
		logoffset++;
		
	}
	
	if (l_strMessg == "Opening new logging session...") {
		
		m_iLogtype.insert(m_iLogtype.begin() + logoffset, LOG_WARN);
		m_strProg_module.insert(m_strProg_module.begin(), m_modulename);
		m_strMessg.insert(m_strMessg.begin() + logoffset, "Opening new logging session...");
		m_ullMessgcount++;
		m_bCleanUp.insert(m_bCleanUp.begin() + logoffset, false);
		logoffset++;
		return 0;
	}
	else {
		if (l_strMessg.empty()) {
			l_strMessg = "Sample entry. This logger object uses " + std::to_string(GetObjSize()) + " bytes and has made " + std::to_string(m_ldEntrycount) + " log entries";
			l_iType = LOG_INFO;
			l_strProg_module = m_modulename;
		}
	}


	
	m_iLogtype.push_back(l_iType);
	m_strProg_module.push_back(l_strProg_module);
	m_strMessg.push_back(l_strMessg);
	m_bCleanUp.push_back(false);

	m_ullMessgcount++;
	return 0;
}
int filelog::stoplogging(bool closeprog) {
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:stopping writing thread" << std::endl;
	addtologqueue("Closing current logging session...", LOG_WARN, m_modulename);

	if (m_bThreadstarted) {
		m_bStoplog = true;
		m_bThreadstarted = false;
		m_trdLogthread.join();

		return 0;
	}
	
	return 1;
}
int filelog::startlogging() {
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:creating thread (again?)" << std::endl;
	m_bStoplog = false;
	m_bThreadstarted = true;
	m_trdLogthread = std::thread(&filelog::mainthread, this);
// 	if (!log_started) {
// 		addtologqueue("Started \"" + artyk::app_name+ "\". Version: "+artyk::app_version+" Build: "+to_string(artyk::app_build), LOG_SUCCESS, "Engine");
// 	}
	addtologqueue("Opening new logging session...", LOG_WARN, m_modulename);
	return 0;
}
int filelog::mainthread() {
	std::string writestr;
	//static bool closing = false;
	Screen myscr;
	while (!m_bStoplog || m_ullMessgcount > 0)
	//while (m_ullMessgcount > 0) //for single thread debugging
	{
		//mpr_iWriteiterat++;
// 		if (!closing && artyk::closing_app > 0) {
// 			selfdestruct();
// 			closing = true;
// 		}

		if (vectorcheck()) {
			
			
			//cout << m_ullMessgcount<< " ";
			openfile();
			openfile_tmp();
// 			if (m_strProg_module[0].empty()) {
// 				m_fr.sleep();
// 			}
			//if (m_bDev_cout)std::cout << "LOGGER_TRD:writing to log \"" << m_strMessg[0]<<"\" with "<< m_ullMessgcount <<" remaining"<< std::endl;
			writestr.clear();
			writestr = "[ " + currentDateTime() + " ] [" + checktype(m_iLogtype[0]) + "] [" + m_strProg_module[0] + "]: " + m_strMessg[0] + "\n";;
			//vector<char> vchar;
			//mpr_fstFilestr.write(write.c_str(), write.length());
			//write.clear();
			//writestr += checktype(m_iLogtype[0]);
			
			//writestr += "] [" + m_strProg_module[0] + "]: " + m_strMessg[0] + "\n";
			m_fstFilestr.write(writestr.c_str(), writestr.length());
			m_fstTmpFilestr.write(writestr.c_str(), writestr.length());
			myscr.settitle_ref(writestr);
			closefile();
			closefile_tmp();
			
			//m_bCleanUp[0] = true;
			//if (m_bDev_cout)std::cout << "	cleaning up with " << m_ldEntrycount << " entries ";
			mylock.lock();
			m_strMessg.erase(m_strMessg.begin());
			m_iLogtype.erase(m_iLogtype.begin());
			m_strProg_module.erase(m_strProg_module.begin());
			
			mylock.unlock();
			if(m_ullMessgcount > 0){ this->m_ullMessgcount--; }
			//if (m_bDev_cout)cout << "and " << m_ullMessgcount << " remaining" << std::endl;
			//m_fr2.sleep();
			
		}
		else
		{
			
			//if(m_bDev_cout)std::cout << "LOGGER_TRD:something is wrong with data or the message is empty/nonexistent;\n" << m_ullMessgcount << " remaining and mpr_bStoplog " << m_bStoplog<<std::endl;
			m_fr.sleep();
			continue;
   



		}
	}



	return 0;
}

//utils
const std::string filelog::currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buff[80];
	//tstruct = *localtime(&now);
	localtime_s(&tstruct, &now);
	strftime(buff, sizeof(buff), "%Y-%m-%d.%X", &tstruct);

	return buff;
}

std::string filelog::logdate() {
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

int filelog::createdir(const char* pathtofile) {//creates directories of the path if they dont exist
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

int filelog::openfile() {
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
int filelog::closefile() {
	//if (m_bDev_cout)std::cout << "	closing file" << std::endl;
	if (m_fstFilestr.is_open()) {
		m_fstFilestr.close();
		return 0;
	}
	//if (m_bDev_cout) {MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Logger(filelog)", MB_OK | MB_ICONWARNING);}
	return 1;
}

int filelog::openfile_tmp()
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

int filelog::closefile_tmp()
{
	//if (m_bDev_cout)std::cout << "	closing file" << std::endl;
	if (m_fstTmpFilestr.is_open()) {
		m_fstTmpFilestr.close();
		return 0;
	}
	//if (m_bDev_cout) {MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Logger(filelog)", MB_OK | MB_ICONWARNING);}
	return 1;
}

void filelog::cleanup_vectors()
{
}



const char* filelog::checktype(int l_type) {

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
//	Commented out code
///////////////////////////////////////////////////////////////

//change log path function, it doesnt work, WHY?!

// int filelog::changelogpath(std::string newlogpath) {
// 	logpath = newlogpath;
// 	if (logpath[logpath.length() - 1] != '/' || logpath[logpath.length() - 1] != '\\') {
// 		logpath += '/';
// 	}
// 	createdir(logpath);
// 	filenam = logpath + "LOG_" + logdate() + ".log";
// 	return 0;
// }



//old version of writetolog()


// 	int filelog::writetolog(std::string messg, int type = 0, std::string prog_module = "main") {//writes directly to log file
// 		//auto start = chrono::high_resolution_clock::now();
// 		//type 0 - info, 1-warn, 2-err, 3-fatal err
// 		//prog_module - module of the program that message is being sent from
// 		
// 		std::string write;
// 		write = "[ " + currentDateTime() + " ] [";
// 		//vector<char> vchar;
// 		filestr.write(write.c_str(), write.length());
// 		switch (type)
// 		{
// 
// 		case 0:
// 			write = "INFO";
// 			break;
// 
// 
// 		case 1:
// 			write = "WARN";
// 			break;
// 
// 		case 2:
// 			write = "ERROR";
// 			break;
// 
// 		case 3:
// 			write = "FATAL ERROR";
// 			break;
// 
// 		default:
// 			
// 			break;
// 		}
// 		write += "] [" + prog_module + "]: " + messg + "\n";
// 		filestr.write(write.c_str(), write.length());
// 		
// // 		auto end = chrono::high_resolution_clock::now();
// // 		double time_taken =
// // 			chrono::duration_cast<chrono::nanoseconds>(end - start).count();
// // 
// // 		time_taken *= 1e-9;
// // 		cout << "Time taken by program is : " << fixed
// // 			<< time_taken << setprecision(9);
// // 		cout << " sec" << endl;
// 		
// 		return 0;
// 
// 	}
