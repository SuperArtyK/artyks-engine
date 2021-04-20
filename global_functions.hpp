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

//////////////////////////////////////////////////////////////////////////
// this file contains the global functions that can be used everywhere
// should not cause everything to redefine
//////////////////////////////////////////////////////////////////////////

#pragma once

#ifndef GLOB_FUNCS
#define GLOB_FUNCS
#include "include.hpp"
using namespace std;
//prototypes
//moved from log.cpp
inline int movetoend(std::any arr[], int n, int pos);
//inline int movetoend(int l_iArr[], int l_iN, int l_iPos); //not needed since I have std::any rollin
inline const char* BoolToString(bool b);//converts bool value to string 
inline bool inrange(long double minval, long double maxval, long double i);//checks if the number is in range
inline bool inrange(int minval, int maxval, int i);//checks if the number is in range
inline bool inrange(long long minval, long long maxval, long long i);//checks if the number is in range
inline int strtolower(string& str1);//lowers string to lowercase
inline wstring widen(const string& str);//converts string to wstring
inline string narrow(const wstring& str);//converts wstring to string(possible loss of data)
inline size_t findinstr(string& str, int pos = 0) {
	size_t found = str.find(str, pos);
	if (found != string::npos) { return found; }
	return -1;//if we didnt find return error flag

	
}


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

inline int strtolower(string& str1) {

	//std::transform(str1.begin(), str1.end(), str1.begin(), std::tolower);
	std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);

	return 0;
}
inline wstring widen(const string& str)
{
	wostringstream wstm;
	const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(wstm.getloc());
	for (size_t i = 0; i < str.size(); ++i)
		wstm << ctfacet.widen(str[i]);
	return wstm.str();
}

inline string narrow(const wstring& str)
{
	ostringstream stm;

	const ctype<wchar_t>& ctfacet = use_facet<ctype<wchar_t>>(stm.getloc());

	for (size_t i = 0; i < str.size(); ++i)
		stm << ctfacet.narrow(str[i], 0);
	return stm.str();
}

#endif // !GLOB_FUNCS