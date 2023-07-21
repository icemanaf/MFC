#include <inttypes.h>
#include <vector>

typedef struct Board
{
    uint64_t WhitePawns; // Formerly UserPawns
    uint64_t BlackPawns; // Formerly SystemPawns
}   ChessBoard;


enum MOVE_STATUS
{
    MOVE_OK,
    MOVE_ILLEGAL,
    USER_WINS,
    SYSTEM_WINS
};


static class PawnChessEngine
{
public:
    //const int32_t INFINITY = 2147483647;
    static ChessBoard ReplyMove;

    static std::vector<ChessBoard> GenerateMoves(ChessBoard currentPos, bool userToMove);
    static bool IsPositionLegal(ChessBoard currentPos);

private:
    static uint64_t GetUserMovers(ChessBoard currentPos);
    static uint64_t GetSystemMovers(ChessBoard currentPos);
    static uint64_t GetUserCapturesRight(ChessBoard currentPos);
    static uint64_t GetSystemRightCaptures(ChessBoard currentPos);
    static uint64_t GetUserCapturesLeft(ChessBoard currentPos);
    static uint64_t GetSystemLeftCaptures(ChessBoard currentPos);
    static uint8_t GetNoOfSetBits(uint64_t bitBoard);

};

