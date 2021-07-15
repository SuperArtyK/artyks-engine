/*
	ArtyK's Console (Game) Engine. Console engine for apps and games
	Copyright (C) 2021  Artemii Kozhemiak

	https://github.com/SuperArtyK/artyks-engine

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License 3.0 as published
	by the Free Software Foundation.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License 3.0 for more details.

	You should have received a copy of the GNU General Public License 3.0
	along with this program.  If not, see <https://www.gnu.org/licenses/gpl-3.0.txt>.
*/

/** @file include/engine/func_utils.hpp
 *  This file contains the global functions that can be used everywhere.
 *  
 *  Should not cause everything to break.
 */

#pragma once

#ifndef GLOB_FUNCS_HPP
#define GLOB_FUNCS_HPP
#include <algorithm>
#include <iostream>
#include <chrono>
#include <sstream>
#include "global_vars.hpp"
#include "trig_lookup.hpp"

//prototypes
//moved from log.cpp
using std::string;
using std::wstring;
using std::to_string;

///This namespace contains all of things that any of the part of program can use, and is useful.
///Such things are -- handles to windows, app names, status(we're starter or closing), flags, functions, etc.
namespace artyk {
	

	///global useful functions
	namespace utils {
		//prototypes
		inline std::string BoolToString(bool b);//converts bool value to cstring 


		template<typename T>
		/// <summary>
		/// checks if the value you pass is in range
		/// </summary>
		/// <typeparam name="T">any data type</typeparam>
		/// <param name="minval">minimum value of range</param>
		/// <param name="maxval">maximum value of range</param>
		/// <param name="i">the value to check</param>
		/// <returns>true if in range, false otherwise</returns>
		inline bool inrange(T minval, T maxval, T i);//checks if the number is in range

		///lowers the and writes to the string you pass
		inline void tolower_ref(std::string& str1);//lowers string to lowercase, passes by reference, returns status

		///lowers the string you pass and returns lowered string
		inline std::string tolower_val(std::string str1);//lowers string to lowercase, passes by value, returns lowered string

		///checks if the string is a number. ture if it is, false otherwise
		inline bool isnum(const std::string& s);

		//returns string of current date and time in YYYY-MM-DD.HH:mm:SS format
		inline const std::string currentDateTime();

		template<typename T>
		inline void getCinNum(T& variable);

		///fixes cin stream
		inline void fixcin();

		/// converts/wides normal string to wide string
		inline std::wstring widen_str(const std::string& str);//converts string to wstring

		///converts/narrows wide string to normal string, possible loss of data
		inline std::string narrow_wstr(const std::wstring& str);//converts wstring to string(possible loss of data)

		//finds text in string we pass. if successful return index of first letter of text+1
		inline long long findinstr(const std::string& str, const std::string& text, long long pos = 0);

		///adds memory address value to string
		inline std::string addrtostr(void* myptr);//adds address value to string

		///scans and returns if the app is in focus
		inline bool isinfocus(); //scans if the app is in focus

		
		

		inline unsigned long xorshf96(void) {          //period 2^96-1
			static unsigned long x = rand(), y = rand(), z = rand();
			unsigned long t;
			x ^= x << 16;
			x ^= x >> 5;
			x ^= x << 1;

			t = x;
			x = y;
			y = z;
			z = t ^ x ^ y;

			return z;
	}


#ifdef AE_EXPERIMENTAL
		///counts time between calls, not safe/consistent if used in other threads
		inline float timecounter();
#endif


		inline std::string BoolToString(bool b)
		{
			return b ? "true" : "false";
		}

		template<typename T>
		inline bool inrange(T minval, T maxval, T i)
		{
			return (minval <= i) && (i <= maxval);
		}

		inline void tolower_ref(std::string& str1) {
			std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
		}

		inline std::string tolower_val(std::string str1) {
			std::transform(str1.begin(), str1.end(), str1.begin(), tolower);
			return str1;
		}

		inline bool isnum(const std::string& s)
		{
			for (char c : s) {
				if (!isdigit(c)) {
					return false;
				}
			}
			return true;
		}

		inline const std::string currentDateTime() {//returns current date in YYYY-MM-DD.HH:mm:SS format
			time_t now = time(0);
			struct tm tstruct;
			char buff[80];
			//tstruct = *localtime(&now);
			localtime_s(&tstruct, &now);
			strftime(buff, sizeof(buff), "%Y-%m-%d.%X", &tstruct);

			return buff;
		}

		template<typename T>
		/// <summary>
		/// gets numeric value from cin, and repeats untill the value is numeric
		/// </summary>
		/// <typeparam name="T">type of the variable</typeparam>
		/// <param name="variable">the variable to get input to</param>
		inline void getCinNum(T& variable) {
#undef max
			//used in some of my old projects
			std::cin >> variable;
			while (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Bad entry.  Enter a NUMBER: ";
				std::cin >> variable;
			}
		}

		inline void fixcin() {
			while (std::cin.fail())
			{
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			}
		}


		inline wstring widen_str(const string& str)
		{
			std::wostringstream wstm;
			const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(wstm.getloc());
			for (size_t i = 0; i < str.size(); ++i)
				wstm << ctfacet.widen(str[i]);
			return wstm.str();
		}

		inline string narrow_wstr(const wstring& str)
		{
			std::ostringstream stm;

			const std::ctype<wchar_t>& ctfacet = std::use_facet<std::ctype<wchar_t>>(stm.getloc());

			for (size_t i = 0; i < str.size(); ++i)
				stm << ctfacet.narrow(str[i], 0);
			return stm.str();
		}



		inline long long findinstr(const std::string& str, const std::string& text, long long pos) {//finds text in string we pass. if successful return index of first letter of text+1
			long long int found = str.find(text, pos);
			if (found != string::npos) { return found+1;/*we get a pos we found it in relation to length*/ }
			return 0;//if we didnt find, return false


		}



		
		
		
		inline std::string addrtostr(void* myptr) {//adds address value to string
			std::ostringstream oss;
			oss << myptr;
			return oss.str();
		}

		inline bool isinfocus() //scans if the app is in focus
		{
			return artyk::g_console_hwnd == GetForegroundWindow();
		}

		


#ifdef AE_EXPERIMENTAL
		inline float timecounter() {

			static auto timestart = std::chrono::system_clock::now();
			static auto timeend = std::chrono::system_clock::now();


			timeend = std::chrono::system_clock::now();
			//std::chrono::duration<float> elapsedTime = timeend - timestart;
			//timestart = timeend;
			float fElapsedTime = std::chrono::duration<float>(timeend - timestart).count();
			timestart = timeend;
			return fElapsedTime;

		}
#endif


	}

	///contains mathematical functions
	namespace math {
		//math stuff

		inline constexpr float PI = 3.141592654f;

		inline float sindeg(float degrees) {//calculated sine of the given degrees
			return sin(degrees * PI / 180.0f);
		}
		inline float cosdeg(float degrees) {//calculated sine of the given degrees
			return cos(degrees * PI / 180.0f);
		}
		inline float tandeg(float degrees) {//calculated sine of the given degrees
			return tan(degrees * PI / 180.0f);
		}
		inline float cotdeg(float degrees) {//calculated sine of the given degrees
			return 1 / tan(degrees * PI / 180.0f);
		}
		inline float cscdeg(float degrees) {//calculated sine of the given degrees
			return 1 / sin(degrees * PI / 180.0f);
		}
		inline float secdeg(float degrees) {//calculated sine of the given degrees
			return 1 / cos(degrees * PI / 180.0f);
		}
	}

	/// <summary>
	/// This function initialises all stuff the engine needs, before it starts. 
	/// Call it at the start up, or somewhere when you need, but you can do it only once.
	/// </summary>
	/// <param name="high_priority">flag to make main process high-priority</param>
	/// <param name="noscrollbar">flag to disable console scrollbar showing</param>
	/// <param name="noresize">flag to disable window resize ability</param>
	/// <returns></returns>
	inline void init_main(bool high_priority = true, bool noscrollbar = false, bool noresize = false) {
		if (artyk::app_startstatus != SHRT_MAX) {
			srand(time(NULL));
			SetConsoleActiveScreenBuffer(artyk::g_output_handle);
			SetConsoleCP(CP_UTF8);
			SetConsoleOutputCP(CP_UTF8);
			std::locale loc("en_US.UTF8");
			std::cout.imbue(loc);
			std::locale::global(loc);
			std::setlocale(LC_ALL, "en_US.UTF8");
			if (high_priority)
				SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
			if (noscrollbar)
				ShowScrollBar(GetConsoleWindow(), SB_VERT, 0);
			if (noresize)
				SetWindowLong(GetConsoleWindow(), GWL_STYLE, GetWindowLong(GetConsoleWindow(), GWL_STYLE) & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX);


			artyk::app_startstatus = SHRT_MAX;
			//done intitializing
		}
	}



}





#endif // !GLOB_FUNCS_HPP

