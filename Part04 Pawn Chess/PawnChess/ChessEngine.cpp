
#include "pch.h"
#include <vector>
#include "ChessEngine.h"
#include <inttypes.h>

/*assume user is playing on a1 to a6 and system is playing  on squares f1-f6
    * All moves and captures are coded on this assumption
    */

//the first 36 squares ie bits 0-35
const uint64_t BOARD_MASK = 0xFFFFFFFFF;// the bitboard of the relevant squares. only take in to consideration 

//0000000000000000000000000000000000000000000000000000000000 111111 (the first rank)
const uint64_t RANK_1_MASK = 0x3f;
//0000000000000000000000000000000000000000000000000000111111000000 (the second rank)
const uint64_t RANK_2_MASK = 0xFC0;
const uint64_t RANK_3_MASK = 0x3F000;
const uint64_t RANK_4_MASK = 0xFC0000;
const uint64_t RANK_5_MASK = 0x3F000000;
const uint64_t RANK_6_MASK = 0xFC0000000;

const int16_t PAWN_VALUE = 10000;

// TODO: This how to do delegates?
//public delegate int Evaluate(ChessBoard currentPos, bool UserToMove);

bool PawnChessEngine::IsPositionLegal(ChessBoard currentPos)
{
    //basic check- position conficts between white and black
    return (currentPos.BlackPawns & currentPos.WhitePawns & BOARD_MASK) == 0 ? true : false;
}

//todo 

std::vector<uint64_t> GenerateMoves(ChessBoard currentPos, bool userToMove)
{
	return std::vector<uint64_t>();
}
