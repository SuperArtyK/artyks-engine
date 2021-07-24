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
 ///converts degrees value x to radians
#define torad(x) x * 3.141592654f / 180.0f

namespace artyk::math {
	///contains mathematical functions
	//math stuff

	constexpr float PI = 3.141592654f;

	inline float sindeg(float degrees) {//calculated sine of the given degrees
		return sinf(torad(degrees));
	}
	inline float cosdeg(float degrees) {//calculated sine of the given degrees
		return cosf(torad(degrees));
	}
	inline float tandeg(float degrees) {//calculated sine of the given degrees
		return tanf(torad(degrees));
	}
	inline float cotdeg(float degrees) {//calculated sine of the given degrees
		return 1 / tanf(torad(degrees));
	}
	inline float cscdeg(float degrees) {//calculated sine of the given degrees
		return 1 / sinf(torad(degrees));
	}
	inline float secdeg(float degrees) {//calculated sine of the given degrees
		return 1 / cosf(torad(degrees));
	}
}