// ModernCpp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>

#include "Cpp11.h"
#include "Cpp14.h"
#include "Cpp17.h"

using namespace MODERNCPP;

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

int main()
{
    std::cout << "Hello ModernCpp!\n"; 

	#pragma region Cpp11

	Cpp11 cpp11;
	cpp11.ConstExpr(2, 3);
	cpp11.ForEachLoop();
	cpp11.EnumClass(WeekDay::Friday);
	cpp11.Lambda();
	cpp11.PseudoNumberGenerator(1, 6);
	cpp11.RandomNumberGenerator(1, 6);
	cpp11.Tuple();							// inline struct, no need struct object
	cpp11.UniquePtr();						// replacement of AutoPtr
	cpp11.NullPtr(nullptr);					// replacement of NULL
	cpp11.ReferenceWrapper();				// object slicing
	cpp11.InitializerList();
	cpp11.StaticAssert();					// compile-time assert
	cpp11.VariadicTemplate();
	cpp11.UnorderedContainers();
	cpp11.ParallelThreads();
	cpp11.ProducerConsumer();
	cpp11.RegularExpression();
	cpp11.VariableSizes();
	cpp11.TrailingReturnType();
	cpp11.DeclType();

	// threaded call, through static, OLD way
	std::thread t1(Cpp11::DoWork, &cpp11);
	t1.join();

	// call member directly, C++11 NEW way
	std::thread t2 = cpp11.CallDoWorkInternal(1);
	t2.join();

	#pragma endregion

	#pragma region Cpp14

	Cpp14 cpp14;
	cpp14.AggregateMemberInitialization();
	cpp14.BinaryLiterals();
	cpp14.DigitSeparators();
	cpp14.ReturnTypeDeduction(1, 2);		// can be int, double, etc
	cpp14.GenericLambdaExpr();
	cpp14.LambdaCaptureExpr();
	cpp14.ConstantExprRestriction();
	cpp14.VariableTemplate();
	cpp14.TupleAddressingByType();
	cpp14.MakeUnique();
	cpp14.DeprecatedMethod();
	cpp14.HeterogeneousLookupAssocContainers();
	cpp14.SharedMutexesLocking();

	#pragma endregion

	#pragma region Cpp17

	Cpp17 cpp17;

	#pragma endregion

	return 0;
}