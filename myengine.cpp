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

void scolor(uint_fast8_t r, uint_fast8_t g, uint_fast8_t b) {
	CONSOLE_SCREEN_BUFFER_INFOEX info;
	info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfoEx(hConsole, &info);

	info.ColorTable[0] = RGB(255, 100, 0);
	
		info.ColorTable[3] = RGB(135, 206, 235);
	
		info.ColorTable[15] = RGB(25, 0, 255);

	SetConsoleScreenBufferInfoEx(hConsole, &info);
	SetConsoleTextAttribute(hConsole, 15);
	std::cout << "Hello World!" << std::endl;
}


int main()
{
// 	_getch();
// 	Keyboard mykb;
// 	vector<sKeyState> temp;
// 	for(;;) {
// 
// 		temp = mykb.GetPressedKeys();
// 		if (temp.size() < 1) {
// 			cout << "No keys pressed\n";
// 			 
// 		}
// 		else
// 		{
// 			cout << "keys pressed: ";
// 			for (int a = 0; a < temp.size(); a++) {
// 
// 				cout << temp[a].name << ", ";
// 
// 			}
// 			cout << "\n";
// 		}
// 
// 
// 	}

	
// 	
// 	while (true) {
// 		start = chrono::high_resolution_clock::now();
// 
// 		ab++;
// 		//SetPixelV(consoleDC, i, i2, RGB(rand() % 255, rand() % 255, rand() % 255));
// 		SetPixel(consoleDC, i, i2, RGB(rand() % 255, rand() % 255, rand() % 255));
// 		end = chrono::high_resolution_clock::now();
// 		time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count() * 1e-9;
// 		myscr.settitle(to_string(time_taken));
// 	}
	
	_getch();
}

// Run program: Ctrl + F5 or Debug >s Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
