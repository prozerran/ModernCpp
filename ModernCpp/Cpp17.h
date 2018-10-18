#pragma once

#ifndef __MODERN_CPP_17_H
#define __MODERN_CPP_17_H

#include "Cpp11.h"

// C++17 __has_include
#ifdef __has_include                           // Check if __has_include is present
#  if __has_include(<optional>)                // Check for a standard library
#    include <optional>
#  elif __has_include(<experimental/optional>) // Check for an experimental version
#    include <experimental/optional>
#  elif __has_include(<boost/optional.hpp>)    // Try with an external library
#    include <boost/optional.hpp>
#  else                                        // Not found at all
#     error "Missing <optional>"
#  endif
#endif

#ifdef __has_cpp_attribute                      // Check if __has_cpp_attribute is present
#  if __has_cpp_attribute(deprecated)           // Check for an attribute
#    define DEPRECATED(msg) [[deprecated(msg)]]
#  endif
#endif
#ifndef DEPRECATED
#    define DEPRECATED(msg)
#endif

using namespace std;

// https://www.learncpp.com/cpp-tutorial/b-2-introduction-to-c17/

namespace MODERNCPP::CPP17		// C++17, nested namespace
{
	class Cpp17 : public Base {
	public:

		explicit Cpp17() noexcept {}			// cannot called implicitly

		// auto return type deduction
		template<typename T, typename U, typename V>
		auto TemplateArgumentDeduction(T const& p_char, U const& p_int, V const& p_bool) {
			return std::tuple(p_char, p_int, p_bool);  // Auto type deduction from arguments
		}

		// using typename vs class, compile time if
		template <typename T>
		auto CompileTimeIf(T t) {
			if constexpr (std::is_pointer_v<T>)
				return *t; // deduces return type to int for T = int*
			else
				return t;  // deduces return type to int for T = int
		}

		DEPRECATED("Method has been deprecated")
		void DeprecatedMethod() {};

		long Preprocessor(unsigned);
		void StaticAssert();
		void StdAny();
		void StdByte();
		void StdFileSystem();					// alternative to boost filesystetm
		void StdOptional();
		void StdSharedPtr() = delete;			// not fully supported until C++20
		void StdVariant();
		void StdConjunction();
		void StdDisjunction();
		void StdNegation();
		void StdUncaughtExceptions();
		void StructuredBindingDeclaration();
		void IfSwitchInitializer();
		void InlineVariable();
		void FoldExpression();
		void RemovedTrigraphs();				// unable to output special characters
		void Utf8Literals();
		void TemplateDeductionConstructor();

	private:

		// optional can be used as the return type of a factory that may fail
		std::optional<std::string> create1(bool b) {
			if (b)
				return "Godzilla";
			return {};
		}

		// std::nullopt can be used to create any (empty) std::optional
		auto create2(bool b) {
			return b ? std::optional<std::string>{"Godzilla"} : std::nullopt;
		}

		// std::reference_wrapper may be used to return a reference
		auto create_ref(bool b) {
			static std::string value = "Godzilla";
			return b ? std::optional<std::reference_wrapper<std::string>>{value}
			: std::nullopt;
		}

		// As of C++17, a structured binding declaration can be used to simplify splitting multiple
		// returned values into separate variables:
		std::tuple<int, double> returnTuple() // return a tuple that contains an int and a double
		{
			return std::make_tuple(5, 6.7); // use std::make_tuple() as shortcut to make a tuple to return
		}

		// Fold Expressions
		template<typename ...Args> auto sum1(Args ...args)
		{
			return (args + ... + 0);
		}

		template<typename ...Args> auto sum2(Args ...args)
		{
			return (args + ...);
		}

		template<typename ...Args>
		void FoldPrint(Args&&... args) {
			(std::cout << ... << forward<Args>(args)) << '\n';
		}

		template<typename T, typename... Args>
		void FoldPushBack(vector<T>& v, Args&&... args)
		{
			(v.push_back(args), ...);
		}

		// conjunction
		// func is enabled if all Ts... have the same type as T
		template<typename T, typename... Ts>
		std::enable_if_t<std::conjunction_v<std::is_same<T, Ts>...>>
			func(T, Ts...) {
			std::cout << "all types in pack are T\n";
		}

		// otherwise
		template<typename T, typename... Ts>
		std::enable_if_t<!std::conjunction_v<std::is_same<T, Ts>...>>
			func(T, Ts...) {
			std::cout << "not all types in pack are T\n";
		}

	private:

		inline static const std::string inline_variable = "This is an inline variable!";

	};
}

#endif
