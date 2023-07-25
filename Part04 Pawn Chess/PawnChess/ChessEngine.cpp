
#include "pch.h"
#include <vector>
#include "ChessEngine.h"
#include <inttypes.h>

/*assume user is playing on a1 to a6 and system is playing  on squares f1-f6
    * All moves and captures are coded on this assumption
    */

//the first 36 squares ie bits 0-35
const uint64_t BOARD_MASK = 0xFFFFFFFFF;// the bitboard of the relevant squares. only take in to consideration 

//0000000000000000000000000000000000000000000000000000000000111111 (the first rank)
const uint64_t RANK_1_MASK = 0x3f;
//0000000000000000000000000000000000000000000000000000111111000000 (the second rank)
const uint64_t RANK_2_MASK = 0xFC0;
const uint64_t RANK_3_MASK = 0x3F000;
const uint64_t RANK_4_MASK = 0xFC0000;
const uint64_t RANK_5_MASK = 0x3F000000;
const uint64_t RANK_6_MASK = 0xFC0000000;

const int16_t PAWN_VALUE = 10000;

ChessBoard PawnChessEngine::ReplyMove;

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
                    new_players_bitboard = (currentPos.UserPawns & (~bitCounter)) | (bitCounter << 5);
                    //remove the captured black piece
                    new_oponents_bitboard = (~(bitCounter << 5)) & currentPos.SystemPawns;

                    ChessBoard newBoard;
                    newBoard.UserPawns = new_players_bitboard;
                    newBoard.SystemPawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }


                //generate positions for the right captures
                if ((bitCounter & RightCaptures) != 0)
                {
                    //todo
                    new_players_bitboard = (currentPos.UserPawns & (~bitCounter) & BOARD_MASK) | (bitCounter << 7);
                    new_oponents_bitboard = (~(bitCounter << 7)) & currentPos.SystemPawns;

                    ChessBoard newBoard;
                    newBoard.UserPawns = new_players_bitboard;
                    newBoard.SystemPawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }

                //generate positions from the white movers bit board
                if ((bitCounter & Movers) != 0)
                {
                    // create a new board position
                    new_players_bitboard = (currentPos.UserPawns & (~bitCounter)) | (bitCounter << 6);

                    ChessBoard newBoard;
                    newBoard.UserPawns = new_players_bitboard;
                    newBoard.SystemPawns = currentPos.SystemPawns;
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

                    new_players_bitboard = ((currentPos.SystemPawns & BOARD_MASK & (~bitCounter)) | (bitCounter >> 5));
                    new_oponents_bitboard = (currentPos.UserPawns & BOARD_MASK) & (~(bitCounter >> 5));

                    ChessBoard newBoard;
                    newBoard.SystemPawns = new_players_bitboard;
                    newBoard.UserPawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }

                //left captures
                if ((bitCounter & LeftCaptures) != 0)
                {

                    new_players_bitboard = ((currentPos.SystemPawns & BOARD_MASK & (~bitCounter)) | (bitCounter >> 7));
                    new_oponents_bitboard = ((currentPos.UserPawns & BOARD_MASK) & (~(bitCounter >> 7)));

                    ChessBoard newBoard;
                    newBoard.SystemPawns = new_players_bitboard;
                    newBoard.UserPawns = new_oponents_bitboard;
                    generatedMoves.push_back(newBoard);
                }



                //movers
                if ((bitCounter & Movers) != 0)
                {
                    new_players_bitboard = (((~bitCounter) & currentPos.SystemPawns & BOARD_MASK) | ((bitCounter) >> 6));


                    ChessBoard newBoard;
                    newBoard.SystemPawns = new_players_bitboard;
                    newBoard.UserPawns = currentPos.UserPawns;
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
    uint64_t unoccupied_squares = (~(currentPos.SystemPawns | currentPos.UserPawns)) & BOARD_MASK;

    //shift the user pawns up one rank and AND it with the unoccupied squares , this leaves us with the user pawns that have moved
    uint64_t user_movers = (currentPos.UserPawns << 6) & unoccupied_squares;

    /*shift down to the original positions, now you have the bitboard of the user pawns that can move forward*/
    user_movers = user_movers >> 6;

    return user_movers;
}

uint64_t PawnChessEngine::GetSystemMovers(ChessBoard currentPos)
{
    /*Compute a bit board of system pawns that can move ahead*/
    uint64_t unoccupied_squares = (~(currentPos.SystemPawns | currentPos.UserPawns)) & BOARD_MASK;

    uint64_t system_movers = (currentPos.SystemPawns >> 6) & unoccupied_squares;

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


    uint64_t user_right_captures = (currentPos.UserPawns & RIGHT_CAPTURE_MASK); // this gives us the white pawns after it the cropping by the mask.

    //shift the white pawns by 7,this moves them up by 1 rank , to the right diagonal
    user_right_captures = user_right_captures << 7;

    // AND it with the black pawns this gives the bitboard of the white pieces
    user_right_captures = (user_right_captures & currentPos.SystemPawns);

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

    uint64_t system_right_captures = (currentPos.SystemPawns & RIGHT_CAPTURE_MASK);

    //shift the pawns down by 5,takes them to the right bottom diagnol.
    system_right_captures = system_right_captures >> 5;

    //AND it with the user pawns, gives us the system pawns that can capture 
    system_right_captures = (system_right_captures & currentPos.UserPawns);

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
    uint64_t user_left_captures = (currentPos.UserPawns & LEFT_CAPTURE_MASK);

    //shift the resultant value by 5to the right , to move it up by 1 rank to the left diagonal
    user_left_captures = user_left_captures << 5;

    //AND the result with the bitboard of the black pawns
    user_left_captures = (user_left_captures & currentPos.SystemPawns & BOARD_MASK);


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
    uint64_t system_left_captures = (currentPos.SystemPawns & LEFT_CAPTURE_MASK);

    //shift right by 7
    system_left_captures = system_left_captures >> 7;

    //AND the result with the bitboard of WHITE pawns
    system_left_captures = (system_left_captures & currentPos.UserPawns & BOARD_MASK);

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
    return (currentPos.SystemPawns & currentPos.UserPawns & BOARD_MASK) == 0 ? true : false;
}

bool PawnChessEngine::ValidateMove(ChessBoard currentPos, ChessBoard moveAfterPos, bool userToMove)
{
    /*if this returns false then then the move is illegal*/
    bool Bret = false;
    std::vector<ChessBoard> possibleMovesFromPos = GenerateMoves(currentPos, userToMove);

    for (int i = 0; i < possibleMovesFromPos.size(); i++)
    {
        if ((moveAfterPos.UserPawns == possibleMovesFromPos[i].UserPawns) && (moveAfterPos.SystemPawns == possibleMovesFromPos[i].SystemPawns))
        {
            Bret = true;
            break;
        }
    }

    return Bret;
}

MOVE_STATUS PawnChessEngine::DetectWinLoss(ChessBoard currentPos, bool userToMove)
{
    int noOfPossibleMoves = 0;
    if (userToMove)
    {
        //white to move
        if ((RANK_6_MASK & currentPos.UserPawns) != 0)
            return MOVE_STATUS::USER_WINS;

        //if no moves are possible for user then system wins

        noOfPossibleMoves = GetNoOfSetBits(GetUserMovers(currentPos)) + GetNoOfSetBits(GetUserCapturesRight(currentPos)) + GetNoOfSetBits(GetUserCapturesLeft(currentPos));

        if (noOfPossibleMoves == 0)
            return MOVE_STATUS::SYSTEM_WINS;

        return MOVE_STATUS::MOVE_OK;
    }
    else
    {
        //system to move
        if ((RANK_1_MASK & currentPos.SystemPawns) != 0)
            return MOVE_STATUS::SYSTEM_WINS;



        noOfPossibleMoves = GetNoOfSetBits(GetSystemMovers(currentPos)) + GetNoOfSetBits(GetSystemLeftCaptures(currentPos)) + GetNoOfSetBits(GetSystemRightCaptures(currentPos));

        if (noOfPossibleMoves == 0)
            return MOVE_STATUS::USER_WINS;

        return MOVE_STATUS::MOVE_OK;
    }
}

/* evaluation function
 * Currently only checks material count*/
int32_t PawnChessEngine::EvaluatePosition(ChessBoard currentPos, bool userToMove)
{
    int32_t iRet = 0;
    if (userToMove)
    {
        /*reward for mobility and captures*/
       // iRet += (GetNoOfSetBits(GetUserMovers(currentPos)) * 5 + GetNoOfSetBits(GetUserCapturesLeft(currentPos)) * 10 + GetNoOfSetBits(GetUserCapturesRight(currentPos)) *10);


        /* get the material count of the pawns*/
        //iRet += (GetNoOfSetBits(GetUserMovers(currentPos)) - GetNoOfSetBits(GetSystemMovers(currentPos))) * 10;

        iRet += (GetNoOfSetBits(currentPos.UserPawns) - GetNoOfSetBits(currentPos.SystemPawns)) * PAWN_VALUE;
    }
    else
    {
        //iRet += (GetNoOfSetBits(GetSystemMovers(currentPos)) - GetNoOfSetBits(GetUserMovers(currentPos)) )* 10;
        iRet += (GetNoOfSetBits(currentPos.SystemPawns) - GetNoOfSetBits(currentPos.UserPawns)) * PAWN_VALUE;
    }

    return iRet;
}

int32_t PawnChessEngine::EvaluatePositionEx(ChessBoard currentPos, bool userToMove)
{
    /*improved version of the eval used with minmaxEX
     Evaluated from whites point of view;negate for black*/
    int32_t iRet = 0;


    /*reward for mobility*/
    iRet += (GetNoOfSetBits(GetUserMovers(currentPos)) - GetNoOfSetBits(GetSystemMovers(currentPos))) * 100;

    /* get the material count of the pawns*/
    iRet += (GetNoOfSetBits(currentPos.UserPawns) - GetNoOfSetBits(currentPos.SystemPawns)) * PAWN_VALUE;


    /*position based scores for unblocked pieces*/
    iRet += GetNoOfSetBits(((RANK_2_MASK & currentPos.UserPawns) << 5) & (~(currentPos.SystemPawns & RANK_3_MASK))) * 5;
    iRet += GetNoOfSetBits(((RANK_3_MASK & currentPos.UserPawns) << 5) & (~(currentPos.SystemPawns & RANK_4_MASK))) * 10;
    iRet += GetNoOfSetBits(((RANK_4_MASK & currentPos.UserPawns) << 5) & (~(currentPos.SystemPawns & RANK_5_MASK))) * 2000;
    iRet += GetNoOfSetBits(((RANK_5_MASK & currentPos.UserPawns) << 5) & (~(currentPos.SystemPawns & RANK_6_MASK))) * 50000;

    iRet += GetNoOfSetBits(((RANK_2_MASK & currentPos.SystemPawns) >> 5) & (~(currentPos.UserPawns & RANK_1_MASK))) * -50000;
    iRet += GetNoOfSetBits(((RANK_3_MASK & currentPos.SystemPawns) >> 5) & (~(currentPos.UserPawns & RANK_2_MASK))) * -2000;
    iRet += GetNoOfSetBits(((RANK_4_MASK & currentPos.UserPawns) >> 5) & (~(currentPos.UserPawns & RANK_3_MASK))) * -10;
    iRet += GetNoOfSetBits(((RANK_5_MASK & currentPos.UserPawns) >> 5) & (~(currentPos.UserPawns & RANK_4_MASK))) * -5;


    if (!userToMove)
        iRet = iRet * -1;
    return iRet;
}

int32_t PawnChessEngine::MinMaxEx(ChessBoard currentPos, bool userToMove, int currentDepth, int depthToSearch, int alpha, int beta,
    int32_t(*evalFunction)(ChessBoard, bool))
{
    /*supposed to be the improved version with the inclusion of alpha beta */

    /*Say if all the moves are loosing moves.. all nodes return -iNFINITY according to the algorithm.. we still need to play a move
     This flag detects whether a move has been selected for the UI*/
    bool uiMoveSelected = false;

    int32_t current_move_score;
    int32_t best_score = -INFINITY32;
    if (userToMove)
    {
        /*if a white pawn is in the 6th rank its a win for white! no need to check anything else*/
        if ((RANK_6_MASK & currentPos.UserPawns) != 0)
            return INFINITY32;
        /*if a black pawn is in the first rank its a win for black*/
        if ((RANK_1_MASK & currentPos.SystemPawns) != 0)
            return -INFINITY32;
    }
    else
    {
        /*if a black pawn is in the first rank its a win for black*/
        if ((RANK_1_MASK & currentPos.SystemPawns) != 0)
            return INFINITY32;


        /*if a white pawn is in the 6th rank its a win for white! no need to check anything else*/
        if ((RANK_6_MASK & currentPos.UserPawns) != 0)
            return -INFINITY32;
    }

    if (currentDepth == 0)
        return evalFunction(currentPos, userToMove);

    std::vector<ChessBoard> moves = PawnChessEngine::GenerateMoves(currentPos, userToMove);

    /*if the side to move doesnt have any moves to play it has lost*/
    if (moves.empty())
        return -INFINITY32;

    //----------------------END OF TERMINAL NODE DETECTION AREA-------------------------------------------

    for (int i = 0; i < moves.size(); i++)
    {
        current_move_score = -MinMaxEx(moves[i], !userToMove, currentDepth - 1, depthToSearch, -beta, -alpha, evalFunction);

        if (current_move_score > best_score)
        {
            best_score = current_move_score;

            /* ----------------START OF UI MOVE SELECTION CODE-THIS IS SPECIFIC TO THIS APPLICATION AND IS NOT PART OF THE STANDARD ALPHA-BETA NEGAMAX ALGORITHM*/
            if (currentDepth == depthToSearch)
            {
                /*update the global variable that is used by the UI to update the bitboard
                 * Update the Move Selected flag to true
                 */
                ReplyMove = moves[i];
                uiMoveSelected = true;
            }
            /* ---------------------------------------END OF UI SELECTION CODE-------------------------------------------------------------- ------------------*/

        }

        /* ----------------START OF UI MOVE SELECTION CODE-THIS IS SPECIFIC TO THIS APPLICATION AND IS NOT PART OF THE STANDARD ALPHA-BETA NEGAMAX ALGORITHM*/
                        /*if we're are on the last move on the moves[] array and we still haven't picked a move to play,
                         * We need to do it NOW! */
        if (currentDepth == depthToSearch)
        {
            if (i == (moves.size() - 1))
            {
                if (!uiMoveSelected)
                {
                    /*select the first move for now; later we could do a evaluation
                     * function and scientifically select the least worst move
                     */
                    MinMaxEx(currentPos, userToMove, 1, 1, -INFINITY32, INFINITY32, EvaluatePositionEx);
                    // ReplyMove = moves[0];
                    uiMoveSelected = true;
                }
            }
        }
        /* ---------------------------------------END OF UI SELECTION CODE-------------------------------------------------------------- ------------------*/

        if (best_score > alpha)
            alpha = best_score;

        /*Beta cut off*/
        if (alpha >= beta)
            break;

    }

    return best_score;
}