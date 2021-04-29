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
#include "oldsound.hpp"
#include "timer.hpp"
#include "keyboard.hpp"
#include "screen.hpp"
#include "include.hpp"
#include "log.hpp"


using namespace std;



long double tmp = 0, tmp2 = 0;




struct conscoord {
	unsigned short x = 0;
	unsigned short y = 0;
};
int main()
{

	filelog mylog;
	srand(time(NULL));
	Keyboard mykb;
	Screen myscr;
// 	filelog mylog;
// 	for (int i = 0; i < 1000; i++) {
// 		mylog.writetolog("");
// 		cout <<i<< "done\n";
// 	}
	//vector<sKeyState> vec1;
	//int i = 0;
	//frame_rater<10> fr;
	auto start = chrono::high_resolution_clock::now();
	auto end = chrono::high_resolution_clock::now();
	double time_taken =
		chrono::duration_cast<chrono::nanoseconds>(end - start).count();

// 	time_taken *= 1e-9;
// 	cout << "Time taken by program is : " << fixed
// 		<< time_taken << setprecision(9);
// 	cout << " sec" << endl;
	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {
		
		mylog.writetolog(to_string(i));

		
		i++;
	}
	end = chrono::high_resolution_clock::now();
	time_taken =
		chrono::duration_cast<chrono::nanoseconds>(end - start).count();

	time_taken *= 1e-9;
	cout << "Time taken by program is : " << fixed
		<< time_taken << setprecision(9);
	cout << " sec" << endl;

	start = chrono::high_resolution_clock::now();
	for (int i = 0; i < 10000; i++) {

		mylog.w2logtest(to_string(i));


		i++;
	}
	end = chrono::high_resolution_clock::now();
	time_taken =
		chrono::duration_cast<chrono::nanoseconds>(end - start).count();

	time_taken *= 1e-9;
	cout << "Time taken by program is : " << fixed
		<< time_taken << setprecision(9);
	cout << " sec" << endl;

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
