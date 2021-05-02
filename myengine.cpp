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

//modules
#include "oldsound.hpp"//ALWAYS! include it first
#include "log.hpp"
#include "keyboard.hpp"
#include "screen.hpp"

#include "timer.hpp"

//global stuff
#include "include.hpp"
#include "global_vars.hpp"
#include "global_functions.hpp"




using namespace std;



long double tmp = 0, tmp2 = 0;

class Logarray {
public:
	Logarray() {
		m_strArr = nullptr;
		m_ullLength = 0;
		m_ullCapacity = 0;
	}
	Logarray(uint64_t initlength) {
		m_strArr = new std::string[initlength];
		m_ullLength = 0;
		m_ullCapacity = initlength;// we don't store any value in strings, so length is 0, but we allocated them
		m_allocmem = true;
	}

	int pushback(std::string pushstr) {

		string* tempptr = nullptr;
		if (m_ullLength+1 >= m_ullCapacity) {
			//we need to reallocate and copy
			tempptr = new string[m_ullCapacity*8];
			for (uint64_t i = 0; i < m_ullLength;i++) {
				tempptr[i].append(m_strArr[i]);
			}

		}

	}


	~Logarray() {
		if (m_allocmem) {
			delete[] m_strArr;
		}
	}




private:
	
	short step;//the steps we are taking when allocating
	bool m_allocmem; //if we allocated the memory
	std::string* m_strArr;
	uint64_t m_ullLength, m_ullCapacity;
	std::string temp;



};

int main()
{

	Filelog mylog;
	srand(time(NULL));
	Keyboard mykb;
	Screen myscr;
	//myscr.setFont(2, 2, true, "Consolas");
	myscr.setScreenSize(1280, 720);
	//filelog mylog;
	artyk::timecounter();
	string temp;

	for (int i = 0;i<512; i++) {
		temp.push_back('a');
	}

// 	for (int i = 0;; i++) {
// 		mylog.writetolog("");
// 		cout <<i<< " done\n";
// 
// 	}
	double res = artyk::timecounter();
	cout << res;
	//vector<sKeyState> vec1;
	//int i = 0;
	
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
