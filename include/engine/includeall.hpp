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

/** @file include/engine/includeall.hpp
 *  This file includes everything this engine has, for less typing.
 *  Don't include this file in the engine header files!
 *  the least problem is that the compile time will be big, if so.
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef INCLUDE_ENGINE_HPP
#define INCLUDE_ENGINE_HPP

 //from least to biggest dependencies
#include "engine_flags.hpp"
#include "engine_math.hpp"
#include "trig_lookup.hpp"
#include "typedefs.hpp"
#include "custom_types.hpp"
#include "global_vars.hpp"
#include "AEFrame.hpp"
#include "AELog.hpp"
#include "func_system.hpp"
#include "func_utils.hpp"
#include "AETimer.hpp"
#include "AEBaseClass.hpp"
#include "AEBeep.hpp"
#include "AEScreen.hpp"
#include "AEKeyboard.hpp"
#include "engine_include.hpp"

#endif // !INCLUDE_ENGINE_HPP



