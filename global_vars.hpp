/*
	ArtyK's Console (Game) Engine. Console engine for apps and games
	Copyright (C) 2021  Artemii Kozhemiak

	https://github.com/SuperArtyK/myengine

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//////////////////////////////////////////////////////////////////////////
// this file contains the global variables that can be used everywhere
// should not cause everything to redefine
//////////////////////////////////////////////////////////////////////////

#pragma once


#ifndef GLOB_VARS
#define GLOB_VARS

#include <atomic>
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <string>
//#include "keyboard.hpp"
using std::vector;
using std::string;
using std::atomic;
using std::wstring;
using std::to_string;




//color codes
#define BLACK	 0 
#define DBLUE	 1  //DARK
#define DGREEN	 2  //DARK
#define DCYAN	 3  //DARK
#define DRED	 4  //DARK
#define DPURPLE	 5  //DARK
#define DYELLOW	 6  //DARK
#define DWHITE	 7  //DARK
#define GRAY	 8  
#define BLUE	 9  //BRIGHT
#define GREEN	 10 //BRIGHT
#define CYAN	 11 //BRIGHT
#define RED		 12 //BRIGHT
#define PURPLE	 13 //BRIGHT
#define YELLOW	 14 //BRIGHT
#define WHITE	 15 //BRIGHT

#define GAME_FPS 60


inline const int def_color[2] = { BLACK, GREEN };
#define NOCOLOR -1
#define DEFCLR_BGR def_color[0]
#define DEFCLR_FGR def_color[1]

//im making this, so I can differentiate between std, mine, and other namespaces
namespace artyk {
	
	inline int closing_app = 0;//flag if app is being closed, 1 if it user closes app, more -- modules are turning off
	const string app_name = "ArtyK's Engine Test";
	const int app_build = 162;//how do you autoincrement these?
	const string app_version = "v.0.0.3";
	const string app_name_full = app_name + " " + app_version + ":"+to_string(app_build);
	
	const string key_names[173]{//for GetKeyID
		//DO NOT TOUCH! unless you know what you're doing
		"lbutton",
		"rbutton",
		"cancel",
		"mbutton",
		"xbutton1",
		"xbutton2",
		"back",
		"tab",
		"clear",
		"return",
		"shift",
		"control",
		"menu",
		"pause",
		"capital",
		"kana",
		"hanguel",
		"hangul",
		"junja",
		"final",
		"hanja",
		"kanji",
		"escape",
		"convert",
		"nonconvert",
		"accept",
		"modechange",
		"space",
		"prior",
		"next",
		"end",
		"home",
		"left",
		"up",
		"right",
		"down",
		"select",
		"print",
		"execute",
		"snapshot",
		"insert",
		"delete",
		"help",
		"0",
		"1",
		"2",
		"3",
		"4",
		"5",
		"6",
		"7",
		"8",
		"9",
		"a",
		"b",
		"c",
		"d",
		"e",
		"f",
		"g",
		"h",
		"i",
		"j",
		"k",
		"l",
		"m",
		"n",
		"o",
		"p",
		"q",
		"r",
		"s",
		"t",
		"u",
		"v",
		"w",
		"x",
		"y",
		"z",
		"lwin",
		"rwin",
		"apps",
		"sleep",
		"numpad0",
		"numpad1",
		"numpad2",
		"numpad3",
		"numpad4",
		"numpad5",
		"numpad6",
		"numpad7",
		"numpad8",
		"numpad9",
		"multiply",
		"add",
		"separator",
		"subtract",
		"decimal",
		"divide",
		"f1",
		"f2",
		"f3",
		"f4",
		"f5",
		"f6",
		"f7",
		"f8",
		"f9",
		"f10",
		"f11",
		"f12",
		"f13",
		"f14",
		"f15",
		"f16",
		"f17",
		"f18",
		"f19",
		"f20",
		"f21",
		"f22",
		"f23",
		"f24",
		"numlock",
		"scroll",
		"lshift",
		"rshift",
		"lcontrol",
		"rcontrol",
		"lmenu",
		"rmenu",
		"browserback",
		"browserforward",
		"browserrefresh",
		"browserstop",
		"browsersearch",
		"browserfavorites",
		"browserhome",
		"volumemute",
		"volumedown",
		"volumeup",
		"medianexttrack",
		"mediaprevtrack",
		"mediastop",
		"mediaplaypause",
		"launchmail",
		"launchmediaselect",
		"launchapp1",
		"launchapp2",
		"oem1",
		"oemplus",
		"oemcomma",
		"oemminus",
		"oemperiod",
		"oem2",
		"oem3",
		"oem4",
		"oem5",
		"oem6",
		"oem7",
		"oem8",
		"oem102",
		"processkey",
		"packet",
		"attn",
		"crsel",
		"exsel",
		"ereof",
		"play",
		"zoom",
		"noname",
		"pa1",
		"oemclear",
};

	const short key_ids[173]{//for GetKeyID
		//DO NOT TOUCH! unless you know what you're doing
	1,
	2,
	3,
	4,
	5,
	6,
	8,
	9,
	12,
	13,
	16,
	17,
	18,
	19,
	20,
	21,
	21,
	21,
	23,
	24,
	25,
	25,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
	34,
	35,
	36,
	37,
	38,
	39,
	40,
	41,
	42,
	43,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	56,
	57,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	134,
	135,
	144,
	145,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
	182,
	183,
	186,
	187,
	188,
	189,
	190,
	191,
	192,
	219,
	220,
	221,
	222,
	223,
	226,
	229,
	231,
	246,
	247,
	248,
	249,
	250,
	251,
	252,
	253,
	254,
};

	const string log_typekw[8]{//message type, for m_iLogtype

		"info",
		"warn",
		"error",
		"fatal_error",
		"severe_warn",
		"ok!",
		"success!",

	};

	const string log_modkw[6]{
		"main",
		"sound",
		"logger",
		"keyboard",
		"graphics",

	};
	// inline string log_msgkw[3]{
	// 	"bad",
	// 	"good",
	// 
	// 
	// 
	// };


	const int log_kwcolor[15][2]{

		//type
		{DEFCLR_BGR, CYAN},//info
		{BLACK, YELLOW},//warn
		{DEFCLR_BGR, RED},//error
		{DRED, BLACK},//fatal_error
		{YELLOW, BLACK},//severe_warn
		{DEFCLR_BGR, GREEN},//ok!
		{GREEN, BLACK},//success!

		//module
		{DEFCLR_BGR, PURPLE},//main
		{DEFCLR_BGR, DCYAN},//sound
		{DEFCLR_BGR, WHITE},//logger
		{DEFCLR_BGR, GRAY},//keyboard
		{DEFCLR_BGR, DPURPLE},//graphics


				{DEFCLR_BGR, DBLUE},//bad
				{DEFCLR_BGR, BLUE},//good

	};

}











// inline int log_kwcolor[sizeof(log_keywords) / sizeof(log_keywords[0])][2]{
// 
// //  backgr, foregr
// 	{NOCOLOR, RED},//error
// 	{DRED, BLACK},//fatal_error
// 	{NOCOLOR, GREEN},//ok!
// 	{GREEN, BLACK},//success!
// 	{BLACK, YELLOW},//warn
// 	{YELLOW, BLACK},//severe_warn
// 	{NOCOLOR, CYAN},//info
// 	{NOCOLOR, PURPLE},//main
// 	{NOCOLOR, DCYAN},//sound
// 	{NOCOLOR, WHITE},//logger
// 	{NOCOLOR, GRAY},//keyboard
// 	{NOCOLOR, DPURPLE},//graphics
// 	{NOCOLOR, DRED},//bad
// 	{NOCOLOR, DGREEN},//good
// 	{DWHITE, DWHITE},
// 
// 
// 
// 
// };




#endif // !GLOB_VARS


