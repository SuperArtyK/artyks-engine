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
 *  This file contains engine function (hopefully useful)utilities.
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef GLOB_FUNCS_HPP
#define GLOB_FUNCS_HPP
#include "global_vars.hpp"
#include "trig_lookup.hpp"

 //prototypes
 //moved from log.cpp
using std::string;
using std::wstring;
using std::to_string;


#define arrsize(x) (sizeof(x)/sizeof(x[0]))
#define mod0(x,y) !(x%y)




///This namespace contains all of things that any of the part of program can use, and is useful.
///Such things are -- handles to windows, app names, status(we're starter or closing), flags, functions, etc.
namespace artyk {
	///global useful functions
	namespace utils {

//////////////////////////////////////////////////////////////////////////
// prototypes
//////////////////////////////////////////////////////////////////////////


		inline std::string btostr(bool b);//converts bool value to std::string

		/// <summary>
		/// checks if the value you pass is in range
		/// </summary>
		/// <typeparam name="T">any data type</typeparam>
		/// <param name="minval">minimum value of range</param>
		/// <param name="maxval">maximum value of range</param>
		/// <param name="i">the value to check</param>
		/// <returns>true if in range, false otherwise</returns>
		template<typename T>
		inline bool inrange(T minval, T maxval, T i);//checks if the number is in range

		///lowers the and writes to the string you pass
		inline void tolower_ref(std::string& str1);//lowers string to lowercase, passes by reference, returns status

		///lowers the string you pass and returns lowered string
		inline std::string tolower_val(std::string str1);//lowers string to lowercase, passes by value, returns lowered string

		///checks if the string is a number. ture if it is, false otherwise
		inline bool isnum(const std::string& s);

		///returns string of current date and time in YYYY-MM-DD.HH:mm:SS format
		inline const std::string currentDateTime();

		/// <summary>
		/// gets numeric value from cin, and repeats untill the value is numeric
		/// </summary>
		/// <typeparam name="T">type of the variable</typeparam>
		/// <param name="variable">the variable to get input to</param>
		template<typename T>
		inline void getCinNum(T& variable);

		///fixes cin stream
		inline void fixcin(void);

		/// converts/wides normal string to wide string
		inline std::wstring widen_str(const std::string& str);//converts string to wstring

		///converts/narrows wide string to normal string, possible loss of data
		inline std::string narrow_wstr(const std::wstring& str);//converts wstring to string(possible loss of data)

		//finds text in string we pass. if successful return index of first letter of text+1
		inline long long findinstr(const std::string& str, const std::string& text, long long pos = 0);

		///adds memory address value to string
		inline std::string addrtostr(void* myptr);//adds address value to string

		///scans and returns if the app is in focus
		inline bool isinfocus(void); //scans if the app is in focus

		///pseudorandom number generator based on XOR algorithm
		inline unsigned long rand_xor(void);

		///returns color attribute of the cell from given backround and foreground
		constexpr inline smalluint getattrib(smalluint bgr, smalluint fgr);


#ifdef AE_EXPERIMENTAL
		///counts time between calls, not safe/consistent if used in other threads
		inline float timecounter(void);
#endif

//////////////////////////////////////////////////////////////////////////
// defines
//////////////////////////////////////////////////////////////////////////


		inline std::string btostr(bool b)
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

		inline const std::string currentDateTime(void) {//returns current date in YYYY-MM-DD.HH:mm:SS format
			time_t now = time(0);
			struct tm tstruct;
			char buff[80];
			//tstruct = *localtime(&now);
			localtime_s(&tstruct, &now);
			strftime(buff, sizeof(buff), "%Y-%m-%d.%X", &tstruct);

			return buff;
		}

		template<typename T>
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
			if (found != string::npos) { return found + 1;/*we get a pos we found it in relation to length*/ }
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
			static systime timestart = getsystime;
			static systime timeend = getsystime;

			timeend = std::chrono::system_clock::now();
			//std::chrono::duration<float> elapsedTime = timeend - timestart;
			//timestart = timeend;
			float fElapsedTime = std::chrono::duration<float>(timeend - timestart).count();
			timestart = timeend;
			return fElapsedTime;
		}
#endif

		inline unsigned long rand_xor(void) {
			static unsigned long x = std::rand(), y = std::rand(), z = std::rand();
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

		constexpr inline smalluint getattrib(smalluint bgr = color::DEF_BGR, smalluint fgr = color::DEF_FGR) {
			return bgr * 16 + fgr;
		}
	}

}

#endif // !GLOB_FUNCS_HPP