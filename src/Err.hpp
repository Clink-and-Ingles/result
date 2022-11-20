//  Copyright 2021-2022 Kevin Ingles
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
//  File: Err.hpp
//  Description: Data structure which returns the error type of result data structure
//  Note: This is duplicated in/from Ok.hpp.
//        Some may have qualms with this duplication, however, this duplication
//        is intentional to facilitate documentation
//  ==================================

#include <memory>
#include <type_traits>

/// Generic empty struct that can be used to zero initialize the Err classes
template<typename T>
struct VoidErr {
};

// Ownership in rust is very clear, but in C++ we have to spell it out.
// This class takes ownership of a pointer or reference passed.
// This means that the passed pointer of reference is NULL after the function call.

/// OwningErr takes a value or pointer and moves as a r-value.
/// It is then assumed that the instance of OwningErr is the only owner of the passed object.
/// To ensure this, be sure to use smart pointers in your code to make it obvious to the compiler
/// and the user whether the passed objects should owned our not.
template<typename T>
class OwningErr
{
	public:

	using underlying_type = typename std::remove_pointer<typename std::decay<T>::type>::type;

	OwningErr() = default;

	OwningErr(T&& value) noexcept
	{
		if constexpr (std::is_pointer<T>::value)
		{
			m_stored_value = std::make_unique<underlying_type>();
			m_stored_value.reset(value);
			value = nullptr;
		}
		else { m_stored_value = std::make_unique<underlying_type>(value); }
	}

	template<typename U>
	OwningErr(OwningErr<U>&& ok) noexcept : m_stored_value{ std::move(ok.m_stored_value) }
	{
	}

	OwningErr(VoidErr<T>) noexcept : m_stored_value{} {}

	[[nodiscard]] underlying_type& get(void) { return *m_stored_value.release(); }

	[[nodiscard]] underlying_type* release(void) { return m_stored_value.release(); }

	private:

	// pointer to stored information
	std::unique_ptr<underlying_type> m_stored_value;
};

/// NonowningErr only takes by reference and only stores a reference.
/// The user should ensure that the lifetime of the object does not terminate before the instance
/// of the NonowningErr has terminated, otherwise you would be accessing a nullptr
template<typename T>
class NonowningErr
{
	public:

	// TODO: Implement for reference, but make sure to reference count some_how
	using underlying_type = typename std::remove_pointer<typename std::decay<T>::type>::type;

	NonowningErr() = default;

	NonowningErr(std::shared_ptr<T> ptr) noexcept : m_stored_value{ std::weak_ptr(ptr) } {}

	template<typename U>
	NonowningErr(NonowningErr<U>&& ok) noexcept : m_stored_value{ std::weak_ptr(ok.m_stored_value) }
	{
	}

	NonowningErr(VoidErr<T>) noexcept : m_stored_value{} {}

	[[nodiscard]] underlying_type& get(void) { return *m_stored_value.lock().get(); }

	private:

	// pointer to stored information
	std::weak_ptr<underlying_type> m_stored_value;
};

using Err = NonowningErr<void>;
