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
//    File: NonowningOk_test.cpp
//    Description: Checks to make sure all the template magic in Ok.hpp works for non-owning data
//    stucture
//    =================================

#include "Ok.hpp"
#include "test.hpp"

#include <string>

void check_NonowningOk_create_for_shared_pointer(void);
void check_NonowningOk_for_proper_get(void);

int main()
{
	check_NonowningOk_create_for_shared_pointer();
	check_NonowningOk_for_proper_get();
	return 0;
}

void check_NonowningOk_create_for_shared_pointer(void)
{
	PrintLn("Beginning of pointer function");
	std::shared_ptr<std::string> vec(new std::string("nonowning ok"));
	for (auto& x : *vec)
	{
		type_name(x);
		x = 1.0;
	}

	NonowningOk<std::string> my_ok(vec);
	COMPILE_TIME_PRINT("\033[01;32mNo error compiling construction of NonowningOk from pointer\033[0m");
	PrintLn("Construct NonowningOk from pointer: \033[01;32m[Passed]\033[0m");
}

void check_NonowningOk_for_proper_get(void)
{
	std::shared_ptr<int>		n(new int);
	NonowningOk<int>			my_ok_1(n);
	[[maybe_unused]] const auto n_ref = my_ok_1.get();
	PrintLn("NonowningOk does proper get and release: \033[01;32m[Passed]\033[0m");
}
