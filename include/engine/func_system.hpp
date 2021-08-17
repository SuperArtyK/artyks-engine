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

/** @file include/engine/func_system.hpp
 *  This file contains functions for system message box creating.
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef AEFUNC_SYSTEM_HPP
#define AEFUNC_SYSTEM_HPP

#include "include/engine/AELog.hpp"
using std::string;
using std::atomic;
using std::to_string;

///gets full info of code for AEDebugInfo, not working as func default argument
#define GET_FULL_DBG_INFO AEDebugInfo{ __FILE__, __DATE__, __TIME__, __FUNCTION__, __LINE__}
///gets some info of code for AEDebugInfo, this one works as func default arg
#define GET_DBG_INFO AEDebugInfo{__FILE__, __DATE__, __TIME__, "", __LINE__}

/// \brief This struct is used for collecting and using in-place debug info.
/// Please use the macros, GET_FULL_DBG_INFO and GET_DBG_INFO, to get the information.
struct AEDebugInfo
{
	///data of current file
	const string currentfile;
	///compilation date of the file
	const string compiledate;
	///compilation time of the file
	const string compiletime;
	///function name this is used in
	const string compilefunc;
	///line of code this is used in
	const int line;
};

namespace artyk {//continuing our namespace
	namespace utils {
		/// <summary>
		/// Throws engine fatal error window and closes the program
		/// </summary>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="dbg">debug info</param>
		/// <param name="errorcode">error code to exit with</param>
		inline void FError(const string& text = "Fatal error", const string& modulename = DEF_MNAME, const AEDebugInfo& dbg = GET_DBG_INFO, int errorcode = 1) {
			//throws an error message box with given text and optional modulename
			//and then exits the program with given error code
			MessageBoxA(g_console_hwnd, (
				text +
#ifdef AE_DETAILED_WINDOW_MESSAGES
				"\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES +
#endif // AE_DETAILED_WINDOW_MESSAGES

				"\nThe Engine will now exit").c_str(), "Engine FATAL ERROR:", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
			exit(errorcode);
		}
		/// <summary>
		/// Throws engine error window
		/// </summary>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="dbg">debug info</param>
		inline void Error(const string& text = "Error", const string& modulename = DEF_MNAME, const AEDebugInfo& dbg = GET_DBG_INFO) {
			//throws an error message box with given text and optional modulename

			MessageBoxA(g_console_hwnd, (
				text +
#ifdef AE_DETAILED_WINDOW_MESSAGES
				"\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES +
#endif // AE_DETAILED_WINDOW_MESSAGES

				"\nThe Engine will continue").c_str(), "Engine ERROR:", MB_OK | MB_ICONERROR);
		}

		/// <summary>
		/// Throws engine warning window
		/// </summary>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="usedebug">flag if to use the debug info</param>
		/// <param name="dbg">debug info</param>
		inline void Warn(const string& text = "Warning", const string& modulename = DEF_MNAME, bool usedebug = false, const AEDebugInfo& dbg = GET_DBG_INFO) {
			//throws an warning message box with given text and optional modulename

			MessageBoxA(g_console_hwnd, (
				text + "\n\nModule: " + modulename
#ifdef AE_DETAILED_WINDOW_MESSAGES
				+ ((usedebug) ? "\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES : "")
#endif // AE_DETAILED_WINDOW_MESSAGES

				).c_str(), "Engine Warning:", MB_OK | MB_ICONWARNING);
		}

		/// <summary>
		/// Throws engine information window
		/// </summary>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="usedebug">flag if to use the debug info</param>
		/// <param name="dbg">debug info</param>
		inline void Info(const string& text = "Info", const string& modulename = DEF_MNAME, bool usedebug = false, const AEDebugInfo& dbg = GET_DBG_INFO) {
			//throws an information message box with given text and optional modulename

			MessageBoxA(g_console_hwnd, (
				text
#ifdef AE_DETAILED_WINDOW_MESSAGES
				+ ((usedebug) ? "\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES : "")
#endif // AE_DETAILED_WINDOW_MESSAGES

				).c_str(), "Engine Info:", MB_OK | MB_ICONINFORMATION);
		}

		//these are exact copies of functions above, but with logging to the logfile of the given AELog

		/// <summary>
		/// Throws engine fatal error window and closes the program, and logs that into log file
		/// </summary>
		/// <param name="logger">the logger that will be used for logging text into log file</param>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="dbg">debug info</param>
		/// <param name="errorcode">error code to exit with</param>
		inline void FError_log(AELog* logger, const string& text = "Fatal error(log)", const string& modulename = DEF_MNAME, const AEDebugInfo& dbg = GET_DBG_INFO, int errorcode = 1) {
			//throws an error message box with given text and modulename
			//and then exits the program with given error code
			if (logger)
				logger->writetolog(text, LOG_FATAL_ERROR, modulename);
			MessageBoxA(g_console_hwnd, (
				text +
#ifdef AE_DETAILED_WINDOW_MESSAGES
				"\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES +
#endif // AE_DETAILED_WINDOW_MESSAGES

				"\nThe Engine will now exit").c_str(), "Engine FATAL ERROR:", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);
			exit(errorcode);
		}

		/// <summary>
		/// Throws engine error window and logs that into log file
		/// </summary>
		/// <param name="logger">the logger that will be used for logging text into log file</param>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="dbg">debug info</param>
		inline void Error_log(AELog* logger, const string& text = "Error(log)", const string& modulename = DEF_MNAME, const AEDebugInfo& dbg = GET_DBG_INFO) {
			//throws an error message box with given text and modulename
			if (logger)
				logger->writetolog(text, LOG_ERROR, modulename);
			MessageBoxA(g_console_hwnd, (
				text +
#ifdef AE_DETAILED_WINDOW_MESSAGES
				"\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES +
#endif // AE_DETAILED_WINDOW_MESSAGES

				"\nThe Engine will now exit").c_str(), "Engine ERROR:", MB_OK | MB_ICONERROR);
		}

		/// <summary>
		/// Throws engine warning window and logs that into log file
		/// </summary>
		/// <param name="logger">the logger that will be used for logging text into log file</param>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="usedebug">flag if to use the debug info</param>
		/// <param name="dbg">debug info</param>
		inline void Warn_log(AELog* logger, const string& text = "Warning(log)", const string& modulename = DEF_MNAME, bool usedebug = false, const AEDebugInfo& dbg = GET_DBG_INFO) {
			//throws an warning message box with given text and modulename

			if (logger)
				logger->writetolog(text, LOG_WARN, modulename);
			MessageBoxA(g_console_hwnd, (
				text + "\n\nModule: " + modulename
#ifdef AE_DETAILED_WINDOW_MESSAGES
				+ ((usedebug) ? "\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES : "")
#endif // AE_DETAILED_WINDOW_MESSAGES

				).c_str(), "Engine Warning:", MB_OK | MB_ICONWARNING);
		}

		/// <summary>
		/// Throws engine information window and logs that into log file
		/// </summary>
		/// <param name="logger">the logger that will be used for logging text into log file</param>
		/// <param name="text">text of the window</param>
		/// <param name="modulename">the name of the module that asked for the window</param>
		/// <param name="usedebug">flag if to use the debug info</param>
		/// <param name="dbg">debug info</param>
		inline void Info_log(AELog* logger, const string& text = "Info(log)", const string& modulename = DEF_MNAME, bool usedebug = false, const AEDebugInfo& dbg = GET_DBG_INFO) {
			//throws an information message box with given text and modulename

			if (logger)
				logger->writetolog(text, LOG_INFO, modulename);
			MessageBoxA(g_console_hwnd, (
				text
#ifdef AE_DETAILED_WINDOW_MESSAGES
				+ ((usedebug) ? "\n\nModule: " + modulename + AE_DETAILED_WINDOW_MESSAGES : "")
#endif // AE_DETAILED_WINDOW_MESSAGES

				).c_str(), "Engine Info:", MB_OK | MB_ICONINFORMATION);
		}
	}
}

#endif // !AEFUNC_SYSTEM_HPP