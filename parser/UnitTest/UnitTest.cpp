#include "pch.h"
#include "CppUnitTest.h"
#include "../parser/parser.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{
	TEST_CLASS(UnitTest)
	{
	public:
		
		TEST_METHOD(Test1)
		{
			Assert::IsTrue(Parser("const+(const*(const+const))/identifier>=const*(identifier)").parse());
		}

		TEST_METHOD(Test2)
		{
			Assert::IsTrue(Parser("const-const").parse());
		}

		TEST_METHOD(Test3)
		{
			Assert::IsTrue(Parser("const").parse());
		}

		TEST_METHOD(Test4)
		{
			Assert::IsTrue(Parser("((const))").parse());
		}

		TEST_METHOD(Test5)
		{
			Assert::IsFalse(Parser("").parse());
		}

		TEST_METHOD(Test6)
		{
			Assert::IsFalse(Parser("const-+const").parse());
		}

		TEST_METHOD(Test7)
		{
			Assert::IsFalse(Parser("const<const/identifier>=const*identifier").parse());
		}

		TEST_METHOD(Test8)
		{
			Assert::IsFalse(Parser("(const<=const*identifier)").parse());
		}

		TEST_METHOD(Test9)
		{
			Assert::IsFalse(Parser("(const").parse());
		}

		TEST_METHOD(Test10)
		{
			Assert::IsFalse(Parser("const)").parse());
		}

		TEST_METHOD(Test11)
		{
			Assert::IsFalse(Parser("()").parse());
		}
	};
}
