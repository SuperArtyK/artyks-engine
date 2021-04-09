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






//#include "include.hpp"//includes EVERYTHING that I need
#include <iostream>
#include <thread>
#include <fstream>
//using namespace std;

//const int maxtries = 10000;

class filelog {

public:
	bool dev;//for debug information
	filelog();

	filelog(std::string pathtolog);
	//int changelogpath(std::string newlogpath);
	//filelog& operator=(std::string differentpath);

	~filelog() {
		if(threadstarted)
		stoplogging();

	}
	//auto asyncbeep = std::async(std::launch::async, [] { Beep(1000, 5000); });


// 	int writetolog(std::string messg, int type = 0, std::string prog_module = "main") {//writes directly to log file
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

	int writetolog(std::string messg, int type = 0, std::string prog_module = "main");
	int stoplogging();
	int startlogging();
	
	
private:
	//functions
	bool stoplog;//flag that for stopping/opening logging session
	int mainthread();
	int movetoendSTR(std::string arr[], int n, int pos);
	int movetoendINT(int arr[], int n, int pos);

	//date/time
	const std::string currentDateTime();

	std::string logdate();


	int createdir(std::string pathtofile);
	int openfile();
	int closefile();


	//vars
	std::string datentime;//stores date/time data
	std::string filenam;
	std::fstream filestr;//file "editor"
	std::string logpath;//log path variable
	bool threadstarted;
	std::thread logthread;
	

	//logging vars
	const int maxmessgcount = 100;
	std::string messg[100];
	int messgcount;
	int type[100];
	std::string prog_module[100];


	


};
inline  filelog deflogger;//default logger





#endif

