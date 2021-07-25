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

/** @file include/engine/engine_flags.hpp
 *  This file contains engine flags that are used to alter it's code and behaviour.
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef ENGINE_FLAGS_HPP
#define ENGINE_FLAGS_HPP

 //engine flags

  ///enables the debug features
#define AE_DEBUG

///enables global modules
///the global_timer and global_focusScanner don't apply to that
#define AE_GLOBALMODULE

///experimental features
///WARNING:May break engine completely!
#define AE_EXPERIMENTAL

///enables the trig lookup tables(may compile slow AF!)
#define AE_LOOKUP_TABLE

///enables logger
#define AE_LOG_ENABLE //enables logger

///auto-adding application title when changing title with AEScreen
#define AE_ADD_APP_TITLE

///AEFrame delay in AEKeyboard key scanning thread
#define AE_KB_DELAY

///AEFrame delay in AEBeep playing thread
#define AE_BP_DELAY

///utilises the AEDebugInfo, currently used in message functions
///good for debugging
///@see func_system.hpp
#define AE_DETAILED_WINDOW_MESSAGES "\nLine: " + to_string(dbg.line) + "\nFile: " + dbg.currentfile + "\nCompile Date: " + dbg.compiledate + "\nCompile Time: " + dbg.compiletime + "\nFunction: " + dbg.compilefunc

///enable mouse scanning in the global_keyboard module
#define AE_GLOBALKB_ENABLE_MOUSE

#define AE_GFX_ENABLE_WRAPPING

#define AE_GFX_ALWAYS_CLEAR_AFTER_DRAW

//undefine flags to turn them off

#undef AE_DEBUG
#undef AE_GLOBALMODULE
#undef AE_EXPERIMENTAL
//#undef AE_LOOKUP_TABLE
#undef AE_LOG_ENABLE
#undef AE_ADD_APP_TITLE
//#undef AE_KB_DELAY
//#undef AE_BP_DELAY
//#undef AE_DETAILED_WINDOW_MESSAGES
#undef AE_GLOBALKB_ENABLE_MOUSE
#undef AE_GFX_ENABLE_WRAPPING
#undef AE_GFX_ALWAYS_CLEAR_AFTER_DRAW

//I know this is a crutch, but I don't want to add AE_DEBUG to the compiler settings, might break all of it
#ifdef NDEBUG
//remove if you want to test debug features in Release mode
#undef AE_DEBUG

#endif // NDEBUG

#endif // !ENGINE_FLAGS_HPP