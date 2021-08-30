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

/** @file include/engine/vector.hpp
 *  This file contains the engine vector's defitions and declarations(math vector, not std::vector)
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <cmath>
#include <limits>

 ///2d integer vector type
struct vec2int
{

	int x;
	int y;

	///returns magnitude of a vector
	inline float magnitude() const {
		return sqrtf(x * x + y * y);
	}

	///returns zeroed vector
	constexpr static inline vec2int zero() {
		return { 0,0 };
	}

};


///2d (float) vector type
struct vec2
{
	float x;
	float y;

	///returns magnitude of a vector
	inline float magnitude() const {
		return sqrtf(x * x + y * y);
	}

	///returns zeroed vector
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

	///returns magnitude of a vector
	inline float magnitude() const {
		return sqrtf(x * x + y * y + z * z);
	}

	///returns zeroed vector
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


	///returns magnitude of a vector
	inline float magnitude() const {
		return sqrtf(x * x + y * y + z * z);
	}

	///returns zeroed vector
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

/// <summary>
/// compares 2 vectors and returns bool as result
/// </summary>
/// @note the compiler will return an error if you will pass anything else than vectors in first 2 arguments
/// <typeparam name="A">vector type of first vector</typeparam>
/// <typeparam name="B">vector type of second vector</typeparam>
/// <param name="_vec1">vector 1</param>
/// <param name="_vec2">vector 2</param>
/// <param name="comptype">compare type: 0 </param>
/// <param name="_epsilon">epsilon to compare against, should not be bigger than float epsilon( std::numeric_limits<float>::epsilon() )</param>
/// <returns></returns>
template<typename A, typename B>
inline bool comparevec(const A& _vec1, const B& _vec2, const int comptype, const float _epsilon = std::numeric_limits<float>::epsilon()) {
	static_assert(
		(std::is_same<A, vec2>::value || std::is_same<A, vec2int>::value) ||
		(std::is_same<B, vec2>::value || std::is_same<B, vec2int>::value) ||
		(std::is_same<A, vec3>::value || std::is_same<A, vec3int>::value) ||
		(std::is_same<B, vec3>::value || std::is_same<B, vec3int>::value)
		, "[engine]: comparevec: You cannot use non-vector here.");


	const float diff1 = _vec1.x - _vec2.x;
	const float diff2 = _vec1.y - _vec2.y;
	float diff3;
	if constexpr ((std::is_same<A, vec3>::value || std::is_same<A, vec3int>::value) && (std::is_same<B, vec3>::value || std::is_same<B, vec3int>::value)) {
		diff3 = _vec1.z - _vec2.z;
	}
	else {
		diff3 = diff2;
	}
	switch (comptype)
	{
		//equality
	case 0:
		return (diff1 < _epsilon && -diff1 < _epsilon) && (diff2 < _epsilon && -diff2 < _epsilon) && (diff3 < _epsilon && -diff3 < _epsilon);
		break;

		//not-equality
	case 1:
		return (diff1 > _epsilon || -diff1 > _epsilon) || (diff2 > _epsilon || -diff2 > _epsilon) || (diff3 > _epsilon || -diff3 > _epsilon);
		break;

		//less than
	case 2:
		return (diff1 < -_epsilon) || (diff2 < -_epsilon) || (diff3 < -_epsilon);
		break;

		//more than
	case 3:
		return (diff1 > _epsilon) || (diff2 > _epsilon) || (diff3 > _epsilon);
		break;

		//less than or equals to
	case 4:
		return (diff1 <= _epsilon) && (diff2 <= _epsilon) && (diff3 <= _epsilon);
		break;

		//more than
	case 5:
		return (diff1 >= 0) && (diff2 >= 0) && (diff3 >= 0);
		break;

	default:
		return false;
		break;
	}
}

#endif // !VECTORS_HPP

