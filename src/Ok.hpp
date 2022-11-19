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

// Ownership in rust is very clear, but in C++ we have to spell it out. This
// class takes owndership of a pointer or reference passed. This means that the
// passed pointer of reference is NULL after the function call.
//
// Since the oject is essentially "moved", const constructors should be
// disabled

/// OwningOk takes a value or poitner and moves as a r-value.
/// It is then assumed that the instance of OwningOk is the only owner of the passed object.
/// To ensure this, be sure to use smart pointers in your code to make it obvious to the compiler
/// and the user whether the passed objects should owned our not.
template <typename T>
class OwningOk
{
	public:

	using underlying_type = typename std::remove_pointer<typename std::decay<T>::type>::type;

	OwningOk() = default;

	OwningOk(T&& value) noexcept
	{
		if constexpr (std::is_pointer<T>::value)
		{
			m_stored_value = std::make_unique<underlying_type>();
			m_stored_value.reset(value);
			value = nullptr;
		}
		else { m_stored_value = std::make_unique<underlying_type>(value); }
	}

	template <typename U>
	OwningOk(OwningOk<U>&& ok) noexcept : m_stored_value{ std::move(ok.m_stored_value) }
	{
	}

	[[nodiscard]] underlying_type& get(void) { return *m_stored_value.release(); }

	[[nodiscard]] underlying_type* release(void) { return m_stored_value.release(); }

	private:

	// pointer to stored information
	std::unique_ptr<underlying_type> m_stored_value;
};

/// NonowningOk only takes by reference and only stores a reference.
/// The user should ensure that the lifetime of the object does not terminate before the instance
/// of the NonowningOk has terminated, otherwise you would be accessing a nullptr
template <typename T>
class NonowningOk
{
	public:

	// TODO: Implement for reference, but make sure to reference count some_how
	using underlying_type = typename std::remove_pointer<typename std::decay<T>::type>::type;

	NonowningOk() = default;

	NonowningOk(std::shared_ptr<T> ptr) noexcept : m_stored_value{ std::weak_ptr(ptr) } {}

	template <typename U>
	NonowningOk(NonowningOk<U>&& ok) noexcept : m_stored_value{ std::weak_ptr(ok.m_stored_value) }
	{
	}

	[[nodiscard]] underlying_type& get(void) { return *m_stored_value.lock().get(); }

	private:

	// pointer to stored information
	std::weak_ptr<underlying_type> m_stored_value;
};
