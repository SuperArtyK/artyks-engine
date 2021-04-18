#include "log.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <windows.h>
//using namespace std;
using std::vector;
using std::cout;
using std::cin;//if I'll need it sometime
using std::endl;

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


//functions for global usage
//will move to separate file
const char* BoolToString(bool b);
int movetoendSTR(std::string l_strArr[], int l_iN, int l_iPos);
int movetoendINT(int l_iArr[], int l_iN, int l_iPos);



//important stuff
filelog::filelog() : m_strDatentime(""), m_strLogpath("logs/"), m_bThreadstarted(false), m_bStoplog(false), m_ullMessgcount(0), m_ldEntrycount(0.0), m_bDev_log(false) {
	createdir(m_strLogpath.c_str());
	m_strFilename = m_strLogpath + "LOG_" + logdate() + ".log";

	if (m_bDev_cout)std::cout << "LOGGER_MAIN:copyconstruct1" << std::endl;
	
}
filelog::filelog(std::string l_strPathtolog) : m_strDatentime(""), m_strLogpath(l_strPathtolog), m_bThreadstarted(false), m_bStoplog(false), m_ullMessgcount(0), m_bDev_log(false) {
	if (m_strLogpath[m_strLogpath.length() - 2] != '/' || m_strLogpath[m_strLogpath.length() - 2] != '\\') {
		m_strLogpath += '/';
	}
	createdir(m_strLogpath.c_str());
	m_strFilename = m_strLogpath + "LOG_" + logdate() + ".log";

	if (m_bDev_cout)std::cout << "LOGGER_MAIN:copyconstruct2" << std::endl;
	

}




//logging itself
int filelog::writetolog(std::string l_strMessg, int l_iType, std::string l_strProg_module) {

	if (l_strMessg == "Opening new logging session...") {
		m_strMessg.insert(m_strMessg.begin(), "Opening new logging session...");
		m_iLogtype.insert(m_iLogtype.begin(), 1);
		m_strProg_module.insert(m_strProg_module.begin(), "Logger");
		m_ullMessgcount++;
	}
	else {
		if (l_strMessg.empty()) {
			m_strMessg.push_back("Sample entry. This logger object uses " + std::to_string(objsize()) + " bytes and has made " + std::to_string(m_ldEntrycount) + " log entries");
			m_iLogtype.push_back(0);
			m_strProg_module.push_back("Logger");
			m_ullMessgcount++;
		}
		else
		{
			m_strMessg.push_back(l_strMessg);
			m_iLogtype.push_back(l_iType);
			m_strProg_module.push_back(l_strProg_module);
			m_ullMessgcount++;
		}
	}

	


	
	if (m_bDev_cout)std::cout << "LOGGER_MAIN:gonna write to log \"" << l_strMessg <<"\""<< std::endl;
	if (!m_bThreadstarted) {
		startlogging();

	}
	
	//mainthread();//for single thread debugging
	return 0;
}
int filelog::stoplogging() {
	if (m_bDev_cout)std::cout << "LOGGER_MAIN:stopping writing thread" << std::endl;
	writetolog("Closing current logging session...", 1, "Logger");
	if (m_bThreadstarted) {
		m_bStoplog = true;
		m_bThreadstarted = false;
		m_trdLogthread.join();
		return 0;
	}
	
	return 1;
}
int filelog::startlogging() {
	if (m_bDev_cout)std::cout << "LOGGER_MAIN:creating thread (again?)" << std::endl;
	m_bStoplog = false;
	m_bThreadstarted = true;
	m_trdLogthread = std::thread(&filelog::mainthread, this);
	writetolog("Opening new logging session...", 1, "Logger");
	return 0;
}
int filelog::mainthread() {
	std::string writestr;
	while (!m_bStoplog || m_ullMessgcount > 0)
	//while (mpr_ullMessgcount > 0) //for single thread debugging
	{
		//mpr_iWriteiterat++;
		

		if (vectorcheck()) {
			
			

			openfile();
			if (m_bDev_cout)std::cout << "LOGGER_TRD:writing to log \"" << m_strMessg[0]<<"\" with "<< m_ullMessgcount <<" remaining"<< std::endl;
			writestr.clear();
			writestr = "[ " + currentDateTime() + " ] [";
			//vector<char> vchar;
			//mpr_fstFilestr.write(write.c_str(), write.length());
			//write.clear();
			switch (m_iLogtype[0])
			{

			case 0:
				writestr += "INFO";
				break;


			case 1:
				writestr += "WARN";
				break;

			case 2:
				writestr += "ERROR";
				break;

			case 3:
				writestr += "FATAL ERROR";
				break;

			default:

				break;
			}
			writestr += "] [" + m_strProg_module[0] + "]: " + m_strMessg[0] + "\n";
			m_fstFilestr.write(writestr.c_str(), writestr.length());
			closefile();
			m_ldEntrycount++;
			if (m_bDev_cout)std::cout << "	cleaning up with " << m_ldEntrycount << " entries ";
			m_strMessg.erase(m_strMessg.begin());
			m_iLogtype.erase(m_iLogtype.begin());
			m_strProg_module.erase(m_strProg_module.begin());
			if(m_ullMessgcount > 0){ this->m_ullMessgcount--; }
			if (m_bDev_cout)cout << "and " << m_ullMessgcount << " remaining" << std::endl;
		
			
		}
		else
		{
			
			if(m_bDev_cout)std::cout << "LOGGER_TRD:something is wrong with data or the message is empty/nonexistent;\n" << m_ullMessgcount << " remaining and mpr_bStoplog " << m_bStoplog<<std::endl;
			




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
	if (m_bDev_log)writetolog("Getting Date and Time for the Log file...", 0, "Logger");
	std::string temp = currentDateTime();
	std::string temp2;
	short i = 0;
	while (temp[i] != '.') {
		temp2 += temp[i];
		i++;
	}
	
	if (m_bDev_cout)writetolog("OK! Got the log file date: "+temp2, 0, "Logger");
	return temp2;
}

int filelog::createdir(const char* pathtofile) {//creates directories of the path if they dont exist
	if (m_bDev_log)writetolog("Creating directory for log file "+*pathtofile, 0, "Logger");
	bool result;
	result = std::filesystem::create_directories(pathtofile);
	if (result) {
		if (m_bDev_log)writetolog("OK! Successfully created directory for log file" +*pathtofile, 0, "Logger");
	}
	else
	{
		if (m_bDev_log)writetolog("ERROR! Could not create directory for log file" + *pathtofile, 2, "Logger");
	}

	return 0;
}

int filelog::openfile() {
	//a crutch to show the data in file instantly, as std::fstream has some kind of buffer
	//so you need to close file after writing to it, so the data will update
	//cout << filenam << endl;
	if (m_bDev_cout)std::cout << "opening file "+ *m_strFilename.c_str() << std::endl;
	if (m_bDev_log)writetolog("Opening log file \"" + m_strFilename + "\n...", 0, "Logger");
	m_fstFilestr.open(m_strFilename.c_str(), std::fstream::out | std::fstream::app);
	if (m_fstFilestr.is_open()) {
		if (m_bDev_log)writetolog("OK! Opened file \"" + m_strFilename + "\n for logging", 0, "Logger");
		return 0;

	}
	
		MessageBoxA(0, "FATAL ERROR:Could not open file for logging!\nThe program will now exit.", "FATAL ERROR:Filelog", MB_OK | MB_ICONERROR);
		if (m_bDev_log)writetolog("ERROR! Could not create log file" + m_strFilename, 2, "Logger");//wont be written, but 
		//exit(1);
	
	return 1;
}
int filelog::closefile() {
	if (m_bDev_cout)std::cout << "	closing file" << std::endl;
	if (m_fstFilestr.is_open()) {
		m_fstFilestr.close();
		return 0;
	}
	if (m_bDev_cout) {
		MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Logger(filelog)", MB_OK | MB_ICONWARNING);
	}
	return 1;
}
















//definitions of global functions
//will be moved to separate files later
int movetoendSTR(std::string l_strarr[], int l_in, int l_ipos) {
	std::string save = l_strarr[l_in];

	for (int i = l_ipos; i < l_in - 1; i++)
		l_strarr[i] = l_strarr[i + 1];

	l_strarr[l_in - 1] = save;
	return 0;
}

int movetoendINT(int l_iArr[], int l_iN, int l_iPos) {

	int save = l_iArr[l_iPos];

	for (int i = l_iPos; i < l_iN - 1; i++)
		l_iArr[i] = l_iArr[i + 1];

	l_iArr[l_iN - 1] = save;
	return 0;
}


const char* BoolToString(bool b)
{
	return b ? "OK!" : "ERROR!";
}


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
