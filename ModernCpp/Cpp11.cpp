#include "pch.h"
#include "Cpp11.h"

#include <mutex>
#include <queue>
#include <regex>
#include <string>
#include <random>
#include <cassert>
#include <utility>
#include <iostream>
#include <algorithm>
#include <functional>

namespace MODERNCPP
{
	// Simple move constructor
	Cpp11::Cpp11(Cpp11&& arg) : member(std::move(arg.member)) // the expression "arg.member" is lvalue
	{}

	// Simple move assignment operator
	Cpp11& Cpp11::operator=(Cpp11&& arg)
	{
		if (this != &arg)
			member = std::move(arg.member);

		return *this;
	}

	Cpp11::Cpp11(const std::initializer_list<int> &list) : // initialized via list initialization
		Cpp11(0, 1)			// possibly call delegating constructor
	{
		for (const auto &i : list)
			m_vMap[count++] = i;
	}

	Cpp11& Cpp11::operator=(const std::initializer_list<int> &list)
	{
		count = 0;
		for (const auto &i : list)
			m_vMap[count++] = i;

		return *this;
	}

	int& Cpp11::operator[](int index)
	{
		assert(index >= 0 && index < m_vMap.size());
		return m_vMap[index];
	}

	void Cpp11::DoWork(void* pObj)
	{
		std::cout << "DoWork";

		Cpp11* p = static_cast<Cpp11*>(pObj);
		if (p)
		{
			p->DoWorkInternal();
		}
	}

	void Cpp11::DoWorkInternal()
	{
		std::cout << "DoWorkInternal";
	}

	std::thread Cpp11::CallDoWorkInternal(int n)
	{
		return std::thread([=] { DoWorkInternal(); });
	}

	void Cpp11::ForEachLoop()
	{
		// old way
		for (std::vector<int>::iterator i = m_vMap.begin(); i != m_vMap.end(); i++)
			std::cout << *i;

		// auto
		for (auto i = m_vMap.begin(); i != m_vMap.end(); i++)
			std::cout << *i;

		// auto foreach usinig reference (no copy)
		for (const auto &i : m_vMap) 
			std::cout << i;

		// foreach lambda
		std::for_each(std::begin(m_vMap), std::end(m_vMap), [](auto i) {
			std::cout << i;
		});

		std::cout << std::endl;
	}

	long Cpp11::EnumClass(WeekDay day)
	{
		return as_integer(day);
	}

	void Cpp11::Lambda()
	{
		// [firstPart](secondPart) TypeYouReturn{ BodyOfLambda}(acctualParameters);
		/*
			firstPart is used for variable scope which will be exploited inside the lambda function. You could use more variables if have a need for that.
			Keep the following in mind when you are using this syntax for the [firstPart]:

			[] it means that you will not provide anything to give to lambda.
			[&] it is used to say that you have some references to mess with.
			[=] it is used to make the copy.
			[this] is used to the enclosing class.

			secondPart is necessary for parameter list for nameless function but it could be left empty as well.
			TypeYouReturn is used to register what type will be returned from your lambda.
			BodyOfLambda is used for actions you wish to do, in here you will type some code that will be used to perform actions you are intended to apply in the body of this function.
			actualParameters are used to provide input into lambda function.
		*/

		// generic lambda, operator() is a template with two parameters
		auto glambda = [](auto a, auto&& b) { return a < b; };
		bool b = glambda(3, 3.14); // ok

		// generic lambda, operator() is a template with one parameter
		auto vglambda = [](auto printer) {
			return [=](auto&&... ts) // generic lambda, ts is a parameter pack
			{
				printer(std::forward<decltype(ts)>(ts)...);
				return [=] { printer(ts...); }; // nullary lambda (takes no parameters)
			};
		};

		auto p = vglambda([](auto v1, auto v2, auto v3) { std::cout << v1 << v2 << v3; });
		auto q = p(1, 'a', 3.14); // outputs 1a3.14
		q();                      // outputs 1a3.14

		// the context of the following lambda is the member function X::f
		[=]()->int
		{
			return operator()(this->x + y); // X::operator()(this->x + (*this).y)
											// this has type X*
		};
		std::cout << std::endl;
	}

	void Cpp11::StaticAssert()
	{
		// static_assert(evaluatedExpression, stringMessage);
		//static_assert(sizeof(long long int) >= 16, "This is unexpected");	

		// Possible output:
		// 1: error: static assertion failed : Swap requires copying
		// 2: error : static assertion failed : Data Structure requires default - constructible elements

		int a, b;
		swap(a, b);

		no_copy nc_a, nc_b;
		//swap(nc_a, nc_b); // error 1

		data_structure<int> ds_ok;
		//data_structure<no_default> ds_error; // error 2
	}

	long Cpp11::PseudoNumberGenerator(int min, int max)
	{
		//normal_distribution									// bell curve normal dist
		uniform_int_distribution<int> num_range{ min, max };	// distribution that maps to the ints min..max
		default_random_engine re {};							// the default engine

		//std::cout << num_range(re);				// x becomes a value in [1:6]

		auto dice{ std::bind(num_range,re) };		// make a generator
		auto d = dice();							// roll the dice: x becomes a value in [1:6]

		std::cout << d << endl;
		return d;
	}

	long Cpp11::RandomNumberGenerator(int min, int max)
	{
		std::random_device rd;		//Will be used to obtain a seed for the random number engine
		std::mt19937 gen(rd());		//Standard mersenne_twister_engine seeded with rd()
		std::uniform_int_distribution<> dis(min, max);

		auto g = dis(gen);
		std::cout << g << endl;
		return g;
	}

	void Cpp11::UniquePtr()
	{
		std::unique_ptr<Cpp11> up(new Cpp11);

		if (up && up->BaseMethod())		// use implicit cast to bool to ensure res contains a Resource
			std::cout << *up;
	}

	std::tuple<int, double> Cpp11::ReturnTuple()	// return a tuple that contains an int and a double
	{
		return std::make_tuple(5, 6.7);				// std::make_tuple() as shortcut to make a tuple to return
	}

	void Cpp11::Tuple()
	{
		// tuple
		std::tuple<int, double> s = ReturnTuple();	// get our tuple
		std::cout << std::get<0>(s) << ' '
				  << std::get<1>(s) << '\n';		// use std::get<n> to get the nth element of the tuple

		// tie to tuple
		int a;
		double b;
		std::tie(a, b) = ReturnTuple();		// put elements of tuple in variables a and b
		std::cout << a << ' ' << b << '\n';
	}

	void Cpp11::ReferenceWrapper()
	{
		std::vector<std::reference_wrapper<Base>> v;	// our vector is a vector of std::reference_wrapper wrapped Base (not Base&)					
		
		Base b; b.setValue(5);
		Cpp11 d; d.setValue(6);
	
		v.push_back(b); // add a Base object to our vector
		v.push_back(d); // add a Derived object to our vector

		// Print out all of the elements in our vector, use .get() to get our element from the wrapper
		for (const auto& i : v)
			std::cout << "Class = " << i.get().getName() << " Value = " << i.get().getValue() << "\n";
	}

	void Cpp11::InitializerList()
	{
		Cpp11 array{ 5, 4, 3, 2, 1 }; // initializer list
		for (int count = 0; count < array.getLength(); ++count)
			std::cout << array[count] << ' ';

		array = { 1, 3, 5, 7, 9, 11 };		// calls assignment = init list

		for (int count = 0; count < array.getLength(); ++count)
			std::cout << array[count] << ' ';

		std::cout << std::endl;
	}

	void Cpp11::VariadicTemplate()
	{
		long sum = adder(1, 2, 3, 8, 7);
		std::string s1 = "x", s2 = "aa", s3 = "bb", s4 = "yy";
		std::string ssum = adder(s1, s2, s3, s4);		
		std::cout << s1 << ssum << endl;
	}

	void Cpp11::UnorderedContainers()
	{
		//unordered_map
		//unordered_set
		//unordered_multimap
		//unordered_multiset

		// Iterate and print keys and values of unordered_map
		for (const auto& n : u) {
			std::cout << "Key:[" << n.first << "] Value:[" << n.second << "]\n";
		}

		// Add two new entries to the unordered_map
		u["BLACK"] = "#000000";
		u["WHITE"] = "#FFFFFF";

		// Output values by key
		std::cout << "The HEX of color RED is:[" << u["RED"] << "]\n";
		std::cout << "The HEX of color BLACK is:[" << u["BLACK"] << "]\n";
	}

	void Cpp11::RegularExpression()
	{
		std::string s = "Some people, when confronted with a problem, think "
			"\"I know, I'll use regular expressions.\" "
			"Now they have two problems.";

		std::regex self_regex("REGULAR EXPRESSIONS",
			std::regex_constants::ECMAScript | std::regex_constants::icase);
		if (std::regex_search(s, self_regex)) {
			std::cout << "Text contains the phrase 'regular expressions'\n";
		}

		std::regex word_regex("(\\S+)");
		auto words_begin =
			std::sregex_iterator(s.begin(), s.end(), word_regex);
		auto words_end = std::sregex_iterator();

		std::cout << "Found "
			<< std::distance(words_begin, words_end)
			<< " words\n";

		const int N = 6;
		std::cout << "Words longer than " << N << " characters:\n";
		for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
			std::smatch match = *i;
			std::string match_str = match.str();
			if (match_str.size() > N) {
				std::cout << "  " << match_str << '\n';
			}
		}

		std::regex long_word_regex("(\\w{7,})");
		std::string new_s = std::regex_replace(s, long_word_regex, "[$&]");
		std::cout << new_s << '\n';
	}

	void Cpp11::ParallelThreads()
	{
		const size_t nloop = 11;

		// Serial version
		{
			// Pre loop
			std::cout << "serial:" << std::endl;
			// loop over all items
			for (int i = 0; i < nloop; i++)
			{
				// inner loop
				{
					const int j = i * i;
					std::cout << j << std::endl;
				}
			}
			// Post loop
			std::cout << std::endl;
		}

		// Parallel version
		// number of threads
		const size_t nthreads = std::thread::hardware_concurrency();
		{
			// Pre loop
			std::cout << "parallel (" << nthreads << " threads):" << std::endl;
			std::vector<std::thread> threads(nthreads);
			std::mutex critical;
			for (int t = 0; t < nthreads; t++)
			{
				threads[t] = std::thread(std::bind(
					[&](const int bi, const int ei, const int t)
				{
					// loop over all items
					for (int i = bi; i < ei; i++)
					{
						// inner loop
						{
							const int j = i * i;
							// (optional) make output critical
							std::lock_guard<std::mutex> lock(critical);
							std::cout << j << std::endl;
						}
					}
				}, t*nloop / nthreads, (t + 1) == nthreads ? nloop : (t + 1)*nloop / nthreads, t));
			}
			std::for_each(threads.begin(), threads.end(), [](std::thread& x) {x.join(); });
			// Post loop
			std::cout << std::endl;
		}
	}

	void Cpp11::ProducerConsumer()
	{
		int c = 0;
		bool done = false;
		std::queue<int> goods;

		thread producer([&]() {
			for (int i = 0; i < 500; ++i) {
				goods.push(i);
				c++;
			}
			done = true;
		});

		thread consumer([&]() {
			while (!done) {
				while (!goods.empty()) {
					goods.pop();
					c--;
				}
			}
		});

		producer.join();
		consumer.join();
		cout << "Net: " << c << endl;
	}

	void Cpp11::VariableSizes()
	{
		std::cout << "bool:\t\t" << sizeof(bool) << " bytes" << std::endl;
		std::cout << "char:\t\t" << sizeof(char) << " bytes" << std::endl;
		std::cout << "wchar_t:\t" << sizeof(wchar_t) << " bytes" << std::endl;
		std::cout << "char16_t:\t" << sizeof(char16_t) << " bytes" << std::endl; // C++11, may not be supported by your compiler
		std::cout << "char32_t:\t" << sizeof(char32_t) << " bytes" << std::endl; // C++11, may not be supported by your compiler
		std::cout << "short:\t\t" << sizeof(short) << " bytes" << std::endl;
		std::cout << "int:\t\t" << sizeof(int) << " bytes" << std::endl;
		std::cout << "long:\t\t" << sizeof(long) << " bytes" << std::endl;
		std::cout << "long long:\t" << sizeof(long long) << " bytes" << std::endl; // C++11, may not be supported by your compiler
		std::cout << "float:\t\t" << sizeof(float) << " bytes" << std::endl;
		std::cout << "double:\t\t" << sizeof(double) << " bytes" << std::endl;
		std::cout << "long double:\t" << sizeof(long double) << " bytes" << std::endl;
	}

	auto Cpp11::TrailingReturnType() const -> int
	{
		return 1;
	}

	void Cpp11::DeclType()
	{
		// C++ program to demonstrate working of auto and type inference 
		auto x = 4;
		auto y = 3.37;
		auto ptr = &x;
		std::cout << typeid(x).name() << endl
				  << typeid(y).name() << endl
				  << typeid(ptr).name() << endl;

		//int x = 5;
		// j will be of type int : data type of x 
		decltype(x) j = x + 5;
		std::cout << typeid(j).name();

		// This call returns 3.44 of doubale type 
		std::cout << findMin(4, 3.44) << endl;

		// This call returns 3 of int type 
		std::cout << findMin(5.4, 3) << endl;
	}

	std::ostream& operator<<(std::ostream& out, const Cpp11 &res)
	{
		out << "I am in Cpp11\n";
		return out;
	}
}


