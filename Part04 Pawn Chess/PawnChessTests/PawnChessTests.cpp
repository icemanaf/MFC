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

		TEST_METHOD(GenerateMoves_StartingPosition_SizeSix)
		{
			const uint64_t RANK_6_MASK = 0xFC0000000;
			const uint64_t RANK_1_MASK = 0x3f;
			ChessBoard starting;
			starting.BlackPawns = RANK_6_MASK;
			starting.WhitePawns = RANK_1_MASK;

			std::vector<ChessBoard> moves = PawnChessEngine::GenerateMoves(starting, true);

			Assert::AreEqual(6, static_cast<int>(moves.size()));
		}

		TEST_METHOD(GenerateMoves_FacingPositions_SizeTen)
		{
			const uint64_t RANK_6_MASK = 0xFC0000000;
			const uint64_t RANK_5_MASK = 0x3F000000;
			ChessBoard facing;
			facing.BlackPawns = RANK_6_MASK;
			facing.WhitePawns = RANK_5_MASK;

			std::vector<ChessBoard> moves = PawnChessEngine::GenerateMoves(facing, false);

			Assert::AreEqual(10, static_cast<int>(moves.size()));
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

		TEST_METHOD(ValidateMove_NotMoved_False)
		{
			const uint64_t RANK_6_MASK = 0xFC0000000;
			const uint64_t RANK_1_MASK = 0x3f;
			ChessBoard starting;
			starting.BlackPawns = RANK_6_MASK;
			starting.WhitePawns = RANK_1_MASK;

			bool valid = PawnChessEngine::ValidateMove(starting, starting, true);

			Assert::IsFalse(valid);
		}

		TEST_METHOD(ValidateMove_Moved_True)
		{
			const uint64_t RANK_6_MASK = 0xFC0000000;
			const uint64_t RANK_1_MASK = 0x3f;
			ChessBoard starting;
			starting.BlackPawns = RANK_6_MASK;
			starting.WhitePawns = RANK_1_MASK;

			ChessBoard ending;
			ending.BlackPawns = RANK_6_MASK;
			// 0000000000000000000000000000000000000000000000000000100000011111
			ending.WhitePawns = 0x81f;

			bool valid = PawnChessEngine::ValidateMove(starting, ending, true);

			Assert::IsTrue(valid);
		}

		TEST_METHOD(ValidateMove_MovedTooFar_False)
		{
			const uint64_t RANK_6_MASK = 0xFC0000000;
			const uint64_t RANK_1_MASK = 0x3f;
			ChessBoard starting;
			starting.BlackPawns = RANK_6_MASK;
			starting.WhitePawns = RANK_1_MASK;

			ChessBoard ending;
			ending.BlackPawns = RANK_6_MASK;
			// 0000000000000000000000000000000000000000000000100000000000011111
			ending.WhitePawns = 0x2001f;

			bool valid = PawnChessEngine::ValidateMove(starting, ending, true);

			Assert::IsFalse(valid);
		}

		TEST_METHOD(MinMaxEx_WhiteInRank6Win_Infinity)
		{
			const uint64_t RANK_6_MASK = 0xFC0000000;
			const uint64_t RANK_5_MASK = 0x3F000000;
			ChessBoard currentPos;
			currentPos.BlackPawns = RANK_5_MASK;
			currentPos.WhitePawns = RANK_6_MASK;

			int depthToSearch = 11;

			int32_t result = PawnChessEngine::MinMaxEx(currentPos, false, depthToSearch, depthToSearch,
				-INFINITY32, INFINITY32, EvaluatePosition);

			Assert::AreEqual(INFINITY32, result);
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
