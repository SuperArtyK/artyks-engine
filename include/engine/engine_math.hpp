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

/** @file include/engine/engine_math.hpp
 *  This file contains the engine math functions, (probably)optimised for engine stuff.
 *  may be faster or equivalent to what can you find/do yourself.
 *
 *  Should not cause everything to break.
 */

#pragma once

#include <cmath>
#include <limits>
///converts degrees value x to radians
#define torad(x) x * 3.141592654f / 180.0f


///contains mathematical functions
namespace artyk::math {
	
	//math stuff
	///engine pi variable
	constexpr float PI = 3.141592654f;

	///calculates sine of the given degrees
	inline float sindeg(float degrees) {
		return sinf(torad(degrees));
	}

	///calculates cosine of the given degrees
	inline float cosdeg(float degrees) {
		return cosf(torad(degrees));
	}

	///calculates tangent of the given degrees
	inline float tandeg(float degrees) {
		return tanf(torad(degrees));
	}

	///calculates cotangent of the given degrees
	inline float cotdeg(float degrees) {
		return 1 / tanf(torad(degrees));
	}

	///calculates cosecant of the given degrees
	inline float cscdeg(float degrees) {
		return 1 / sinf(torad(degrees));
	}

	///calculates secant of the given degrees
	inline float secdeg(float degrees) {
		return 1 / cosf(torad(degrees));
	}

	///rounds the given number to int(assumed its less than INT_MAX)
	constexpr inline int roundtoint(const float num) {
		return (num<0)? int(num + 0.5f) *-1:int(num + 0.5f);
	}

	///rounds the given number to bigint(assumed its less than BINT_MAX)
	constexpr inline long long roundtobigint(const float num) {
		return (num < 0) ? (long long)(num + 0.5f) * -1 : (long long)(num + 0.5f);
	}

	///returns absolute value of given float
	constexpr inline float absval(const float num) {
		return (num < 0) ? num -1 : num;
	}

	///checks if given 2 floats are equal, using epsilon(must not go further than float epsilon)
	constexpr inline bool fequals(const float num, const float num2, const float _epsilon = std::numeric_limits<float>::epsilon()) {
		return (num - num2) < _epsilon &&
			   -(num - num2) < _epsilon;
	}

	///another sqrt implementation, useful when you need calculate sqrt for constexpr
	constexpr float fsqrt(const float num)
	{
		float val[2] = { num,0 };
		while (!fequals(val[0], val[1])) {
			val[1] = val[0];
			val[0] = 0.5f * (val[0] + num / val[0]);
		}
		return val[1];
	}

}