// myengine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment(lib, "winmm.lib")

#if not defined _DEBUG
#define _SECURE_SCL 0
#define _HAS_ITERATOR_DEBUGGING 0

#endif
//#include "sound.hpp"
#include "keyboard.hpp"
#include "screen.hpp"
#include "include.hpp"
#include "log.hpp"

using namespace std;

//var name cheatsheet
/*
<scope/type><access level>_<datatype><Name>
mpb_bDev
^ ^ ^ ^
| | | |
| | | Dev(Name
| | |
| | boolean(datatype
| |
| public(access level)
|
member var(type)

*/



int main()
{
// 	_getch();
// 	Sound mysound;
// 	
// 	setFont(10, 14);
// 	setScreenSize(12, 10);
// 	for (int i = 0; i < 1000; i++) {
// 
// 		cout << i << "\n";
// 		Sleep(50);
// 	}

// 	filelog mylog("logs1/");
// 	//mylog.m_bDev_cout = true;
// 	mylog.writetolog("log1 is working");
// 	mylog.writetolog("log2 is working");
// 	mylog.writetolog("log3 is working");
// 	mylog.writetolog("log4 is working");
// 	mylog.writetolog("log5 is working");
// 	mylog.writetolog("log6 is working");
// 	mylog.writetolog("log7 is working");
// 	mylog.writetolog("log8 is working");
// 	mylog.writetolog("log9 is working");
// 	mylog.writetolog("log10 is working");
// 	mylog.writetolog("log11 is working");
	//vector<vector<string>> tempn;

// 	for (int i = 0; i < 10000; i++) {
// 
// 		cout << i << endl;
// 		Sleep(100);
// 	}
	//mylog.mainthread();
	thread t2;
	t2 = thread(checkkeystates);
	for (;;) {


		cout << g_kstKeys[40].m_bHeld <<endl;
		if(g_kstKeys[26].m_bPressed) break;


	}
	stoptrd = true;
	t2.join();

	_getch();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
