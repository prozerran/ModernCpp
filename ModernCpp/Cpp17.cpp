#include "pch.h"
#include "Cpp17.h"

#include <string>
#include <cassert>
#include <iostream>
#include <algorithm>
#include <any>
#include <iostream>
#include <filesystem>
#include <variant>
#include <exception>
#include <stdexcept>
#include <cmath>

//#define NDEBUG		// disable assert() for release

namespace MODERNCPP::CPP17
{
	long Cpp17::Preprocessor(unsigned i = 0b00)
	{
#if __cpp_binary_literals                    // Check for the presence of a feature
		unsigned mask1 = 0b11000000;
		unsigned mask2 = 0b00000111;
#else
		unsigned mask1 = 0xC0;
		unsigned mask2 = 0x07;
#endif
		if (i & mask1)
			return 1;
		if (i & mask2)
			return 2;
		return 0;
	}

	void Cpp17::StaticAssert()
	{
		assert(true && "Could not be found in database");		// C++98

		//static_assert(sizeof(long) == 8, "long must be 8 bytes");
		static_assert(sizeof(int) == 4, "int must be 4 bytes");		// C++11
		static_assert(sizeof(int) == 4);							// C++17, no need message
	}

	void Cpp17::StdAny()
	{
		std::cout << std::boolalpha;

		// any type
		std::any a = 1;
		std::cout << a.type().name() << ": " << std::any_cast<int>(a) << '\n';
		a = 3.14;
		std::cout << a.type().name() << ": " << std::any_cast<double>(a) << '\n';
		a = true;
		std::cout << a.type().name() << ": " << std::any_cast<bool>(a) << '\n';

		// bad cast
		try
		{
			a = 1;
			std::cout << std::any_cast<float>(a) << '\n';
		}
		catch (const std::bad_any_cast& e)
		{
			std::cout << e.what() << '\n';
		}

		// has value
		a = 1;
		if (a.has_value())
		{
			std::cout << a.type().name() << '\n';
		}

		// reset
		a.reset();
		if (!a.has_value())
		{
			std::cout << "no value\n";
		}

		// pointer to contained data
		a = 1;
		int* i = std::any_cast<int>(&a);
		std::cout << *i << "\n";
	}

	void Cpp17::StdByte()
	{
		int n = 7;
		std::byte b = (std::byte) n;

		int x = std::to_integer<int>(b);
		std::cout << x << endl;
	}

	void Cpp17::StdFileSystem()
	{
		namespace fs = std::filesystem;	
		fs::path p = "C:/a/b/c/d/e/f";

		if (!fs::exists(p))
		{
			fs::create_directories(p);
			fs::remove_all("C:/a/");
		}
		std::cout << fs::current_path();
	}

	void Cpp17::StdOptional()
	{
		std::cout << "create1(false) returned "
			<< create1(false).value_or("empty") << '\n';

		// optional-returning factory functions are usable as conditions of while and if
		if (auto str = create2(true)) {
			std::cout << "create2(true) returned " << *str << '\n';
		}

		if (auto str = create_ref(true)) {
			// using get() to access the reference_wrapper's value
			std::cout << "create_ref(true) returned " << str->get() << '\n';
			str->get() = "Mothra";
			std::cout << "modifying it changed it to " << str->get() << '\n';
		}
	}

	void Cpp17::StdVariant()
	{
		std::variant<int, float> v, w;
		v = 12; // v contains int
		int i = std::get<int>(v);
		w = std::get<int>(v);
		w = std::get<0>(v);		// same effect as the previous line
		w = v;					// same effect as the previous line

	//  std::get<double>(v);	// error: no double in [int, float]
	//  std::get<3>(v);			// error: valid index values are 0 and 1

		try {
			std::get<float>(w); // w contains int, not float: will throw
		}
		catch (const std::bad_variant_access&) {}

		std::variant<std::string> x("abc"); // converting constructors work when unambiguous
		x = "def"; // converting assignment also works when unambiguous

		std::variant<std::string, bool> y("abc"); // casts to bool when passed a char const *
		assert(std::holds_alternative<bool>(y)); // succeeds
		y = "xyz"s;
		assert(std::holds_alternative<std::string>(y)); //succeeds
	}

	void Cpp17::StdConjunction()
	{
		func(1, 2, 3);
		func(1, 2, "hello!");
	}

	void Cpp17::StdDisjunction()
	{
		using result0 =
			std::disjunction<std::bool_constant<false>, std::bool_constant<false>,
			std::bool_constant<false>>;
		using result1 =
			std::disjunction<std::bool_constant<true>, std::bool_constant<false>,
			std::bool_constant<false>>;

		std::cout << std::boolalpha;
		std::cout << result0::value << '\n';
		std::cout << result1::value << '\n';
	}

	void Cpp17::StdNegation()
	{
		static_assert(
			std::is_same<
			std::bool_constant<false>,
			typename std::negation<std::bool_constant<true>>::type>::value,
			"");
		static_assert(
			std::is_same<
			std::bool_constant<true>,
			typename std::negation<std::bool_constant<false>>::type>::value,
			"");

		std::cout << std::boolalpha;
		std::cout << std::negation<std::bool_constant<true>>::value << '\n';
		std::cout << std::negation<std::bool_constant<false>>::value << '\n';
	}

	void Cpp17::StdUncaughtExceptions()
	{
		struct Foo {
			int count = std::uncaught_exceptions();
			~Foo() {
				std::cout << (count == std::uncaught_exceptions()
					? "~Foo() called normally\n"
					: "~Foo() called during stack unwinding\n");
			}
		};

		Foo f;
		try {
			Foo f;
			std::cout << "Exception thrown\n";
			throw std::runtime_error("test exception");
		}
		catch (const std::exception& e) {
			std::cout << "Exception caught: " << e.what() << '\n';
		}
	}

	void Cpp17::StructuredBindingDeclaration()
	{
		// used structured binding declaration to put results of tuple in variables a and b
		auto[a, b] = returnTuple(); 

		std::cout << a << ' ' << b << '\n';
	}

	void Cpp17::IfSwitchInitializer()
	{
		auto getval = [value = 1]{ return value; };

		if (auto a = getval(); a < 10) {
			std::cout << a << endl;
		}

		switch (auto b = getval(); b) {
		case 1:
			std::cout << b << endl;
			break;
		}
	}

	void Cpp17::InlineVariable()
	{
		std::cout << inline_variable << endl;
	}

	void Cpp17::FoldExpression()
	{
		cout << sum1(1, 2, 3, 4, 5, 6, 7) << "\n";
		cout << sum2(1, 2, 3, 4, 5, 6, 7) << "\n";

		FoldPrint("hello", ", ", 10, ", ", 90.0);

		std::vector<int> v;
		FoldPushBack(v, 1, 2, 3, 4);

		for (auto &i : v)
			std::cout << i << "\n";
	}

	void Cpp17::RemovedTrigraphs()
	{
		std::cout << "??=" << " ";		// #
		std::cout << "??//" << " ";		// /
		std::cout << "??'" << " ";		// ^
		std::cout << "??)" << " ";		// ]
		std::cout << "??(" << " ";		// [
		std::cout << "??!" << " ";		// |
		std::cout << "??<" << " ";		// {
		std::cout << "??>" << " ";		// }
		std::cout << "??-" << " ";		// ~
		std::cout << std::endl;
	}

	void Cpp17::Utf8Literals()
	{
		auto h = u8"Hello World";
		std::cout << h << endl;
	}

	void Cpp17::HexFloatingPointLiterals()
	{
		double d = 0x1.2p3; // hex fraction 1.2 (decimal 1.125) scaled by 2^3, that is 9.0

		std::cout << 58. << '\n'
			<< 4e2 << '\n'
			<< 123.456e-67 << '\n'
			<< .1E4f << '\n'
			<< d << '\n'
			<< 0x10.1p0 << '\n';
	}

	void Cpp17::TemplateDeductionConstructor()
	{
		// similar to call TemplateArgumentDeduction
		auto [a, b] = std::pair(5.0, false);		// instead of std::pair<double, bool>(5.0, false)
		std::cout << a << " " << b << endl;
	}

	// suppresses compiler warnings about unused param entities.
	int Cpp17::StandardAttributes(bool b [[maybe_unused]])
	{
		int num = 0;

		switch (num) {
		case 0:
			// DO your stuff....
			// indicates that a fallthrough in a switch statement is intentional.
			[[fallthrough]];	// no need break

		default:
			return num;
		}
		return num;
	}

	void Cpp17::SpecialMathFunctions()
	{
		// EACH math function has 3 versions
		// double		func
		// float		func_f
		// long double	func_l

		auto binom = [](int n, int k) { return 1 / ((n + 1)*std::beta(n - k + 1, k + 1)); };

		// associated Laguerre polynomials 
		std::cout << std::assoc_laguerre(1, 10, 0.5) << endl;

		// associated Legendre polynomials 
		std::cout << std::assoc_legendre(1, 10, 0.5) << endl;

		// beta function
		std::cout << binom(1, 5) << endl;

		// (complete)elliptic integral of the first kind
		std::cout << std::comp_ellint_1(3.14159) << endl;

		// (complete) elliptic integral of the second kind 
		std::cout << std::comp_ellint_2(3.14159) << endl;

		// (complete) elliptic integral of the third kind 
		std::cout << std::comp_ellint_3(3.14159, 1.0) << endl;

		// regular modified cylindrical Bessel functions 
		std::cout << std::cyl_bessel_i(0, 1.2345) << endl;

		// cylindrical Bessel functions (of the first kind) 
		std::cout << std::cyl_bessel_j(0, 1.2345) << endl;

		// irregular modified cylindrical Bessel functions
		std::cout << std::cyl_bessel_k(0, 1.2345) << endl;

		// cylindrical Neumann functions 
		std::cout << std::cyl_neumann(0.5, 1.2345) << endl;

		// cylindrical Neumann functions 
		std::cout << std::ellint_1(0.5, 1.2345) << endl;

		// (incomplete)elliptic integral of the second kind
		std::cout << std::ellint_2(0.5, 1.2345) << endl;

		// exponential integral
		std::cout << std::expint(0) << endl;

		// hermite polynomials 
		std::cout << std::hermite(3, 10) << endl;

		// Legendre polynomials 
		std::cout << std::legendre(3, 10) << endl;

		// Laguerre polynomials 
		std::cout << std::laguerre(3, 10) << endl;

		// Riemann zeta function 
		std::cout << std::riemann_zeta(3) << endl;

		// spherical Bessel functions (of the first kind) 
		std::cout << std::sph_bessel(3, 6) << endl;

		// spherical associated Legendre functions 
		std::cout << std::sph_legendre(3, 6, 9.0) << endl;

		// spherical Neumann functions 
		std::cout << std::sph_neumann(3, 6.0) << endl;
	}
}