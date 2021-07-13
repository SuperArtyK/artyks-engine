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

/** @file include/engine/AELog.hpp
 *  This file contains the logger class, for logging data to file.
 *  
 *  Should not cause everything to break.
 */

#pragma once

#ifndef FILELOG_HPP
#define FILELOG_HPP

#include <atomic>
#include <fstream>
#include <string>
#include "typedefs.hpp"
#include "global_vars.hpp"
using std::string;
using std::atomic;

//logger types, don't touch
///logger info message type  
#define LOG_INFO 0 
///logger warning message type  
#define LOG_WARN 1
///logger error message type
#define LOG_ERROR 2
///logger fatal error message type  
#define LOG_FERROR 3
///logger fatal error message type  
#define LOG_FATAL_ERROR 3
///logger severe warning message type  
#define LOG_SWARN 4
///logger severe warning message type  
#define LOG_SEVERE_WARNING 4
///logger ok message type  
#define LOG_OK 5
///logger success message type  
#define LOG_SUCCESS 6


///Default log path, used in modules and logger as default path. Changing it won't(hopefully) break anything, but will change lots of stuff  
#define DEF_LOG_PATH "logs/"


/// \brief This module implements logging as much as you will probably need.  
/// This module is not inherited from the base class, as it the base class is based on this class(AELog pointer)
/// that says it all, it's just a logger.  
class AELog {
public:
	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="l_strPathtolog">path to the log file. The folder it is bound to is the folder of exe(you can't really go above), by default it's DEF_LOG_PATH</param>
	/// <param name="l_strLogName">name of the log file. The log file will be in format of [log_name]_[year-month-day].log. By default it's "LOG"</param>
	AELog(const std::string& l_strPathtolog = DEF_LOG_PATH, const std::string& l_strLogName = "LOG");

	~AELog();
	/// <summary>
	/// writes to the log
	/// </summary>
	/// <param name="l_strMessg">message you want to log</param>
	/// <param name="l_iType">type of the message, refer to LOG_ defines</param>
	/// <param name="l_strProg_module">module name/just where the message came from</param>
	/// <returns>0 if completed successfully, 1 if AE_LOG_ENABLE is not defined</returns>
	int writetolog(const std::string& l_strMessg, const smalluint l_iType = LOG_INFO, const std::string& l_strProg_module = "Logger");


	///Returns the module name 
	///@see __AEBaseClass::getmodulename()
	inline string getmodulename(void) const{ return m_modulename; }
	/// returns m_modulenum of the module
	///@see __AEBaseClass::getmodulenum()
	inline biguint getmodulenum(void) const{return m_modulenum;}
	///returns m_ullEntrycount of the module
	inline biguint getentrycount(void)const { return m_ullEntrycount; }
#ifdef AE_EXPERIMENTAL
	/// uses all utils for class
	///@see __AEBaseClass::benchmark()
	void benchmark() {
		writetolog("");
		getmodulename();
		getmodulenum();
		getentrycount();
	}
#endif
private:	
	/// retusn date for the log file, in string with format of "[year]-[month]-[day]"
	static std::string logdate(void);
	///checks the given type and returns it's word representation
	static const char* checktype(const smalluint l_type);
	/// opens file
	/// <returns>0 if completed successfully, 1 if not or AE_LOG_ENABLE is not defined</returns>
	int openfile(void);
	/// closes file
	int closefile(void);
	/// opens file and starts logging, if AE_LOG_ENABLE is defined
	int startlogging(void);
	/// stops logging and closes file
	int stoplogging(void);
	///file stream variable for the current logfile
	std::fstream m_fstFilestr;//file "editor"
	/// number of the module, used for identification
	biguint m_modulenum;
	/// module name
	const std::string m_modulename;
	/// path to the log file
	const std::string m_strLogpath;
	/// log file name, includes the log path
	const std::string m_strFilename;
	///variable to store the module index number
	static atomic<biguint> m_globalmodulenum;
	/// number of entries this logger has made
	biguint m_ullEntrycount;
	/// object possible size
	static int m_staticObjSize;
	/// flag to indicate if the first log entry is written, used to check if we need to write intro text to log file
	static bool first_log_entry_done;
};
#ifdef AE_GLOBALMODULE
///our global logger, doesnt work correctly with concurrent stuff, so beware it a little
inline AELog global_logger;
#endif
//default logger, will be used if "useGlobLog" is set to true in modules
//wont be moved to global_vars.hpp
//will probably cause infinite recursion for compiler
//I don't want that 


namespace artyk::utils {
	/// <summary>
	/// This function logs to a file using AELog pointer, if AE_DEBUG is defined
	/// </summary>
	/// <param name="logptr">a pointer to the AELog logger</param>
	/// <param name="messg">the message you want to log</param>
	/// <param name="mtype">type of message(error, warning, etc)</param>
	/// <param name="mouldename">name of the module that asked for logging</param>
	/// <returns>0 if AE_DEBUG and AE_LOG_ENABLE are defined and logptr is not nullptr, 1 otherwise</returns>
	inline int debug_log(AELog* logptr, const std::string& messg = "", const smalluint mtype = LOG_INFO, const std::string& mouldename = DEF_MNAME) {//wrapper for debug logging
#ifdef AE_DEBUG
#ifdef AE_LOG_ENABLE
	if (logptr) {
		logptr->writetolog(messg, mtype, mouldename);
		return 0;
	}

#endif // AE_LOG_DISABLE
		
#endif
		return 1;
	}

	/// <summary>
	/// This function logs to a file using AELog pointer
	/// </summary>
	/// <param name="logptr">a pointer to the AELog logger</param>
	/// <param name="messg">the message you want to log</param>
	/// <param name="mtype">type of message(error, warning, etc)</param>
	/// <param name="mouldename">name of the module that asked for logging</param>
	/// <returns>0 if AE_LOG_ENABLE is defined and logptr is not nullptr, 1 otherwise</returns>
	inline int normal_log(AELog* logptr, const std::string& messg = "", const smalluint mtype = LOG_INFO, const std::string& mouldename = DEF_MNAME) {//wrapper for normal logging
#ifdef AE_LOG_ENABLE
	if (logptr) {
			logptr->writetolog(messg, mtype, mouldename);
			return 0;
	}

#endif // AE_LOG_DISABLE
	
		return 1;
	}
}

#endif //FILELOG_HPP



