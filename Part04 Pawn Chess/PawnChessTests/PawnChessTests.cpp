#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace PawnChessTests
{
	TEST_CLASS(PawnChessTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			std::string s = "hello";
			std::string s2 = "hella";

			Assert::AreEqual(s, s2);
		}
	};
}
