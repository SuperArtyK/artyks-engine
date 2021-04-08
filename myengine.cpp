// myengine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#pragma comment(lib, "winmm.lib")
#include "sound.hpp"
#include "screen.hpp"
#include "include.hpp"
#include "log.hpp"

using namespace std;

int main()
{
	
	Sound mysound;
	mysound.MakeSound(400, 10000);
	scrolltitle("Title", 500);
	for (int i = 0; i < 1000; i++) {

		cout << i << "\n";
		Sleep(50);
	}
	
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
