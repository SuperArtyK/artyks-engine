#include "log.hpp"
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>
#include <windows.h>
//using namespace std;


//important stuff
filelog::filelog() : datentime(""), logpath("logs/"), threadstarted(false), stoplog(false), messgcount(0) {
	createdir(logpath);
	filenam = logpath + "LOG_" + logdate() + ".log";

	for (int i = 0; i < maxmessgcount; i++) {
		messg[i].clear();
		type[i] = -1;
		prog_module[i].clear();
	}
	//if(dev)std::cout << "___creating thread"<<std::endl;
	//if (dev)std::cout << "___copyconstruct1" << std::endl;
	
}
filelog::filelog(std::string pathtolog) : datentime(""), logpath(pathtolog), threadstarted(false), stoplog(false), messgcount(0) {
	if (logpath[logpath.length() - 1] != '/' || logpath[logpath.length() - 1] != '\\') {
		logpath += '/';
	}
	createdir(logpath);
	filenam = logpath + "LOG_" + logdate() + ".log";
	for (int i = 0; i < maxmessgcount; i++) {
		messg[i].clear();
		type[i] = -1;
		prog_module[i].clear();
	}
	//if(dev)std::cout << "___creating thread" << std::endl;
	//std::cout << "___copyconstruct2" << std::endl;
	

}

// int filelog::changelogpath(std::string newlogpath) {
// 	logpath = newlogpath;
// 	if (logpath[logpath.length() - 1] != '/' || logpath[logpath.length() - 1] != '\\') {
// 		logpath += '/';
// 	}
// 	createdir(logpath);
// 	filenam = logpath + "LOG_" + logdate() + ".log";
// 	return 0;
// }



//logging itself
int filelog::writetolog(std::string messg, int type, std::string prog_module) {

	this->messg[messgcount] = messg;
	this->type[messgcount] = type;
	this->prog_module[messgcount] = prog_module;
	messgcount++;
// 	std::cout << messgcount << std::endl;
// 	for (int i = 0; i < messgcount; i++) {
// 
// 		std::cout << this->messg[i] << std::endl;
// 	}
	if (!threadstarted) {
		startlogging();

	}
	//if(dev)std::cout << "___gonna write to log" << std::endl;
	//mainthread();
	return 0;
}
int filelog::stoplogging() {
	//if(dev)std::cout << "___stopping thread" << std::endl;
	//if(dev)writetolog("Closing current logging session", 1, "Logger");
	stoplog = true;
	threadstarted = false;
	logthread.join();
	return 0;
}
int filelog::startlogging() {
	//if(dev)std::cout << "___creating thread again" << std::endl;
	stoplog = false;
	threadstarted = true;
	logthread = std::thread(&filelog::mainthread, this);
	//if(dev)writetolog("Opening new logging session", 1, "Logger");
	return 0;
}
int filelog::mainthread() {
	while (!stoplog || messgcount > 0)
	{
		
		std::string write;

		if (!this->messg[0].empty()) {
			
			

			openfile();
			//if(dev)std::cout << "___writing to log" << messg[0]<< std::endl;
			write.clear();
			write = "[ " + currentDateTime() + " ] [";
			//vector<char> vchar;
			filestr.write(write.c_str(), write.length());
			write.clear();
			switch (type[0])
			{

			case 0:
				write = "INFO";
				break;


			case 1:
				write = "WARN";
				break;

			case 2:
				write = "ERROR";
				break;

			case 3:
				write = "FATAL ERROR";
				break;

			default:

				break;
			}
			write += "] [" + prog_module[0] + "]: " + messg[0] + "\n";
			filestr.write(write.c_str(), write.length());
			closefile();
			//if(dev)std::cout << "___cleaning up" << std::endl;
			this->messg[0].clear(); 
			movetoendSTR(messg, maxmessgcount, 0);
			this->type[0] = -1; 
			movetoendINT(type, maxmessgcount, 0);
			this->prog_module[0].clear(); 
			movetoendSTR(prog_module, maxmessgcount, 0);//default
			this->messgcount--;
		}
		else
		{
			//if(dev)std::cout << "___something is wrong with data" << messg[0] << std::endl;
			if (this->messg[0].empty()) { 
				movetoendSTR(this->messg, maxmessgcount, 0); 
			}
			if (this->type[0] != -1) { 
				movetoendINT(this->type, maxmessgcount, 0);
			}
			if (this->prog_module[0].empty()) {
				movetoendSTR(this->messg, maxmessgcount, 0); 
			}




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
	//a crutch to showing the data in file instantly, as std::fstream has some kind of buffer
	//so you need to close file after writing to it, so the data will update
	//cout << filenam << endl;
	//if(dev)std::cout << "___opening file" << std::endl;
	filestr.open(filenam.c_str(), std::fstream::out | std::fstream::app);
	if (filestr.is_open()) {
		return 0;
	}
	if (dev) {
		MessageBoxA(0, "FATAL ERROR:Could not open file for logging!\nThe program will now exit.", "FATAL ERROR:Filelog", MB_OK | MB_ICONERROR);
		//exit(1);
	}
	return 1;
}
int filelog::closefile() {
	//if(dev)std::cout << "___closing file" << std::endl;
	if (filestr.is_open()) {
		filestr.close();
		return 0;
	}
	if (dev) {
		MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Filelog", MB_OK | MB_ICONWARNING);
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