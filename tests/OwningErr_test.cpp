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
//    File: OwningErr_test.cpp
//    Description: Checks to make sure all the template magic in Err.hpp owing Err data struct works
//    =================================

#include "Err.hpp"
#include "test.hpp"

#include <vector>

void check_OwningErr_create_for_value(void);
void check_OwningErr_create_for_pointer(void);
void check_OwningErr_for_proper_get_and_release(void);

int main()
{
	check_OwningErr_create_for_value();
	check_OwningErr_create_for_pointer();
	check_OwningErr_for_proper_get_and_release();
	return 0;
}

void check_OwningErr_create_for_value(void)
{
	std::string vec("owning ok");
	for (auto& x : vec)
		x = 1.0;

	OwningErr<std::string> my_ok(std::move(vec));
	COMPILE_TIME_PRINT("\033[01;32mNo error compiling construction of OwningErr from value\033[0m")
	PrintLn("Construct OwningErr from value: \033[01;32m[Passed]\033[0m");
	PrintLn("End of value function");
}

void check_OwningErr_create_for_pointer(void)
{
	PrintLn("Beginning of pointer function");
	std::string* vec = new std::string("owning ok");
	for (auto& x : *vec)
		x = 1.0;

	OwningErr<std::string*> my_ok(std::move(vec));
	COMPILE_TIME_PRINT("\033[01;32mNo error compiling construction of OwningErr from pointer\033[0m");
	PrintLn("Construct OwningErr from pointer: \033[01;32m[Passed]\033[0m");
}

void check_OwningErr_for_proper_get_and_release(void)
{
	int							n = 10;
	OwningErr<int>				my_ok_1(std::move(n));
	[[maybe_unused]] const auto n_ref = my_ok_1.get();
	[[maybe_unused]] int*		n_ptr = my_ok_1.release();
	PrintLn("OwningErr does proper get and release: \033[01;32m[Passed]\033[0m");
}
