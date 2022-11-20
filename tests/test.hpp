//  Copyright 2021-2022 Liam Clink and Kevin Ingles
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the right to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Sofware is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial poritions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OF OTHER DEALINGS IN THE SOFTWARE
//
//  ==================================
//  Author: Kevin Ingles
//  File: test.hpp
//  Description: Contains the main preamble to run tests
//  ==================================

#ifdef __clang__
#  pragma message("Hello from clang compilation")
#elif defined(__GNUC__)
#  pragma message("Hello from GNU compilation")
#endif

#define STRINGIFY(x) #x
#define COMPILE_TIME_PRINT(x) _Pragma(STRINGIFY(message(x)))

#include <iostream>
#include <string_view>

template<typename... Args>
void Print(Args&&... args)
{
	((std::cout << std::forward<Args>(args) << " "), ...);
}

template<typename... Args>
void PrintLn(Args&&... args)
{
	((std::cout << std::forward<Args>(args) << " "), ...);
	std::cout << "\n";
}

template<typename T>
constexpr auto type_name(T)
{
	// Note this only works for
	std::string_view name = __PRETTY_FUNCTION__;
#ifdef __GNUC__
	std::string_view prefix = "constexpr auto type_name() [with T = ";
#elif defined(__clang__)
	std::string_view prefix = "auto type_name() [T = ";
#endif
	std::string_view suffix = "]";
	name.remove_prefix(prefix.size());
	name.remove_suffix(suffix.size());
	return name;
}
