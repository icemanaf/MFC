#pragma once
#include "ChessEngine.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")



enum CHESS_SQUARE
{
	EMPTY,
	BLACK_PAWN,
	WHITE_PAWN
};



class Presenter
{
private:
	
	CHESS_SQUARE m_displayArray[6][6]= {
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
	};

	bool m_WhiteIsUser = true;
public:
	Presenter();

	void DisplayBoard(CDC* pdc);

	int GetSelectedSquare(int xMousePos, int  yMousePos);

	MOVE_STATUS Move(int startSquare, int targetSquare);

	std::vector<int> ConvertToArray(ChessBoard board);

	ChessBoard ConvertFromArray(std::vector<int> boardArray);

	void ResetBoard();
};

