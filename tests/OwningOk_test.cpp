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
//    File: OwningOk_test.cpp
//    Description: Checks to make sure all the template magic in Ok.hpp owing Ok data struct works
//    =================================

#include "Ok.hpp"
#include "test.hpp"

#include <vector>

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
	std::string vec("owning ok");
	for (auto& x : vec)
		x = 1.0;

	OwningOk<std::string> my_ok(std::move(vec));
	COMPILE_TIME_PRINT("\033[01;32mNo error compiling construction of OwningOk from value\033[0m")
	PrintLn("Construct OwningOk from value: \033[01;32m[Passed]\033[0m");
	PrintLn("End of value function");
}

void check_OwningOk_create_for_pointer(void)
{
	PrintLn("Beginning of pointer function");
	std::string* vec = new std::string("owning ok");
	for (auto& x : *vec)
		x = 1.0;

	OwningOk<std::string*> my_ok(std::move(vec));
	COMPILE_TIME_PRINT("\033[01;32mNo error compiling construction of OwningOk from pointer\033[0m");
	PrintLn("Construct OwningOk from pointer: \033[01;32m[Passed]\033[0m");
}

void check_OwningOk_for_proper_get_and_release(void)
{
	int							n = 10;
	OwningOk<int>				my_ok_1(std::move(n));
	[[maybe_unused]] const auto n_ref = my_ok_1.get();
	[[maybe_unused]] int*		n_ptr = my_ok_1.release();
	PrintLn("OwningOk does proper get and release: \033[01;32m[Passed]\033[0m");
}
