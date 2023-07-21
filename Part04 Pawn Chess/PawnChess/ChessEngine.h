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

    static bool IsPositionLegal(ChessBoard currentPos);


};

std::vector<uint64_t> GenerateMoves(ChessBoard currentPos, bool userToMove);
