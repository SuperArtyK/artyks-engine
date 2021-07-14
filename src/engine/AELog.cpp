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

/** @file src/engine/AELog.cpp
 *  This file contains the logger functions' definitions(refer to AELog.hpp).
 * 
 *  Should not cause everything to break.
 */

#include "AELog.hpp"
#include "func_utils.hpp"
#include "typedefs.hpp"
#include <filesystem>
#include <intrin.h>


atomic<biguint> AELog::m_globalmodulenum;
int AELog::m_staticObjSize = sizeof(atomic<biguint>) +
sizeof(std::fstream) +
sizeof(biguint) * 2 +
(sizeof(std::string) * 1188) +
sizeof(int) +
sizeof(bool);

bool AELog::first_log_entry_done = false;//for marking the exe start in logs, inside filelog
//important stuff
AELog::AELog(const std::string& l_strPathtolog, const std::string& l_strLogName): 
m_modulenum(++m_globalmodulenum),
m_ullEntrycount(0),
m_modulename("AELog" + to_string(m_modulenum)),
m_strLogpath(l_strPathtolog + '/'),
m_strFilename(m_strLogpath + l_strLogName + "_" + logdate() + ".log")

{
#ifdef AE_LOG_ENABLE
	
		std::filesystem::create_directories(m_strLogpath);
		startlogging();

#endif // AE_LOG_DISABLE
	
}

AELog::~AELog() {
#ifdef AE_LOG_ENABLE
	stoplogging();
#endif // AE_LOG_DISABLE
}


//logging itself
int AELog::writetolog(const std::string& l_strMessg, const smalluint l_iType, const std::string& l_strProg_module) {
	
#ifndef AE_LOG_ENABLE
	return 1;
#else
	std::string m_message;
	if (!first_log_entry_done && !artyk::app_name.empty()) {
		first_log_entry_done = true;
		m_message = m_strLogpath + "LOG_" + logdate() + ".log";
		std::fstream engstarted(m_message.c_str(), std::fstream::out | std::fstream::app);
		m_message = "[ " + artyk::utils::currentDateTime() + " ] [" + checktype(LOG_SUCCESS) + "] [Engine]: " + "Started \"" + artyk::app_name + "\". Version: " + artyk::app_version + " Build: " + to_string(artyk::app_build) + "\n";
#ifdef AE_DEBUG
		//https://stackoverflow.com/questions/850774/how-to-determine-the-hardware-cpu-and-ram-on-a-machine
		//thanks bsruth

		SYSTEM_INFO sysinf;
		GetSystemInfo(&sysinf);
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: ----------START SYSTEM INFO----------\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Processor arch: ");
		switch (sysinf.wProcessorArchitecture)
		{
		case PROCESSOR_ARCHITECTURE_AMD64:
			m_message.append("INTEL/AMD x64\n");
			break;
		case PROCESSOR_ARCHITECTURE_ARM:
			m_message.append("ARM\n");
			break;
		case PROCESSOR_ARCHITECTURE_ARM64:
			m_message.append("ARM64\n");
			break;
		case PROCESSOR_ARCHITECTURE_IA64:
			m_message.append("INTEL ITANIUM\n");
			break;
		case PROCESSOR_ARCHITECTURE_INTEL:
			m_message.append("INTEL/AMD x86\n");
			break;
		default:
			m_message.append("UNKNOWN ARCHITECTURE\n");
			break;
		}
		int CPUInfo[4] = { -1 };
		unsigned int nExIds;
		char CPUBrandString[0x40];
		// Get the information associated with each extended ID.
		__cpuid(CPUInfo, 0x80000000);
		nExIds = CPUInfo[0];
		for (unsigned int i = 0x80000000; i <= nExIds; ++i)
		{
			__cpuid(CPUInfo, i);
			// Interpret CPU brand string
			if (i == 0x80000002)
				memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
			else if (i == 0x80000003)
				memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
			else if (i == 0x80000004)
				memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
		}
		//string includes manufacturer, model and clockspeed
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: CPU Name: " + CPUBrandString + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Number of Cores: " + to_string(sysinf.dwNumberOfProcessors) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Processor level: " + to_string(sysinf.wProcessorLevel) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Processor type: " + to_string(sysinf.dwProcessorType) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Active processor mask: " + to_string(sysinf.dwActiveProcessorMask) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Memory Page Size: " + to_string(sysinf.dwPageSize) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Memory allocation granularity: " + to_string(sysinf.dwAllocationGranularity) + "\n");
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof(statex);
		GlobalMemoryStatusEx(&statex);
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Total System Memory: " + to_string(statex.ullTotalPhys / 1024) + "KB\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Free System Memory:  " + to_string(statex.ullAvailPhys / 1024) + "KB\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Memory usage: " + to_string(statex.dwMemoryLoad) + "%\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Total Page File: " + to_string(statex.ullTotalPageFile / 1024) + "KB\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Free Page File:  " + to_string(statex.ullAvailPageFile / 1024) + "KB\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Total Process Memory: " + to_string(statex.ullTotalVirtual / 1024) + "KB\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Free Process Memory:  " + to_string(statex.ullAvailVirtual / 1024) + "KB\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Minimum Application Address: " + artyk::utils::addrtostr(sysinf.lpMinimumApplicationAddress) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: Maximum Application Address: " + artyk::utils::addrtostr(sysinf.lpMaximumApplicationAddress) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: OEM ID: " + to_string(sysinf.dwOemId) + "\n");
		m_message.append("[" + artyk::utils::currentDateTime() + "][" + checktype(LOG_INFO) + "][Engine]: ----------END SYSTEM INFO----------\n");
#endif // AE_DEBUG
		engstarted.write(m_message.c_str(), m_message.size());
		engstarted.close();
		m_ullEntrycount++;
	}

	m_ullEntrycount++;
	
	if (l_strMessg.empty()) {
		m_message = "[ " + artyk::utils::currentDateTime() + " ] [" + checktype(LOG_INFO) + "] [" + m_modulename + "]: Sample entry. This logger object uses approx. " + std::to_string(m_staticObjSize + m_message.capacity()) + " bytes and has made " + std::to_string(m_ullEntrycount) + " log entries\n";
	}
	else {
		m_message = 
			"[ " + artyk::utils::currentDateTime() + 
			" ] [" + checktype(l_iType) + 
			"] [" + l_strProg_module + 
			"]: "+ l_strMessg+
			"\n";
	}
	m_fstFilestr.write(m_message.c_str(), m_message.length());
	return 0;
#endif // AE_LOG_DISABLE
	
}



int AELog::stoplogging(void) {
	//if (m_bDev_cout)std::cout << "LOGGER_MAIN:stopping writing thread" << std::endl;
#ifdef AE_LOG_ENABLE
	if (m_fstFilestr.is_open()) {
		writetolog("Closing current logging session...", LOG_WARN, m_modulename);
		closefile();
	}

#endif // AE_LOG_DISABLE

	return 0;
}
int AELog::startlogging(void) {
#ifdef AE_LOG_ENABLE
	openfile();
	writetolog("Opening new logging session...", LOG_WARN, m_modulename);

#endif // AE_LOG_DISABLE

	
	return 0;
}

//utils

std::string AELog::logdate(void) {
	std::string temp = artyk::utils::currentDateTime();
	return temp.substr(0,artyk::utils::findinstr(temp,".")-1);
}



int AELog::openfile(void) {
#ifdef AE_LOG_ENABLE
m_fstFilestr.open(m_strFilename.c_str(), std::fstream::out | std::fstream::app | std::fstream::binary);
	if (m_fstFilestr.is_open()) {
		return 0;
	}
#endif // AE_LOG_DISABLE
	
	return 1;
}
int AELog::closefile(void) {
	m_fstFilestr.close();
	return 0;
}

const char* AELog::checktype(const smalluint l_type) {

	switch (l_type)
	{

	case LOG_INFO:
		return "INFO";
		break;

	case LOG_WARN:
		return "WARN";
		break;

	case LOG_ERROR:
		return "ERROR";
		break;

	case LOG_FERROR:
		return "FATAL_ERROR";
		break;

	case LOG_SWARN:
		return "SEVERE_WARN";
		break;

	case LOG_OK:
		return "OK";
		break;

	case LOG_SUCCESS:
		return "SUCCESS";
		break;

	default:
		return "INFO";//default will be info, 
		break;
	}

}












//definitions of global functions
//moved to func_utils.hpp
//int movetoendSTR(std::string l_strarr[], int l_in, int l_ipos);
//int movetoendINT(int l_iArr[], int l_iN, int l_iPos);
//const char* BoolToString(bool b);


///////////////////////////////////////////////////////////////
//	Commented out code moved to logger_old_code.txt
///////////////////////////////////////////////////////////////




