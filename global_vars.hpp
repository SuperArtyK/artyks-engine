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

#include "include.hpp"
//#include "keyboard.hpp"
using namespace std;

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

//im making this, so I can differentiate between std, mine, and others
namespace artyk {
	
	inline int closing_app = 0;//flag if app is being closed, 1 if it user closes app, more -- modules are turning off
	const string app_name = "ArtyK's Engine Test";
	const int app_build = 162;//how do you autoincrement these?
	const string app_version = "v.0.0.3";
	
	const string keynames[173][2]{//for GetKeyID
		//DO NOT TOUCH! unless you know what you're doing
		{"lbutton",                      "1"  },
	{"rbutton",                      "2"  },
	{"cancel",                       "3"  },
	{"mbutton",                      "4"  },
	{"xbutton1",                     "5"  },
	{"xbutton2",                     "6"  },
	{"back",                         "8"  },
	{"tab",                          "9"  },
	{"clear",                        "12" },
	{"return",                       "13" },
	{"shift",                        "16" },
	{"control",                      "17" },
	{"menu",                         "18" },
	{"pause",                        "19" },
	{"capital",                      "20" },
	{"kana",                         "21" },
	{"hanguel",                      "21" },
	{"hangul",                       "21" },
	{"junja",                        "23" },
	{"final",                        "24" },
	{"hanja",                        "25" },
	{"kanji",                        "25" },
	{"escape",                       "27" },
	{"convert",                      "28" },
	{"nonconvert",                   "29" },
	{"accept",                       "30" },
	{"modechange",                   "31" },
	{"space",                        "32" },
	{"prior",                        "33" },
	{"next",                         "34" },
	{"end",                          "35" },
	{"home",                         "36" },
	{"left",                         "37" },
	{"up",                           "38" },
	{"right",                        "39" },
	{"down",                         "40" },
	{"select",                       "41" },
	{"print",                        "42" },
	{"execute",                      "43" },
	{"snapshot",                     "44" },
	{"insert",                       "45" },
	{"delete",                       "46" },
	{"help",                         "47" },
	{"0",                            "48" },
	{"1",                            "49" },
	{"2",                            "50" },
	{"3",                            "51" },
	{"4",                            "52" },
	{"5",                            "53" },
	{"6",                            "54" },
	{"7",                            "55" },
	{"8",                            "56" },
	{"9",                            "57" },
	{"a",                            "65" },
	{"b",                            "66" },
	{"c",                            "67" },
	{"d",                            "68" },
	{"e",                            "69" },
	{"f",                            "70" },
	{"g",                            "71" },
	{"h",                            "72" },
	{"i",                            "73" },
	{"j",                            "74" },
	{"k",                            "75" },
	{"l",                            "76" },
	{"m",                            "77" },
	{"n",                            "78" },
	{"o",                            "79" },
	{"p",                            "80" },
	{"q",                            "81" },
	{"r",                            "82" },
	{"s",                            "83" },
	{"t",                            "84" },
	{"u",                            "85" },
	{"v",                            "86" },
	{"w",                            "87" },
	{"x",                            "88" },
	{"y",                            "89" },
	{"z",                            "90" },
	{"lwin",                         "91" },
	{"rwin",                         "92" },
	{"apps",                         "93" },
	{"sleep",                        "95" },
	{"numpad0",                      "96" },
	{"numpad1",                      "97" },
	{"numpad2",                      "98" },
	{"numpad3",                      "99" },
	{"numpad4",                      "100"},
	{"numpad5",                      "101"},
	{"numpad6",                      "102"},
	{"numpad7",                      "103"},
	{"numpad8",                      "104"},
	{"numpad9",                      "105"},
	{"multiply",                     "106"},
	{"add",                          "107"},
	{"separator",                    "108"},
	{"subtract",                     "109"},
	{"decimal",                      "110"},
	{"divide",                       "111"},
	{"f1",                           "112"},
	{"f2",                           "113"},
	{"f3",                           "114"},
	{"f4",                           "115"},
	{"f5",                           "116"},
	{"f6",                           "117"},
	{"f7",                           "118"},
	{"f8",                           "119"},
	{"f9",                           "120"},
	{"f10",                          "121"},
	{"f11",                          "122"},
	{"f12",                          "123"},
	{"f13",                          "124"},
	{"f14",                          "125"},
	{"f15",                          "126"},
	{"f16",                          "127"},
	{"f17",                          "128"},
	{"f18",                          "129"},
	{"f19",                          "130"},
	{"f20",                          "131"},
	{"f21",                          "132"},
	{"f22",                          "133"},
	{"f23",                          "134"},
	{"f24",                          "135"},
	{"numlock",                      "144"},
	{"scroll",                       "145"},
	{"lshift",                       "160"},
	{"rshift",                       "161"},
	{"lcontrol",                     "162"},
	{"rcontrol",                     "163"},
	{"lmenu",                        "164"},
	{"rmenu",                        "165"},
	{"browserback",                  "166"},
	{"browserforward",               "167"},
	{"browserrefresh",               "168"},
	{"browserstop",                  "169"},
	{"browsersearch",                "170"},
	{"browserfavorites",             "171"},
	{"browserhome",                  "172"},
	{"volumemute",                   "173"},
	{"volumedown",                   "174"},
	{"volumeup",                     "175"},
	{"medianexttrack",               "176"},
	{"mediaprevtrack",               "177"},
	{"mediastop",                    "178"},
	{"mediaplaypause",               "179"},
	{"launchmail",                   "180"},
	{"launchmediaselect",            "181"},
	{"launchapp1",                   "182"},
	{"launchapp2",                   "183"},
	{"oem1",                         "186"},
	{"oemplus",                      "187"},
	{"oemcomma",                     "188"},
	{"oemminus",                     "189"},
	{"oemperiod",                    "190"},
	{"oem2",                         "191"},
	{"oem3",                         "192"},
	{"oem4",                         "219"},
	{"oem5",                         "220"},
	{"oem6",                         "221"},
	{"oem7",                         "222"},
	{"oem8",                         "223"},
	{"oem102",                       "226"},
	{"processkey",                   "229"},
	{"packet",                       "231"},
	{"attn",                         "246"},
	{"crsel",                        "247"},
	{"exsel",                        "248"},
	{"ereof",                        "249"},
	{"play",                         "250"},
	{"zoom",                         "251"},
	{"noname",                       "252"},
	{"pa1",                          "253"},
	{"oemclear",                     "254"},
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


