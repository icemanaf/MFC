#include <inttypes.h>
#include <vector>

typedef struct Board
{
    uint64_t WhitePawns;
    uint64_t BlackPawns;
}   ChessBoard;


enum MOVE_STATUS
{
    MOVE_OK,
    MOVE_ILLEGAL,
    USER_WINS,
    SYSTEM_WINS
};


std::vector<uint64_t> GenerateMoves(ChessBoard currentPos, bool userToMove);