//
//    result is a header-only implementation of the rust result crate
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
//    =================================
//    Author: Kevin Ingles
//    File: Ok_struct_works.cpp
//    Description: Checks to make sure all the template mahic in Ok.hpp works
//    =================================
//
#ifdef __clang__
#  pragma message("Hello from clang compilation")
#elif defined(__GNUC__)
#  pragma message("Hello from GNU compilation")
#endif

#define STRINGIFY(x) #x
#define COMPILE_TIME_PRINT(x) _Pragma(STRINGIFY(message(x)))

#include <iostream>
#include <string_view>
#include <vector>

#include "Ok.hpp"

template <typename... Args>
void Print(Args&&... args)
{
	((std::cout << std::forward<Args>(args) << " "), ...);
}

template <typename... Args>
void PrintLn(Args&&... args)
{
	((std::cout << std::forward<Args>(args) << " "), ...);
	std::cout << "\n";
}

template <typename T>
constexpr auto type_name()
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

void check_OwningOk_create_for_value(void);
void check_OwningOk_create_for_pointer(void);
void check_OwningOk_for_proper_get_and_release(void);

int main()
{
	check_OwningOk_create_for_value();
	check_OwningOk_create_for_pointer();
	check_OwningOk_for_proper_get_and_release();
	return 0;
}

void check_OwningOk_create_for_value(void)
{
	std::vector<double> vec(10);
	for (auto& x : vec)
		x = 1.0;

	OwningOk<std::vector<double> > my_ok(std::move(vec));
	COMPILE_TIME_PRINT("\e[01;32mNo error compiling construction of OwningOk from value\e[0m")
	PrintLn("Construct OwningOk from value: \e[01;32m[Passed]\e[0m");
	PrintLn("End of value function");
}

void check_OwningOk_create_for_pointer(void)
{
	PrintLn("Beginning of pointer function");
	std::vector<double>* vec = new std::vector<double>(10);
	for (auto& x : *vec)
		x = 1.0;

	OwningOk<std::vector<double>*> my_ok(std::move(vec));
	COMPILE_TIME_PRINT("\e[01;32mNo error compiling construction of OwningOk from pointer\e[0m");
	PrintLn("Construct OwningOk from pointer: \e[01;32m[Passed]\e[0m");
}

void check_OwningOk_for_proper_get_and_release(void)
{
	int							n = 10;
	OwningOk<int>				my_ok_1(std::move(n));
	[[maybe_unused]] const auto n_ref = my_ok_1.get();
	[[maybe_unused]] int*		n_ptr = my_ok_1.release();
	PrintLn("OwningOk does proper get and release: \e[01;32m[Passed]\e[0m");
}
