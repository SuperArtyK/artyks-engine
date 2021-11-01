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

/** @file include/engine/AEPalette.hpp
 *  This file contains engine palette altering code.
 *
 *  Should not cause everything to break.
 */

#pragma once

#include "typedefs.hpp"
#include <array>


namespace artyk::color {
	constexpr uint rgbtoint(const smalluint r, const smalluint g, const smalluint b) {
		return uint(r | (g << 8) | (b << 16));
	}
	constexpr std::array<smalluint, 3> inttorgb(const uint rgbint) {
		return std::array<smalluint, 3>{smalluint(rgbint), smalluint(rgbint >> 8), smalluint(rgbint >> 16)};
	}

	constexpr uint invertcolor(const uint rgb) {
		return (~rgb << 8) >> 8;
	}
}


class AEPalette {
public:


	/// <summary>
	/// Class constructor
	/// </summary>
	/// <param name="palette">Palette color array to set. Leave blank for default engine palette to be applied</param>
	AEPalette(const std::array<uint, 16>& palette = cp_default) {
		for (int i = 0; i < 16; i++) {
			this->colorpal[i] = palette[i];
		}
	}
	///returns rgb value of color from given color number in palette
	uint getcolor(const smalluint colornum) const {
		return this->colorpal[colornum];
	}

	///sets color in palette to certain rgb value
	void setcolor(const smalluint colornum, const uint col) {
		this->colorpal[colornum] = col;
	}

	std::array<uint, 16> getpalette(const smalluint tpe = 0) const {
		switch (tpe)
		{
		case 1:
			return cp_default;
			break;

		case 2:
			return cp_conclassic;
			break;

		default:
			return colorpal;
			break;
		}
	}

	void getpalette(const AEPalette& two) {
		for (int i = 0; i < 16; i++) {
			this->colorpal = two.colorpal;
		}
	}

private:
	///default engine palette
	constexpr static std::array<uint, 16> cp_default{
		//dark
		artyk::color::rgbtoint(0,   0,   0),  //black 
		artyk::color::rgbtoint(0,   0,   128),//blue  
		artyk::color::rgbtoint(0,   128, 0),  //green 
		artyk::color::rgbtoint(0,   128, 128),//cyan  
		artyk::color::rgbtoint(128, 0,   0),  //red   
		artyk::color::rgbtoint(128, 0,   128),//violet
		artyk::color::rgbtoint(128, 128, 0),  //yellow
		artyk::color::rgbtoint(172, 172, 172),//white 
		//light
		artyk::color::rgbtoint(86,  86,  86), //black 
		artyk::color::rgbtoint(0,   0,   255),//blue  
		artyk::color::rgbtoint(0,   255, 0),  //green 
		artyk::color::rgbtoint(0,   255, 255),//cyan  
		artyk::color::rgbtoint(255, 0,   0),  //red   
		artyk::color::rgbtoint(255, 0,   255),//violet
		artyk::color::rgbtoint(255, 208, 0),  //yellow
		artyk::color::rgbtoint(255, 255, 255),//white 
	};
	///classic console palette
	constexpr static std::array<uint, 16> cp_conclassic{
		//dark
		artyk::color::rgbtoint(12,  12,  12), //black 
		artyk::color::rgbtoint(0,   55,  218),//blue  
		artyk::color::rgbtoint(19,  161, 14), //green 
		artyk::color::rgbtoint(58,  150, 221),//cyan  
		artyk::color::rgbtoint(197, 15,  31), //red   
		artyk::color::rgbtoint(136, 23,  152),//violet
		artyk::color::rgbtoint(193, 156, 0),  //yellow
		artyk::color::rgbtoint(204, 204, 204),//white 
		//light
		artyk::color::rgbtoint(118, 118, 118),//black 
		artyk::color::rgbtoint(59,  120, 255),//blue  
		artyk::color::rgbtoint(22,  198, 12), //green 
		artyk::color::rgbtoint(97,  214, 214),//cyan  
		artyk::color::rgbtoint(231, 72,  86), //red   
		artyk::color::rgbtoint(180, 0,   158),//violet
		artyk::color::rgbtoint(249, 241, 165),//yellow
		artyk::color::rgbtoint(242, 242, 242),//white 
	};

	///color pallete of 16 colors
	std::array<uint, 16> colorpal;

};