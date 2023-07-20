#include <inttypes.h>

struct ChessBoard
{
	 uint64_t WhitePawns;

	 uint64_t BlackPawns;
};


enum MOVE_STATUS
{
    MOVE_OK,
    MOVE_ILLEGAL,
    USER_WINS,
    SYSTEM_WINS
};
