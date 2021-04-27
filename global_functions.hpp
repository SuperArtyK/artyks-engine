/*
	ArtyK's Console (Game) Engine. Console engine for apps and games
	Copyright (C) 2021  Artemii Kozhemiak

	https://github.com/SuperArtyK/myengine

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

//////////////////////////////////////////////////////////////////////////
// this file contains the global functions that can be used everywhere
// should not cause everything to redefine
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef GLOB_FUNCS
#define GLOB_FUNCS
#include "include.hpp"
#include "global_vars.hpp"
//prototypes
//moved from log.cpp
using namespace std;




//im making this, so I can differentiate between std, mine, and others
namespace artyk {

	//prototypes
	inline int movetoend(std::any arr[], int n, int pos);

	//inline int movetoend(int l_iArr[], int l_iN, int l_iPos); //not needed since I have std::any rollin

	inline const char* BoolToString(bool b);//converts bool value to string 

	inline bool inrange(long double minval, long double maxval, long double i);//checks if the number is in range

	inline bool inrange(int minval, int maxval, int i);//checks if the number is in range

	inline bool inrange(long long minval, long long maxval, long long i);//checks if the number is in range

	inline int tolower_ref(std::string& str1);//lowers string to lowercase, passes by reference, returns status
	inline std::string tolower_val(std::string str1);//lowers string to lowercase, passes by value, returns lowered string

	inline bool isnum(const std::string& s);//checks if std::string is a number, thanks stackoverflow

	inline void Sleep_SPIN(double seconds);//has Spin-lock, which completely obliterates cpu, if runs on mult threads
	//but is very precise
	//(not on linux though, usleep still wins)





	//definitions
	inline bool isnum(const std::string& s)
	{
		std::string::const_iterator it = s.begin();
		while (it != s.end() && std::isdigit(*it)) ++it;
		return !s.empty() && it == s.end();
	}
	inline BOOLEAN nanosleep(LONGLONG ns) {//best sleep function I found. 
		/* Declarations */
		HANDLE timer;   /* Timer handle */
		LARGE_INTEGER li;   /* Time defintion */
		/* Create timer */
		if (!(timer = CreateWaitableTimer(NULL, TRUE, NULL)))
			return FALSE;
		/* Set timer properties */
		li.QuadPart = -ns;
		if (!SetWaitableTimer(timer, &li, 0, NULL, NULL, FALSE)) {
			CloseHandle(timer);
			return FALSE;
		}
		/* Start & wait for timer */
		WaitForSingleObject(timer, INFINITE);
		/* Clean resources */
		CloseHandle(timer);
		/* Slept without problems */
		return TRUE;
	}

	inline void Sleep_SPIN(double seconds) {//has Spin-lock, which completely obliterates cpu performance, if runs on mult threads
		//but is very precise
		// //(not on linux though, usleep still wins)
		//old version of artyk::Sleep()
		using namespace std::chrono;

		static double estimate = 5e-3;
		static double mean = 5e-3;
		static double m2 = 0;
		static int64_t count = 1;


		//normal sleep
		while (seconds > estimate) {
			auto start = std::chrono::high_resolution_clock::now();
			this_thread::sleep_for(std::chrono::milliseconds(1));
			auto end = std::chrono::high_resolution_clock::now();

			double observed = (end - start).count() / 1e9;
			seconds -= observed;

			count++;
			double delta = observed - mean;
			mean += delta / count;
			m2 += delta * (observed - mean);
			double stddev = sqrt(m2 / (count - 1));
			estimate = mean + stddev;
		}

		// spin locking
		auto start = std::chrono::high_resolution_clock::now();
		while ((std::chrono::high_resolution_clock::now() - start).count() / 1e9 < seconds);
	}

	inline std::wstring widen(const  std::string& str);//converts string to wstring

	inline std::string narrow(const std::wstring& str);//converts wstring to string(possible loss of data)

	inline long long int findinstr_pos(std::string str, std::string text, int pos = 0);//finds and returns the position of the text in string + 1

	inline std::string findinstr_text(std::string str, std::string text, int pos = 0);//finds text in str we pass, if successful, return text

	//definitions
	inline int movetoend(std::any arr[], int n, int pos) {
		std::any save = arr[pos];

		for (int i = pos; i < n - 1; i++)
			arr[i] = arr[i + 1];

		arr[n - 1] = save;
		return 0;
	}

	inline const char* BoolToString(bool b)
	{
		return b ? "OK!" : "ERROR!";
	}

	inline bool inrange(long double minval, long double maxval, long double i) {
		return minval <= i && i < maxval;
	}
	inline bool inrange(int minval, int maxval, int i) {
		return minval <= i && i < maxval;
	}
	inline bool inrange(long long minval, long long maxval, long long i) {
		return minval <= i && i < maxval;
	}

	inline int tolower_ref(std::string& str1) {

		//std::transform(str1.begin(), str1.end(), str1.begin(), std::tolower);
		std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);

		return 0;
	}
	inline std::string tolower_val(std::string str1) {

		//std::transform(str1.begin(), str1.end(), str1.begin(), std::tolower);
		std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);

		return str1;
	}


	inline wstring widen(const string& str)
	{
		std::wostringstream wstm;
		const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
		for (size_t i = 0; i < str.size(); ++i)
			wstm << ctfacet.widen(str[i]);
		return wstm.str();
	}

	inline string narrow(const wstring& str)
	{
		std::ostringstream stm;

		const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(stm.getloc());

		for (size_t i = 0; i < str.size(); ++i)
			stm << ctfacet.narrow(str[i], 0);
		return stm.str();
	}



	inline long long int findinstr_pos(std::string str, std::string text, int pos) {//finds and returns the position of the text in string + 1
		size_t found = str.find(text, pos);
		if (found != string::npos) { return found + 1;/*we get a pos we found it in relation to length*/ }
		return false;//if we didnt find, return error flag


	}

	inline std::string findinstr_text(std::string str, std::string text, int pos) {//finds text in str we pass, if successful, return text
		size_t found = str.find(text, pos);
		if (found != string::npos) { return text;/*we found it, and we're returning text we were searching for*/ }
		return "";//if we didnt find, return error flag


	}

}




#endif // !GLOB_FUNCS