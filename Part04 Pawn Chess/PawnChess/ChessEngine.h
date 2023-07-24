#include <inttypes.h>
#include <vector>

typedef struct Board
{
    uint64_t UserPawns; 
    uint64_t SystemPawns;
}   ChessBoard;


enum MOVE_STATUS
{
    MOVE_OK,
    MOVE_ILLEGAL,
    USER_WINS,
    SYSTEM_WINS
};

const int32_t INFINITY32 = 2147483647; // INFINITY was taken
static ChessBoard ReplyMove;

class PawnChessEngine
{
public:
    static std::vector<ChessBoard> GenerateMoves(ChessBoard currentPos, bool userToMove);
    static bool IsPositionLegal(ChessBoard currentPos);
    static bool ValidateMove(ChessBoard currentPos, ChessBoard moveAfterPos, bool userToMove);
    static int32_t EvaluatePosition(ChessBoard currentPos, bool userToMove);
    static int32_t MinMaxEx(ChessBoard currentPos, bool userToMove, int currentDepth, int depthToSearch, int alpha, int beta,
        int32_t (*evalFunction)(ChessBoard, bool));

private:
    static uint64_t GetUserMovers(ChessBoard currentPos);
    static uint64_t GetSystemMovers(ChessBoard currentPos);
    static uint64_t GetUserCapturesRight(ChessBoard currentPos);
    static uint64_t GetSystemRightCaptures(ChessBoard currentPos);
    static uint64_t GetUserCapturesLeft(ChessBoard currentPos);
    static uint64_t GetSystemLeftCaptures(ChessBoard currentPos);
    static uint8_t GetNoOfSetBits(uint64_t bitBoard);
    static int32_t EvaluatePositionEx(ChessBoard currentPos, bool userToMove);

};

