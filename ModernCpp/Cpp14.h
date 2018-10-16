#pragma once

#ifndef __MODERN_CPP_14_H
#define __MODERN_CPP_14_H

#include "Cpp11.h"

#include <regex>
#include <iostream>
#include <mutex>  // For std::unique_lock
#include <shared_mutex>
#include <thread>

using namespace std;

// https://www.learncpp.com/cpp-tutorial/b-2-introduction-to-c14/

namespace MODERNCPP
{
	// for constant expression
	template< class T >
	constexpr const T& max(const T& a, const T& b);			// (C++14)

	// variable template
	template<typename T>
	constexpr T pi = T(3.141592653589793238462643383L);		// variable template

	// Usual specialization rules apply:
	template<>
	constexpr const char* pi<const char*> = "pi";

	template<class T>
	T circular_area(T r) // function template
	{
		return pi<T> * r * r; // pi<T> is a variable template instantiation
	}

	struct Rectangle
	{
		double length = 1.0; // non-static member initialization
		double width = 1.0;
	};

	class Fraction {		// for make_unique
	private:
		int m_numerator = 0;
		int m_denominator = 1;

	public:
		Fraction(int numerator = 0, int denominator = 1) :
			m_numerator(numerator), m_denominator(denominator) {}

		friend std::ostream& operator<<(std::ostream& out, const Fraction &f1)
		{
			out << f1.m_numerator << "/" << f1.m_denominator;
			return out;
		}
	};

	// Shared mutexes and locking
	class ThreadSafeCounter {
	public:
		ThreadSafeCounter() = default;

		// Multiple threads/readers can read the counter's value at the same time.
		unsigned int get() const {
			std::shared_lock<std::shared_mutex> lock(mutex_);
			return value_;
		}

		// Only one thread/writer can increment/write the counter's value.
		void increment() {
			std::unique_lock<std::shared_mutex> lock(mutex_);
			value_++;
		}

		// Only one thread/writer can reset/write the counter's value.
		void reset() {
			std::unique_lock<std::shared_mutex> lock(mutex_);
			value_ = 0;
		}

	private:
		mutable std::shared_mutex mutex_;
		unsigned int value_ = 0;
	};

	class Cpp14 : public Base {
	public:

		explicit Cpp14() noexcept {}			// cannot called implicitly

		// auto return type deduction
		template<class T, class U>
		auto ReturnTypeDeduction(T const& lhs, U const& rhs) {
			return lhs * rhs;
		}

		void AggregateMemberInitialization();
		void BinaryLiterals();
		void DigitSeparators();
		void GenericLambdaExpr();
		void LambdaCaptureExpr();
		void ConstantExprRestriction();
		void VariableTemplate();
		void TupleAddressingByType();
		void MakeUnique();
		void HeterogeneousLookupAssocContainers();
		void SharedMutexesLocking();

		//[[deprecated("this method is deprecated")]]
		void DeprecatedMethod();

	private:

		// initialize
		Rectangle x{ 2.0, 2.0 }; // uniform initialization (has precedence)
	};
}

#endif
