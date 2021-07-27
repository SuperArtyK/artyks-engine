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

#ifndef CUSTOM_TYPES_HPP
#define CUSTOM_TYPES_HPP

#include <Windows.h>


///2d integer vector type
struct vec2int
{

	int x;
	int y;
	constexpr static inline vec2int zero() {
		return { 0,0 };
	}

};


///2d (float) vector type
struct vec2
{
	float x;
	float y;
	constexpr static inline vec2 zero() {
		return { 0.0f,0.0f };
	}
};

///3d integer vector type
struct vec3int
{
	int x;
	int y;
	int z;
	constexpr static inline vec3int zero() {
		return { 0,0,0 };
	}
};

///3d (float) vector type
struct vec3
{
	float x;
	float y;
	float z;
	constexpr static inline vec3 zero() {
		return { 0.0f, 0.0f, 0.0f };
	}
};

///converts given vec2 to vec2int(narrowing conversion)
inline vec2int tovec2int(const vec2& val) {
	return { (int)val.x, (int)val.y };
}

///converts given vec2int to vec2(narrowing conversion as well, due to float's mantissa)
inline vec2 tovec2(const vec2int& val) {
	return  { (float)val.x, (float)val.y };
}

///converts given vec3 to vec3int(narrowing conversion)
inline vec3int tovec3int(const vec3& val) {
	return { (int)val.x, (int)val.y, (int)val.z };
}

///converts given vec3int to vec3(narrowing conversion as well, due to float's mantissa)
inline vec3 tovec3(const vec3int& val) {
	return  { (float)val.x, (float)val.y, (float)val.z };
}

///converts given vec3 to vec2(z value is discared)
inline vec2 vec3tovec2(const vec3& val) {
	return { val.x,val.y };
}

///converts given vec2 to vec3(z value is zeroed)
inline vec3 vec2tovec3(const vec2& val) {
	return { val.x,val.y,0.0f };
}

///converts given vec3int to vec2int(z value is discared)
inline vec2int vec3tovec2_int(const vec3int& val) {
	return { val.x,val.y };
}

///converts given vec2int to vec3int(z value is zeroed)
inline vec3int vec2tovec3_int(const vec2int& val) {
	return { val.x,val.y,0 };
}

///data type to store keys and their data/state
struct keystate
{
	///name of the key
	const char* m_name;
	///the GetAsyncKeyState() key state
	short m_state;
	///stores the key id
	unsigned char m_keyid;
	///flag if key is pressed
	bool m_isPressed;
	///flag if key is released
	bool m_isReleased;
	///flag if key is held
	bool m_isHeld;
	///flag if key is used at all
	bool m_isUsed;
};


#endif // !CUSTOM_TYPES_HPP


