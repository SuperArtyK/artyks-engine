#pragma once

//////////////////////////////////////////////////////////////////////////
// this file contains the global functions that can be used everywhere
// should not cause everything to redefine
//////////////////////////////////////////////////////////////////////////

#ifndef GLOB_FUNCS
#define GLOB_FUNCS
#include "include.hpp"

//prototypes
//moved from log.cpp
inline int movetoend(std::any arr[], int n, int pos);
//inline int movetoend(int l_iArr[], int l_iN, int l_iPos); //not needed since I have std::any rollin
inline const char* BoolToString(bool b);
//in range
inline bool inrange(long double minval, long double maxval, long double i);
inline bool inrange(int minval, int maxval, int i);
inline bool inrange(long long minval, long long maxval, long long i);
inline int strtolower(string& str1);


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

	std::transform(str1.begin(), str1.end(), str1.begin(), std::tolower);

	return 0;
}


#endif // !GLOB_FUNCS