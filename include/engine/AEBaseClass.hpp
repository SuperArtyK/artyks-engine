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

/** @file include/engine/AEBaseClass.hpp
 *  This file contains the code for basic ArtyK's Console Engine(utility) class.  
 *  
 *  Should not cause everything to break.  
 */

#pragma once

#ifndef BASECLASS_HPP
#define BASECLASS_HPP

#include "AEFrame.hpp"
#include "AELog.hpp"
using std::string;
using std::to_string;


/// \brief This is the basic class for most of the modules of the ArtyK's Console Engine.  
/// Has the most things that all these modules need.  
class __AEBaseClass {
public:

	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="modulename">name of the module to assign to "m_modulename"</param>
	/// <param name="modulenum">number of the module to assign to "m_modulenum" and to add to "m_modulename"</param>
	explicit __AEBaseClass(const string& modulename, biguint modulenum) : m_logptr(nullptr), m_modulename(modulename+to_string(modulenum)),
		m_modulenum(modulenum) {}

	///virtual destructor, so you can use base class for modules, instead of specifying the module explicitly.
	virtual ~__AEBaseClass() = default;


	/// returns the "m_modulename" of the module 
	virtual inline string getmodulename(void) const {
		return m_modulename;
	}

	/// returns the "m_modulenum" of the module
	virtual inline biguint getmodulenum(void) const {
		return m_modulenum;
	}
#ifdef AE_EXPERIMENTAL
	/// uses all utils for class
	virtual void benchmark(void)  {
		getmodulename();
		getmodulenum();
	}

#endif // AE_EXPERIMENTAL



protected:
	/// pointer to our possibly future logger
	AELog* m_logptr;
	/// module name
	const string m_modulename;
	/// number of the module, used for identification
	const biguint m_modulenum;

};




#endif // !BASECLASS_HPP
