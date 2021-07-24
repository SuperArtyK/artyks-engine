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

/** @file include/engine/custom_types.hpp
 *  This file contains the code for custom datatypes(vectors, keystates, etc)
 *
 *  Should not cause everything to break.
 */

#pragma once

#include "typedefs.hpp"

 ///2d integer vector type
struct vec2int
{
	int x;
	int y;
	constexpr static inline vec2int zero() {
		return { 0,0 };
	}
};
typedef vec2int vector2_int;

///2d (float) vector type
struct vec2
{
	float x;
	float y;
	constexpr static inline vec2 zero() {
		return { 0.0f,0.0f };
	}
};
typedef vec2 vector2;

struct vec3int
{
	int x;
	int y;
	int z;
	constexpr static inline vec3int zero() {
		return { 0,0,0 };
	}
};
typedef vec3int vector3_int;

struct vec3
{
	float x;
	float y;
	float z;
	constexpr static inline vec3 zero() {
		return { 0.0f, 0.0f, 0.0f };
	}
};
typedef vec3 vector3;

///data type to store keys and their data
struct keystate
{
	///name of the key
	const char* m_name;
	///stores the key id
	smalluint m_keyid;
	///the GetAsyncKeyState() key state
	short m_state;
	///flag if key is pressed
	bool m_isPressed;
	///flag if key is released
	bool m_isReleased;
	///flag if key is held
	bool m_isHeld;
	///flag if key is used at all
	bool m_isUsed;
};
typedef keystate AEKBKey;