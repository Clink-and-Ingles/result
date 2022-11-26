//  Copyright 2021-2022 Liam Clink and Kevin Ingles
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
//  File: LazilyEvaluate.hpp
//  Description: Helper class that allows for lazy evaluation of functions.
//               The lazily evaluated functions tend to be called `thunks` in computer science, so
//               we will stick with this terminology.

#ifndef OL_LAZILY_EVALUATE_HPP
#define OL_LAZILY_EVALUATE_HPP

#include <functional>

// The general API is intended to take a function without any arguments as to avoid having to
// store or create any copies of "by value" variables.
// The simplest way to do this is to pass a Lambda function that captures the values it needs
// by reference.
template<class ReturnType>
class Thunk
{
	public:

	Thunk(std::function<ReturnType()>&& func)
		: m_func{ func },
		  m_evaluated{ false }
	{
	}

	ReturnType operator()()
	{
		evaluate();
		return m_return_value;
	}

	private:

	void evaluate()
	{
		if (!m_evaluated)
		{
			m_return_value = m_func();
			m_evaluated	   = true;
		}
	}

	std::function<ReturnType()> m_func;
	bool						m_evaluated;
	ReturnType					m_return_value;
};

#endif
