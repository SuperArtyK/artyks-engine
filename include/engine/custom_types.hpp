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

#include "vectors.hpp"

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
	///flag if key is used at all(if either of flags is true)
	bool m_isUsed;
};



#endif // !CUSTOM_TYPES_HPP


