//    Copyright (C) 2022  Liam Clink and Kevin Ingles
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// =================================
// Author: Kevin Ingles
// File: Result.hpp
// Description: Contains the class Result<T, E> and all of its functionality
// =================================
//

#ifndef OL_ASSERTION_HPP
#define OL_ASSERTION_HPP

#include <iostream>

#define ASSERT(condition, message)                                                                \
  {                                                                                               \
	if (!(condition))                                                                             \
	{                                                                                             \
	  std::cerr << "Assertion " << #condition << "failed in " << __FILE__ << " line " << __LINE__ \
				<< ": " << message << "\n";                                                       \
	  std::terminate();                                                                           \
	}                                                                                             \
  }

#endif

