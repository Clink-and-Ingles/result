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
//    File: Ok.hpp
//    Description: Provides a tyoe to return from functions and easily construct
//    a Result type
//    =================================

#include <memory>
#include <type_traits>

/// General struct for returing Ok values for functions that initialize a
/// Results object
template <typename T>
struct Ok {
	T stored_value;
};

/// Ownership in rust is very clear, but in C++ we have to spell it out. This
/// class takes owndership of a pointer or reference passed. This means that the
/// passed pointer of reference is NULL after the function call.
///
/// Since the oject is essentially "moved", const constructors should be
/// disabled
template <typename T>
struct OwningOk : public Ok<T> {
	using underlying_type = typename std::remove_pointer<typename std::decay<T>::value>::value;

	OwningOk()		   = default;
	OwningOk(const T&) = delete;

	OwningOk(T value)
	{
		if constexpr (std::is_pointer<T>::value)
		{
			stored_value = std::make_unique<underlying_type>();
			stored_value.reset(value);
			value			 = nullptr;
			m_mark_to_delete = true;
		}
		else if constexpr (std::is_reference<T>::value)
		{
			stored_value	 = std::make_unique(value);
			&value			 = nullptr;
			m_mark_to_delete = true;
		}
	}

	OwningOk(T&& value)
	{
		if constexpr (std::is_pointer<T>::value)
		{
			stored_value	 = new T();
			*stored_value	 = *value;
			value			 = nullptr;
			m_mark_to_delete = true;
		}
		else if constexpr (std::is_reference<T>::value)
		{
			stored_value	 = new T();
			*stored_value	 = value;
			&value			 = nullptr;
			m_mark_to_delete = true;
		}
	}

	template <typename U>
	OwningOk(Ok<U>&& ok);	 // Check that udnerlying types are the same

	~OwningOk()
	{
		if (m_mark_to_delete) delete stored_value;
	}

	std::unique_ptr<underlying_type> stored_value;

	private:

	bool m_mark_to_delete;
};
