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

#ifndef OL_RESULT_HPP
#define OL_RESULT_HPP

#include <functional>
#include <memory>
#include <optional>
#include <type_traits>

#include "Err.hpp"
#include "Ok.hpp"

// Forward declarations
template<typename T, typename E>
class OwningResult;
template<typename T, typename E>
class NonowningResult;

// Ownership in rust is very clear, but in C++ we have to spell it out.
// This class takes ownership of a pointer or reference passed.
// This means that the passed pointer of reference is NULL after the function call.

/// OwningResult employes the OwningOk and OwningErr data structures which take r-value references
/// only.
template<typename T, typename E>
class OwningResult
{
	friend NonowningResult<T, E>;

	public:

	OwningResult(OwningOk<T> ok)
		: m_is_ok{ true },
		  m_value{ ok },
		  m_err{ VoidErr<E>() }
	{
	}

	OwningResult(OwningErr<T> err)
		: m_is_ok{ false },
		  m_value{ VoidOk<T>() },
		  m_err{ err }
	{
	}

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_ok
	// Returns true OwningResult is OwningOk
	[[nodiscard]] bool is_ok() { return m_is_ok; }

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_ok_and
	// Returns true if the result if OwningOk and the value inside of it matches a predicate
	[[nodiscard]] bool is_ok_and(std::function<bool(T&)> func)
	{
		if (m_is_ok && func(m_value.get())) return true;
		else return false;
	}

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_err
	// Returns true OwningResult is OwningErr
	[[nodiscard]] bool is_err() { return !m_is_ok; }

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_ok_and
	// Returns true if the result if OwningErr and the value inside of it matches a predicate
	[[nodiscard]] bool is_err_and(std::function<bool(T&)> func)
	{
		if (!m_is_ok && func(m_err.get())) return true;
		else return false;
	}

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.ok
	// Converts from OwningResult to std::option<T>
	// Consumes instance of OwningOk<T>, discarding the error
	[[nodiscard]] std::optional<T> ok()
	{
		if (m_is_ok) return std::optional<T>(m_value.release());
		else return std::nullopt;
	}

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.err
	// Converts from OwningResult to std::option<E>
	// Consumes instance of OwningOk<E>, discarding the error
	[[nodiscard]] std::optional<T> err()
	{
		if (!m_is_ok) return std::optional<T>(m_err.release());
		else return std::nullopt;
	}

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.as_ref
	// Converts from OwningResult to std::option<E>
	// Consumes instance of OwningOk<E>, discarding the error
	// This also fulfills the requirements for Results<T, E>::as_mut function
	[[nodiscard]] NonowningResult<T, E> as_ref() { return NonowningResult<T, E>(*this); }

	// https://doc.rust-lang.org/std/result/enum.Result.html#method.map
	// Maps a OwningResult<T, E> to a OwningResult<U, E> by applying a function to a Ok value,
	// leaving the Err value untouched
	template<typename U>
	[[nodiscard]] OwningResult<U, E> map(std::function<U(T&)> func)
	{
		if (m_is_ok)
		{
			auto new_ok = OwninOk<U>(func(m_value.get()));
			return OwningResult<U, E>(new_ok);
		}
		else { return OwningResult<U, E>(m_err); }
	}

	private:

	bool		 m_is_ok;
	OwningOk<T>	 m_value;
	OwningErr<E> m_err;
};

/// NonowningResult employes the NonowningOk and NonowningErr data structures.
/// These structures only take share_ptrs, so be sure to instantiate with shared pointers
template<typename T, typename E>
class NonowningResult
{
	friend OwningResult<T, E>;

	public:

	NonowningResult(NonowningOk<T> ok)
		: m_is_ok{ true },
		  m_value{ ok },
		  m_err{ VoidErr<E>() }
	{
	}

	NonowningResult(NonowningErr<T> err)
		: m_is_ok{ false },
		  m_value{ VoidOk<T>() },
		  m_err{ err }
	{
	}

	NonowningResult(OwningResult<T, E> other)
		: m_is_ok{ other.m_is_ok },
		  m_value{ std::shared_ptr<T>(std::move(other.m_value)) },
		  m_err{ std::shared_ptr<E>(std::move(other.m_err)) }
	{
	}

	private:

	bool			m_is_ok;
	NonowningOk<T>	m_value;
	NonowningErr<E> m_err;
};

#endif
