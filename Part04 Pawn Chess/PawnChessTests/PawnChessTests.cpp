#include "pch.h"
#include "CppUnitTest.h"
#include "../PawnChess/ChessEngine.cpp"
#include <inttypes.h>

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

			Assert::AreNotEqual(s, s2);
		}

		TEST_METHOD(IsPositionLegal_StartingPosition_True)
		{
			const uint64_t RANK_6_MASK = 0xFC0000000;
			const uint64_t RANK_1_MASK = 0x3f;
			ChessBoard starting;
			starting.BlackPawns = RANK_6_MASK;
			starting.WhitePawns = RANK_1_MASK;

			bool result = PawnChessEngine::IsPositionLegal(starting);
			Assert::IsTrue(result);
		}

		TEST_METHOD(IsPositionLegal_Overlaps_False)
		{
			const uint64_t RANK_1_MASK = 0x3f;
			ChessBoard overlapping;
			overlapping.BlackPawns = RANK_1_MASK;
			overlapping.WhitePawns = RANK_1_MASK;

			bool result1 = PawnChessEngine::IsPositionLegal(overlapping);
			Assert::IsFalse(result1);

			overlapping.WhitePawns = 0x1;

			bool result2 = PawnChessEngine::IsPositionLegal(overlapping);
			Assert::IsFalse(result2);
		}

	//private:
	//	ChessBoard array2dToBitboard(int board[][4])
	//	{
	//		ChessBoard retBoard;
	//		uint64_t bitCounter = 1;

	//		for (int i = 0; i < 6; i++)
	//		{
	//			for (int j = 0; j < 6; j++)
	//			{
	//				if (graphicalBoard[j, i].Image == imgSystemPawn)
	//				{
	//					retBoard.BlackPawns = retBoard.BlackPawns | bitCounter;
	//				}
	//				else if (graphicalBoard[j, i].Image == imgUserPawn)
	//				{
	//					retBoard.WhitePawns = retBoard.WhitePawns | bitCounter;
	//				}
	//				bitCounter = bitCounter << 1; // this will be our mask
	//			}
	//		}

	//		return retBoard;
	//	}
	};

}
