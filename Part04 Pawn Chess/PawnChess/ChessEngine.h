#include <inttypes.h>

struct ChessBoard
{
    uint64_t BlackPawns; // Formerly SystemPawns
	uint64_t WhitePawns; // Formerly UserPawns
};


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