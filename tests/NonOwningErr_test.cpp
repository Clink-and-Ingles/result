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
//    File: NonowningErr_test.cpp
//    Description: Checks to make sure all the template magic in Err.hpp works for non-owning data
//    stucture
//    =================================

#include "Err.hpp"
#include "test.hpp"

#include <string>

void check_NonowningErr_create_for_shared_pointer(void);
void check_NonowningErr_for_proper_get(void);

int main()
{
	check_NonowningErr_create_for_shared_pointer();
	check_NonowningErr_for_proper_get();
	return 0;
}

void check_NonowningErr_create_for_shared_pointer(void)
{
	PrintLn("Beginning of pointer function");
	std::shared_ptr<std::string> vec(new std::string("nonowning ok"));
	for (auto& x : *vec)
	{
		type_name(x);
		x = 1.0;
	}

	NonowningErr<std::string> my_ok(vec);
	COMPILE_TIME_PRINT("\033[01;32mNo error compiling construction of NonowningErr from pointer\033[0m");
	PrintLn("Construct NonowningErr from pointer: \033[01;32m[Passed]\033[0m");
}

void check_NonowningErr_for_proper_get(void)
{
	std::shared_ptr<int>		n(new int);
	NonowningErr<int>			my_ok_1(n);
	[[maybe_unused]] const auto n_ref = my_ok_1.get();
	PrintLn("NonowningErr does proper get and release: \033[01;32m[Passed]\033[0m");
}
