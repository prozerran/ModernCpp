#include "pch.h"
#include "Cpp14.h"

#include <string>
#include <iostream>
#include <algorithm>

namespace MODERNCPP
{
	void Cpp14::AggregateMemberInitialization()
	{
		std::cout << x.length << " " << x.width << endl;
	}

	void Cpp14::BinaryLiterals()
	{
		// C++98
		int bin(0);
		bin = 0x01;		// assign binary 0000 0001 to the variable
		bin = 0x02;		// assign binary 0000 0010 to the variable
		bin = 0x04;		// assign binary 0000 0100 to the variable
		bin = 0x08;		// assign binary 0000 1000 to the variable
		bin = 0x10;		// assign binary 0001 0000 to the variable
		bin = 0x20;		// assign binary 0010 0000 to the variable
		bin = 0x40;		// assign binary 0100 0000 to the variable
		bin = 0x80;		// assign binary 1000 0000 to the variable
		bin = 0xFF;		// assign binary 1111 1111 to the variable
		bin = 0xB3;		// assign binary 1011 0011 to the variable
		bin = 0xF770;	// assign binary 1111 0111 0111 0000 to the variable
		std::cout << bin << endl;

		// C++14
		// In C++14, we can assign binary literals by using the 0b prefix:
		bin = 0;
		bin = 0b1;						// assign binary 0000 0001 to the variable
		bin = 0b11;						// assign binary 0000 0011 to the variable
		bin = 0b1010;					// assign binary 0000 1010 to the variable
		bin = 0b11110000;				// assign binary 1111 0000 to the variable
		std::cout << bin << endl;

		bin = 0b1011'0010;				// assign binary 1011 0010 to the variable
		long value = 2'132'673'462;		// much easier to read than 2132673462
		std::cout << bin << endl;
	}

	void Cpp14::DigitSeparators()
	{
		auto integer_literal = 1'000'000;
		std::cout << integer_literal << endl;

		auto floating_point_literal = 0.000'015'3;
		std::cout << floating_point_literal << endl;

		auto binary_literal = 0b0100'1100'0110;
		std::cout << binary_literal << endl;

		auto silly_example = 1'0'0'000'00;
		std::cout << silly_example << endl;
	}

	void Cpp14::GenericLambdaExpr()
	{
		// For example, this generic lambda-expression containing statement:
		auto L = [](const auto& x, auto& y) { return x + y; };

		// might result in the creation of a closure type, and object that behaves similar to the struct below:
		/*
		struct // anonymous
		{
			template <typename T, typename U>
			auto operator()(const T& x, U& y) const // N3386 Return type deduction
			{
				return x + y;
			}
		} L;
		*/
		std::cout << typeid(L).name() << endl;
	}

	void Cpp14::LambdaCaptureExpr()
	{
		// use of an initializer expression:
		auto lambda1 = [value = 1]{ return value; };
		std::cout << lambda1() << endl;

		// capture by move, via the use of the standard std::move function
		std::unique_ptr<int> ptr(new int(10));
		auto lambda2 = [value = std::move(ptr)]{ return *value; };
		std::cout << lambda2() << endl;
	}

	void Cpp14::ConstantExprRestriction()
	{
		std::vector<int> a, b;
		//This does not compile but as my understadnding of `constexpr` this should
		//std::vector::size is not marked constexpr
		//int array[std::max(a.size(), b.size()];

		//This is trivial use that does compile
		int array[std::max(1, 2)];
	}

	void Cpp14::VariableTemplate()
	{
		auto x = pi<double>;		// can be int, double, float, etc
		std::cout << x << endl;

		auto y = pi<const char*>;	// can be int, double, float, etc
		std::cout << y << endl;

		auto z = circular_area<double>(3.0);	// can be int, double, float, etc
		std::cout << z << endl;		
	}

	void Cpp14::TupleAddressingByType()
	{
		std::tuple<string, string, int> t("foo", "bar", 7);
		int i = get<int>(t);        // i == 7
		int j = get<2>(t);          // Same as before: j == 7
		//string s = get<string>(t);  // Compile-time error due to ambiguity
		string s = get<0>(t);

		std::cout << i << j << s << endl;
	}

	void Cpp14::MakeUnique()
	{
		// https://www.learncpp.com/cpp-tutorial/15-5-stdunique_ptr/

		// Create a single dynamically allocated Fraction with numerator 3 and denominator 5
		std::unique_ptr<Fraction> f1 = std::make_unique<Fraction>(3, 5);
		std::cout << *f1 << '\n';

		// Create a dynamically allocated array of Fractions of length 4
		// We can also use automatic type deduction to good effect here
		auto f2 = std::make_unique<Fraction[]>(4);
		std::cout << f2[0] << '\n';
	}

	void Cpp14::HeterogeneousLookupAssocContainers()
	{
		// less
		int foo[] = { 10,20,5,15,25 };
		int bar[] = { 15,10,20 };
		std::sort(foo, foo + 5, std::less<int>());  // 5 10 15 20 25
		std::sort(bar, bar + 3, std::less<int>());  //   10 15 20
		if (std::includes(foo, foo + 5, bar, bar + 3, std::less<int>()))
			std::cout << "foo includes bar.\n";

		// greater
		int numbers[] = { 20,40,50,10,30 };
		std::sort(numbers, numbers + 5, std::greater<int>());
		for (int i = 0; i < 5; i++)
			std::cout << numbers[i] << ' ';
		std::cout << '\n';
	}

	void Cpp14::SharedMutexesLocking()
	{
		ThreadSafeCounter counter;

		auto increment_and_print = [&counter]() 
		{
			for (int i = 0; i < 3; i++) 
			{
				counter.increment();
				std::cout << std::this_thread::get_id() << ' ' << counter.get() << '\n';

				// Note: Writing to std::cout actually needs to be synchronized as well
				// by another std::mutex. This has been omitted to keep the example small.
			}
		};

		std::thread thread1(increment_and_print);
		std::thread thread2(increment_and_print);

		thread1.join();
		thread2.join();
	}

	void Cpp14::DeprecatedMethod()
	{
		std::cout << "DeprecatedMethod should display a compile warning!\n"
			<< "But somehow not really working, need to figure out why"
			<< endl;
	}
}