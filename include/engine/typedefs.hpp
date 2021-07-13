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

/** @file include/engine/typedefs.hpp
 *  This file contains typedefs/aliases of the variable types that are long and
 *  are used often/inconvenient to use in other naming.
 *  
 *  Should not cause everything to break.
 */

#pragma once

#ifndef TYPEDEFS_HPP
#define TYPEDEFS_HPP

#include <limits>

//a few typedefs so I'll type less code
///short for small unsigned int
typedef unsigned char smalluint;
///small signed int
typedef char smallint;
///big unsigned int
typedef unsigned long long biguint;
///big signed int
typedef long long bigint;
///just unsigned int
typedef unsigned long uint;

//max/min values of these defines

///small unsigned int max
#define SUINT_MAX UCHAR_MAX
///small unsigned int min
#define SUINT_MIN 0

///small signed int max
#define SINT_MAX SCHAR_MAX
///small signed int min
#define SINT_MIN SCHAR_MIN

///big unsigned int max
#define BUINT_MAX ULLONG_MAX
///big unsigned int min
#define BUINT_MIN 0

///big signed int max
#define BINT_MAX LLONG_MAX
///big signed int min
#define BINT_MIN LLONG_MIN

/// no UINT_MAX, as it's already defined\
///unsigned int min
#define UINT_MIN 0

#endif // !TYPEDEFS_HPP




