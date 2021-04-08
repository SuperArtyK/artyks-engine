#pragma once

#ifndef FILELOG
#define FILELOG

//     example how to use :
//     
//     filelog mylog("path/to/log");//this creates an obj from log class
//     //with the path to create the log file in "path/to/log"
//     //even on windows, use forward slash for the subdirectory dividing
//     
//     filelog mylog2;
//     //without any string value("string"), it will default to log path ./logs/
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






//#include "include.hpp"//includes EVERYTHING that I need
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
using namespace std;

const int maxtries = 10000;

class filelog {

public:

	filelog() : datentime(""), logpath("logs/") {
		createdir(logpath);
		filenam = logpath + "LOG_" + logdate() + ".log";
	}


	filelog(string pathtolog) : datentime(""), logpath(pathtolog) {
		if (logpath[logpath.length() - 1] != '/' || logpath[logpath.length() - 1] != '\\') {
			logpath += '/';
		}
		createdir(logpath);
		filenam = logpath + "LOG_" + logdate() + ".log";


	}
	~filelog() {


	}
	//auto asyncbeep = std::async(std::launch::async, [] { Beep(1000, 5000); });


	int writetolog(string messg, int type = 0, string prog_module = "main") {//writes directly to log file
		//auto start = chrono::high_resolution_clock::now();
		//type 0 - info, 1-warn, 2-err, 3-fatal err
		//prog_module - module of the program that message is being sent from
		if (openfile()) {
			MessageBoxA(0, "FATAL ERROR:Could not open file for logging!\nThe program will now exit.", "FATAL ERROR:Filelog", MB_OK | MB_ICONERROR);
			exit(1);

		}
		
		string write;
		write = "[ " + currentDateTime() + " ] [";
		//vector<char> vchar;
		filestr.write(write.c_str(), write.length());
		switch (type)
		{


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
			write = "INFO";
			break;
		}
		write += "] [" + prog_module + "]: " + messg + "\n";
		filestr.write(write.c_str(), write.length());
		if (closefile()) {
			MessageBoxA(0, "WARN:Trying to close not opened file", "WARN:Filelog", MB_OK | MB_ICONWARNING);
		}
// 		auto end = chrono::high_resolution_clock::now();
// 		double time_taken =
// 			chrono::duration_cast<chrono::nanoseconds>(end - start).count();
// 
// 		time_taken *= 1e-9;
// 		cout << "Time taken by program is : " << fixed
// 			<< time_taken << setprecision(9);
// 		cout << " sec" << endl;
		
		return 0;

	}

	
	
	

private:
	
	//date/time
	const std::string currentDateTime() {
		time_t now = time(0);
		struct tm tstruct;
		char buff[80];
		//tstruct = *localtime(&now);
		localtime_s(&tstruct, &now);
		strftime(buff, sizeof(buff), "%Y-%m-%d.%X", &tstruct);

		return buff;
	}
	string datentime;//stores date/time data
	string logdate() {
		string temp = currentDateTime(); 
		string temp2;
		short i = 0;
		while (temp[i] != '.') {
			temp2 += temp[i];
			i++;
		}
		return temp2;
	}// date of the log file creation, so we have one log file per execution

	string filenam;

	fstream filestr;//file "editor"
	string logpath;//log path variable
	int createdir(string pathtofile) {//creates directories of the path if they dont exist
		std::filesystem::create_directories(pathtofile);
		return 0;
	}
	int openfile() {
		//a crutch to showing the data in file instantly, as fstream has some kind of buffer
		//so you need to close file after writing to it, so the data will update
		//cout << filenam << endl;
		filestr.open(filenam.c_str(), fstream::out | fstream::app);
		if (filestr.is_open()) {
			return 0;
		}
		return 1;
	}
	int closefile() {
		if (filestr.is_open()) {
			filestr.close();
			return 0;
		}
		return 1;
	}


};
filelog deflogger;//default logger





#endif

