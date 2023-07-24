
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


std::vector<ChessBoard> PawnChessEngine::GenerateMoves(ChessBoard currentPos, bool userToMove)
{
    int NoOfMoves = 0;//no of moves generated for this position
    uint64_t new_players_bitboard = 0;
    uint64_t new_oponents_bitboard = 0;
    uint64_t bitCounter = 1;

    //white
    uint64_t Movers = 0;
    uint64_t LeftCaptures = 0;
    uint64_t RightCaptures = 0;

    std::vector<ChessBoard> generatedMoves;
    // TODO: Is this the theoretical maximum?
    const uint8_t MAXIMUM_MOVES = 18;
    generatedMoves.reserve(MAXIMUM_MOVES);


    if (userToMove)
    {
        //white
        Movers = GetUserMovers(currentPos);
        LeftCaptures = GetUserCapturesLeft(currentPos);
        RightCaptures = GetUserCapturesRight(currentPos);

        NoOfMoves = GetNoOfSetBits(Movers) + GetNoOfSetBits(LeftCaptures) + GetNoOfSetBits(RightCaptures);
        if (NoOfMoves > 0)
        {

            for (int i = 0; i < 36; i++)
            {


                //generate positions for the left captures
                if ((bitCounter & LeftCaptures) != 0)
                {
                    //move the white pawn
                    new_players_bitboard = (currentPos.WhitePawns & (~bitCounter)) | (bitCounter << 5);
                    //remove the captured black piece
                    new_oponents_bitboard = (~(bitCounter << 5)) & currentPos.BlackPawns;

                    ChessBoard newBoard;
                    newBoard.WhitePawns = new_players_bitboard;
                    newBoard.BlackPawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }


                //generate positions for the right captures
                if ((bitCounter & RightCaptures) != 0)
                {
                    //todo
                    new_players_bitboard = (currentPos.WhitePawns & (~bitCounter) & BOARD_MASK) | (bitCounter << 7);
                    new_oponents_bitboard = (~(bitCounter << 7)) & currentPos.BlackPawns;

                    ChessBoard newBoard;
                    newBoard.WhitePawns = new_players_bitboard;
                    newBoard.BlackPawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }

                //generate positions from the white movers bit board
                if ((bitCounter & Movers) != 0)
                {
                    // create a new board position
                    new_players_bitboard = (currentPos.WhitePawns & (~bitCounter)) | (bitCounter << 6);

                    ChessBoard newBoard;
                    newBoard.WhitePawns = new_players_bitboard;
                    newBoard.BlackPawns = currentPos.BlackPawns;
                    generatedMoves.push_back(newBoard);
                }


                bitCounter = bitCounter << 1;
            }
        }
    }
    else
    {
        //system move
        Movers = GetSystemMovers(currentPos);
        LeftCaptures = GetSystemLeftCaptures(currentPos);
        RightCaptures = GetSystemRightCaptures(currentPos);

        NoOfMoves = GetNoOfSetBits(Movers) + GetNoOfSetBits(LeftCaptures) + GetNoOfSetBits(RightCaptures);
        if (NoOfMoves > 0)
        {

            for (int i = 0; i < 36; i++)
            {

                //right captures
                if ((bitCounter & RightCaptures) != 0)
                {

                    new_players_bitboard = ((currentPos.BlackPawns & BOARD_MASK & (~bitCounter)) | (bitCounter >> 5));
                    new_oponents_bitboard = (currentPos.WhitePawns & BOARD_MASK) & (~(bitCounter >> 5));

                    ChessBoard newBoard;
                    newBoard.BlackPawns = new_players_bitboard;
                    newBoard.WhitePawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }

                //left captures
                if ((bitCounter & LeftCaptures) != 0)
                {

                    new_players_bitboard = ((currentPos.BlackPawns & BOARD_MASK & (~bitCounter)) | (bitCounter >> 7));
                    new_oponents_bitboard = ((currentPos.WhitePawns & BOARD_MASK) & (~(bitCounter >> 7)));

                    ChessBoard newBoard;
                    newBoard.BlackPawns = new_players_bitboard;
                    newBoard.WhitePawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }



                //movers
                if ((bitCounter & Movers) != 0)
                {
                    new_players_bitboard = (((~bitCounter) & currentPos.BlackPawns & BOARD_MASK) | ((bitCounter) >> 6));


                    ChessBoard newBoard;
                    newBoard.BlackPawns = new_players_bitboard;
                    newBoard.WhitePawns = currentPos.WhitePawns;
                    generatedMoves.push_back(newBoard);
                }

                bitCounter = bitCounter << 1;
            }

        }
    }
    generatedMoves.shrink_to_fit();
    return generatedMoves;
}

uint64_t PawnChessEngine::GetUserMovers(ChessBoard currentPos)
{
    /*compute a bitboard of the user pawns that
     * are able to MOVE*/
     // get the bit board of the non occupied squares
    uint64_t unoccupied_squares = (~(currentPos.BlackPawns | currentPos.WhitePawns)) & BOARD_MASK;

    //shift the user pawns up one rank and AND it with the unoccupied squares , this leaves us with the user pawns that have moved
    uint64_t user_movers = (currentPos.WhitePawns << 6) & unoccupied_squares;

    /*shift down to the original positions, now you have the bitboard of the user pawns that can move forward*/
    user_movers = user_movers >> 6;

    return user_movers;
}

uint64_t PawnChessEngine::GetSystemMovers(ChessBoard currentPos)
{
    /*Compute a bit board of system pawns that can move ahead*/
    uint64_t unoccupied_squares = (~(currentPos.BlackPawns | currentPos.WhitePawns)) & BOARD_MASK;

    uint64_t system_movers = (currentPos.BlackPawns >> 6) & unoccupied_squares;

    //shift up to the original positions
    system_movers = system_movers << 6;

    //return the bitboard
    return system_movers;
}

uint64_t PawnChessEngine::GetUserCapturesRight(ChessBoard currentPos)
{

    //todo return the bitboard of user pieces that are able to CAPTURE system pawns on the right diagonal

    /*the user pieces on the the squares f1-f6 cant do any right diagonal captures.
    */
    /* create a mask for the user pieces that will be able to capture diagonally right
     * 0000000000000000000000000000000000 011111 011111 011111 011111 011111
    * hex value 0x1F7DF7DF
     * 1F7DF7DF
    */
    const uint64_t RIGHT_CAPTURE_MASK = 0x1F7DF7DF;


    uint64_t user_right_captures = (currentPos.WhitePawns & RIGHT_CAPTURE_MASK); // this gives us the white pawns after it the cropping by the mask.

    //shift the white pawns by 7,this moves them up by 1 rank , to the right diagonal
    user_right_captures = user_right_captures << 7;

    // AND it with the black pawns this gives the bitboard of the white pieces
    user_right_captures = (user_right_captures & currentPos.BlackPawns);

    //move the result back to where they were before ; ie shift back 7 ;
    user_right_captures = user_right_captures >> 7;

    //we have the bitboard of white pieces that can capture right!
    return user_right_captures;
}

uint64_t PawnChessEngine::GetSystemRightCaptures(ChessBoard currentPos)
{
    //todo - return the captures that are able to capture the user pawns on the right bottom diagnol.
    /*mask 000000000000000000000000000 011111 011111 011111 011111 011111 000000
     * hex value 0xFBEFBEF80
     */

    const uint64_t RIGHT_CAPTURE_MASK = 0x7DF7DF7C0;

    uint64_t system_right_captures = (currentPos.BlackPawns & RIGHT_CAPTURE_MASK);

    //shift the pawns down by 5,takes them to the right bottom diagnol.
    system_right_captures = system_right_captures >> 5;

    //AND it with the user pawns, gives us the system pawns that can capture 
    system_right_captures = (system_right_captures & currentPos.WhitePawns);

    //move the result to the original places
    system_right_captures = system_right_captures << 5;

    //return the bitboard
    return system_right_captures;
}

uint64_t PawnChessEngine::GetUserCapturesLeft(ChessBoard currentPos)
{
    //todo return the bitboard of user pieces that are able to CAPTURE system pawns on the left diagonal
    /* the user pwans on squares a1-a6 cant do any left diagonal captures.
    */

    /* create a mask for the user pieces that will be able to capture diagonally left
     * *0000000000000000000000000000      000000              111110      111110      111110      111110      111110
     *ignore this bits                  6th rank (ignore)   5th Rank    4th Rank    3rd Rank    2nd Rank    1st Rank
     * hex value 0x3EFBEFBE
     *

    */
    const uint64_t LEFT_CAPTURE_MASK = 0x3EFBEFBE;



    //crop with the mask
    uint64_t user_left_captures = (currentPos.WhitePawns & LEFT_CAPTURE_MASK);

    //shift the resultant value by 5to the right , to move it up by 1 rank to the left diagonal
    user_left_captures = user_left_captures << 5;

    //AND the result with the bitboard of the black pawns
    user_left_captures = (user_left_captures & currentPos.BlackPawns & BOARD_MASK);


    //shift it left by 5 to return the white pawns to where they were before
    user_left_captures = user_left_captures >> 5;

    return user_left_captures;
}

uint64_t PawnChessEngine::GetSystemLeftCaptures(ChessBoard currentPos)
{
    /*create a mask that ignores system pawns on f1-f6
     * 0000000000000000000000000000 111110 111110 111110 111110 111110 000000
     * hex value 0xFBEFBEF80
     */

    const uint64_t LEFT_CAPTURE_MASK = 0xFBEFBEF80;

    //crop using the mask to weed out the impossible captures ie the wraparounds
    uint64_t system_left_captures = (currentPos.BlackPawns & LEFT_CAPTURE_MASK);

    //shift right by 7
    system_left_captures = system_left_captures >> 7;

    //AND the result with the bitboard of WHITE pawns
    system_left_captures = (system_left_captures & currentPos.WhitePawns & BOARD_MASK);

    //shift the pieces back to the original positions
    system_left_captures = system_left_captures << 7;

    return system_left_captures;
}

uint8_t PawnChessEngine::GetNoOfSetBits(uint64_t bitBoard)
{
    /*New improved version returns the no of set bits in a bitboard*/
    uint8_t counter;

    for (counter = 0; bitBoard != 0; counter++)
    {
        //common assembly language bit banging technique
        bitBoard = bitBoard & (bitBoard - 1);

    }
    return counter;

}

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
