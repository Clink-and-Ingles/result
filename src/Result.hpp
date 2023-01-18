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
#include <ranges>
#include <type_traits>

#include "Assertions.hpp"
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

	OwningResult(OwningOk<T>&& ok) noexcept : m_is_ok{ true },
											  m_is_consumed{ false },
											  m_value{ std::move(ok) },
											  m_err{ VoidErr<E>() }
	{
	}

	OwningResult(OwningErr<E>&& err) noexcept : m_is_ok{ false },
												m_is_consumed{ false },
												m_value{ VoidOk<T>() },
												m_err{ std::move(err) }
	{
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_ok
	/// Returns true if `OwningResult<T, E>` has `OwningOk<T> != VoidOk<T>`
	[[nodiscard]] bool is_ok() { return m_is_ok; }

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_ok_and
	/// Returns true if the result is `OwningOk<T>` and the value inside of it matches a predicate
	[[nodiscard]] bool is_ok_and(std::function<bool(T&)> func)
	{
		if (m_is_ok && func(m_value.get())) return true;
		else return false;
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_err
	/// Returns true `OwningResult<T, E>` has `OwningErr<E> != VoidErr<E>`
	[[nodiscard]] bool is_err() { return !m_is_ok; }

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.is_ok_and
	/// Returns true if the result is `OwningErr<E>` and the value inside of it matches a predicate
	[[nodiscard]] bool is_err_and(std::function<bool(T&)> func)
	{
		if (!m_is_ok && func(m_err.get())) return true;
		else return false;
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.ok
	/// Converts from `OwningResult<T, E>` to `std::option<T>`
	/// Consumes instance of `OwningOk<T>`, discarding the error
	[[nodiscard]] std::optional<T> ok()
	{
		if (m_is_consumed) return std::nullopt;
		if (m_is_ok)
		{
			m_is_consumed = true;
			return std::optional<T>(m_value.release());
		}
		else return std::nullopt;
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.err
	/// Converts from `OwningResult<T, E>` to `std::option<E>`
	/// Consumes instance of `OwningErr<E>`, discarding the error
	[[nodiscard]] std::optional<T> err()
	{
		if (m_is_consumed) return std::nullopt;
		if (!m_is_ok)
		{
			m_is_consumed = true;
			return std::optional<T>(m_err.release());
		}
		else return std::nullopt;
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.as_ref
	/// Converts from `OwningResult<T, E>` to `NonowningResult<T, E>`
	/// This also fulfills the requirements for `Results<T, E>::as_mut` function
	[[nodiscard]] NonowningResult<T, E> as_ref() noexcept { return NonowningResult<T, E>(*this); }

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.map
	/// Maps a `OwningResult<T, E>` to a `OwningResult<U, E>` by applying a function to a
	/// `OwningOk<T>` value, leaving the Err value untouched
	template<typename U>
	[[nodiscard]] OwningResult<U, E> map(std::function<U(T&)>&& func)
	{
		if (m_is_ok)
		{
			auto new_ok = OwninOk<U>(*std::move(func(m_value.get())));
			return OwningResult<U, E>(std::move(new_ok));
		}
		else { return OwningResult<U, E>(m_err); }
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.map_or
	/// Maps generic type `T` to `U`, or returns the default value for `T` if it fails
	/// Arguments passed to `map_or` are eagerly evaluated
	template<typename U>
	[[nodiscard]] U map_or(U default_value, std::function<U(T&)>&& func)
	{
		if (m_is_ok) return func(m_value.get());
		else return default_value;
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.map_or_else
	/// Maps generic type `T` to `U`, or returns the default value for `T` if it fails
	/// Arguments passed to `map_or_else` are eagerly evaluated
	template<typename U>
	[[nodiscard]] U map_or_else(std::function<U(E&)>&& default_mapper, std::function<U(T&)>&& func)
	{
		if (m_is_ok) return func(m_value.get());
		else return default_mapper(m_err.get());
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.map
	/// Maps a `OwningResult<T, E>` to a `OwningResult<T, F>` by applying a function to a
	/// `OwningErr<E>` value, leaving the Err value untouched
	template<typename F>
	[[nodiscard]] OwningResult<T, F> map_err(std::function<F(E&)>&& func)
	{
		if (!m_is_ok)
		{
			auto new_err = OwninErr<F>(*std::move(func(m_err.get())));
			return OwningResult<T, F>(std::move(new_err));
		}
		else { return OwningResult<T, F>(m_value); }
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.inspect
	/// Calls the provide function with a reference to the contained `OwningOk<T>`
	/// In general, this call should not be used to create a new `OwningResult<T, E>`
	/// or `NonwningResult<T, E>` type
	template<typename ReturnType>
	OwningResult<T, E>& inspect(std::function<ReturnType(OwningOk<T>&)> func)
	{
		if (m_is_ok) func(m_value);
		return *this;
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.inspect_err
	/// Calls the provide function with a reference to the contained `OwningErr<E>`
	/// In general, this call should not be used to create a new `OwningResult<T, E>`
	/// or `NonwningResult<T, E>` type
	template<typename ReturnType>
	OwningResult<T, E>& inspect_err(std::function<ReturnType(OwningOk<T>&)> func)
	{
		if (!m_is_ok) func(m_value);
		return *this;
	}

	/// Based on https://doc.rust-lang.org/std/result/enum.Result.html#method.iter
	/// Returns true if `T` is a container with `std::ranges` constraint
	/// Note: This deviates from Rust implementation was return the underlying `T` contained in
	/// a rust iterator, as this will most likely be used for range-based loops
	bool has_range()
	{
		if constexpr (std::ranges::range<T>) return true;
		else return false;
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.expect
	/// Return the contaied `OwningOk<T>` value.
	/// Consumes an instance of `OwningOk<T>`.
	/// Function interrupts execution if `this` is instantiated with `OwningErr<E>` and return
	/// a error message containg `message`; it is preferred for your to use `unwrap_or`,
	/// `unwrap_of_else`, or `unwrap_of_default`.
	T expect(const std::string_view& message)
	{
		ASSERT(!m_is_consumed, message);
		return m_value.release();
	}

	/// https://doc.rust-lang.org/std/result/enum.Result.html#method.unwrap
	/// Return the contaied `OwningOk<T>` value.
	/// Consumes an instance of `OwningOk<T>`.
	/// Function interrupts execution if `this` is instantiated with `OwningErr<E>`; it is
	/// preferred for your to use `unwrap_or`, `unwrap_of_else`, or `unwrap_of_default`.
	T unwrap()
	{
		ASSERT(!m_is_consumed, "");
		return m_value.release();
	}

	private:

	OwningResult() = delete;

	bool		 m_is_ok;
	bool		 m_is_consumed;
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

	NonowningResult(OwningResult<T, E>&& other) noexcept : m_is_ok{ other.m_is_ok },
														   m_value{ std::move(other.m_value) },
														   m_err{ std::move(other.m_err) }
	{
	}

	private:

	bool			m_is_ok;
	NonowningOk<T>	m_value;
	NonowningErr<E> m_err;
};

#endif
