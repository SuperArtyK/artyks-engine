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

/** @file include/engine/vector.hpp
 *  This file contains the engine vector's defitions and declarations(math vector, not std::vector)
 *
 *  Should not cause everything to break.
 */

#pragma once

#ifndef VECTORS_HPP
#define VECTORS_HPP

#include "engine_math.hpp"


template<typename T, const unsigned int diramount>
/// <summary>
/// The Vector type for engine
/// </summary>
/// <typeparam name="T">data type used for the </typeparam>
struct aevector{
    
    
    ///dimensions of a vector
	T dims[diramount] {};
	
	///returns zero'ed vector
	constexpr static aevector<T, diramount> zero(){
	    return aevector<T, diramount>{};
	}

	///returns magnitude of a vector
	constexpr long double magnitude() const {
		long double	 temp = 0;
		for (size_t i = 0; i < (sizeof(this->dims)/sizeof(this->dims[0]));i++) {
			temp += this->dims[i] * this->dims[i];
		}
		
		return artyk::math::ldsqrt(temp);
	}

	///\brief returns vector's dimension, non const accessor.
	///It is more convenient to use that(sometimes) instead of the aevector::dims[]
	constexpr T& operator[](const long long index){
	    return this->dims[
	           #ifdef AE_VECTOR_WRAP_DIMENSIONS
	               index%(sizeof(dims)/sizeof(dims[0]))
	           #else
	               index
	           #endif
	                ];
	}

	///\brief returns vector's dimension, const accessor.
	///It is more convenient to use that(sometimes) instead of the aevector::dims[]
	constexpr const T& operator[](const long long index) const {
		return this->dims[
#ifdef AE_VECTOR_WRAP_DIMENSIONS
			index % (sizeof(dims) / sizeof(dims[0]))
#else
			index
#endif
		];
	}

	///checks if the vectors are equal
	constexpr bool operator==(const aevector& another) const {
		if( (sizeof(this->dims)/sizeof(this->dims[0])) 
		        != 
		    (sizeof(another.dims)/sizeof(another.dims[0]))
		  ){
		    return false;
		    
		}
		for(size_t i = 0; i < (sizeof(this->dims)/sizeof(this->dims[0])); i++){
		    if(this->dims[i] != another.dims[i]){
		        return false;
		    }
		}
        return true;
	}
    
	///checks if the vectors are not equal
	constexpr bool operator!=(const aevector& another) const {
		return !(operator==(another));
	}
    


	
};





#endif // !VECTORS_HPP

