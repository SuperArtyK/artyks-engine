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


//important stuff
filelog::filelog() : mpr_strDatentime(""), mpr_strLogpath("logs/"), mpr_bThreadstarted(false), mpr_bStoplog(false), mpr_ullMessgcount(0), mpr_ldEntrycount(0.0) {
	createdir(mpr_strLogpath);
	mpr_strFilename = mpr_strLogpath + "LOG_" + logdate() + ".log";

	if (mpb_bDev)std::cout << "___copyconstruct1" << std::endl;
	
}
filelog::filelog(std::string pathtolog) : mpr_strDatentime(""), mpr_strLogpath(pathtolog), mpr_bThreadstarted(false), mpr_bStoplog(false), mpr_ullMessgcount(0) {
	if (mpr_strLogpath[mpr_strLogpath.length() - 2] != '/' || mpr_strLogpath[mpr_strLogpath.length() - 2] != '\\') {
		mpr_strLogpath += '/';
	}
	createdir(mpr_strLogpath);
	mpr_strFilename = mpr_strLogpath + "LOG_" + logdate() + ".log";

	if (mpb_bDev)std::cout << "___copyconstruct2" << std::endl;
	

}




//logging itself
int filelog::writetolog(std::string messg, int type, std::string prog_module) {

	if (messg == "Opening new logging session...") {
		mpr_vec_strMessg.insert(mpr_vec_strMessg.begin(), "Opening new logging session...");
		mpr_vec_iLogtype.insert(mpr_vec_iLogtype.begin(), 1);
		mpr_vec_strProg_module.insert(mpr_vec_strProg_module.begin(), "Logger");
		mpr_ullMessgcount++;
	}
	else {
		if (messg.empty()) {
			mpr_vec_strMessg.push_back("Sample entry. This logger object uses " + std::to_string(objsize()) + " bytes and has made " + std::to_string(mpr_ldEntrycount) + " log entries");
			mpr_vec_iLogtype.push_back(0);
			mpr_vec_strProg_module.push_back("Logger");
			mpr_ullMessgcount++;
		}
		else
		{
			mpr_vec_strMessg.push_back(messg);
			mpr_vec_iLogtype.push_back(type);
			mpr_vec_strProg_module.push_back(prog_module);
			mpr_ullMessgcount++;
		}
	}

	


	
	if (mpb_bDev)std::cout << "___gonna write to log \"" <<messg<<"\""<< std::endl;
	if (!mpr_bThreadstarted) {
		startlogging();

	}
	
	//mainthread();//for single thread debugging
	return 0;
}
int filelog::stoplogging() {
	//if (mpb_bDev)std::cout << "___stopping thread" << std::endl;
	writetolog("Closing current logging session...", 1, "Logger");
	if (mpr_bThreadstarted) {
		mpr_bStoplog = true;
		mpr_bThreadstarted = false;
		mpr_trdLogthread.join();
		return 0;
	}
	
	return 1;
}
int filelog::startlogging() {
	if (mpb_bDev)std::cout << "___creating thread again" << std::endl;
	mpr_bStoplog = false;
	mpr_bThreadstarted = true;
	mpr_trdLogthread = std::thread(&filelog::mainthread, this);
	writetolog("Opening new logging session...", 1, "Logger");
	return 0;
}
int filelog::mainthread() {
	std::string write;
	while (!mpr_bStoplog || mpr_ullMessgcount > 0)
	//while (mpr_ullMessgcount > 0) //for single thread debugging
	{
		//mpr_iWriteiterat++;
		

		if (vectorcheck()) {
			
			

			openfile();
			if (mpb_bDev)std::cout << "	writing to log \"" << mpr_vec_strMessg[0]<<"\" with "<< mpr_ullMessgcount <<" remaining"<< std::endl;
			write.clear();
			write = "[ " + currentDateTime() + " ] [";
			//vector<char> vchar;
			//mpr_fstFilestr.write(write.c_str(), write.length());
			//write.clear();
			switch (mpr_vec_iLogtype[0])
			{

			case 0:
				write += "INFO";
				break;


			case 1:
				write += "WARN";
				break;

			case 2:
				write += "ERROR";
				break;

			case 3:
				write += "FATAL ERROR";
				break;

			default:

				break;
			}
			write += "] [" + mpr_vec_strProg_module[0] + "]: " + mpr_vec_strMessg[0] + "\n";
			mpr_fstFilestr.write(write.c_str(), write.length());
			closefile();
			mpr_ldEntrycount++;
			if (mpb_bDev)std::cout << "	cleaning up with " << mpr_ldEntrycount << " entries ";
			mpr_vec_strMessg.erase(mpr_vec_strMessg.begin());
			mpr_vec_iLogtype.erase(mpr_vec_iLogtype.begin());
			mpr_vec_strProg_module.erase(mpr_vec_strProg_module.begin());
			if(mpr_ullMessgcount > 0){ this->mpr_ullMessgcount--; }
			if (mpb_bDev)cout << "and " << mpr_ullMessgcount << " remaining" << std::endl;
		
			
		}
		else
		{
			
			if(mpb_bDev)std::cout << "	something is wrong with data; " << mpr_ullMessgcount << " remaining and mpr_bStoplog " << mpr_bStoplog<<std::endl;
			




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
	std::string temp = currentDateTime();
	std::string temp2;
	short i = 0;
	while (temp[i] != '.') {
		temp2 += temp[i];
		i++;
	}
	return temp2;
}

int filelog::createdir(std::string pathtofile) {//creates directories of the path if they dont exist
	std::filesystem::create_directories(pathtofile);
	return 0;
}

int filelog::openfile() {
	//a crutch to show the data in file instantly, as std::fstream has some kind of buffer
	//so you need to close file after writing to it, so the data will update
	//cout << filenam << endl;
	if (mpb_bDev)std::cout << "	opening file "+ *mpr_strFilename.c_str() << std::endl;
	mpr_fstFilestr.open(mpr_strFilename.c_str(), std::fstream::out | std::fstream::app);
	if (mpr_fstFilestr.is_open()) {
		return 0;
	}
	if (mpb_bDev) {
		MessageBoxA(0, "FATAL ERROR:Could not open file for logging!\nThe program will now exit.", "FATAL ERROR:Filelog", MB_OK | MB_ICONERROR);
		//exit(1);
	}
	return 1;
}
int filelog::closefile() {
	if (mpb_bDev)std::cout << "	closing file" << std::endl;
	if (mpr_fstFilestr.is_open()) {
		mpr_fstFilestr.close();
		return 0;
	}
	if (mpb_bDev) {
		MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Logger(filelog)", MB_OK | MB_ICONWARNING);
	}
	return 1;
}

int filelog::movetoendSTR(std::string arr[], int n, int pos) {
	std::string save = arr[pos];

	for (int i = pos; i < n - 1; i++)
		arr[i] = arr[i + 1];

	arr[n - 1] = save;
	return 0;
}

int filelog::movetoendINT(int arr[], int n, int pos) {

	int save = arr[pos];

	for (int i = pos; i < n - 1; i++)
		arr[i] = arr[i + 1];

	arr[n - 1] = save;
	return 0;
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
