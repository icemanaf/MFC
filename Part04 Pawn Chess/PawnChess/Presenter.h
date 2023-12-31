#pragma once
#include "ChessEngine.h"
#include <gdiplus.h>
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

#define ROWS 6
#define COLS 6

enum CHESS_SQUARE
{
	EMPTY,
	SYSTEM_PAWN,
	USER_PAWN
};



class Presenter
{
private:
	
	CHESS_SQUARE m_displayArray[ROWS][COLS]= {
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
		{EMPTY,EMPTY,EMPTY,EMPTY,EMPTY},
	};

	bool m_UserPlaysWhite = true;

	SolidBrush* m_lightBrush;
	SolidBrush* m_darkBrush;
	Bitmap* m_BlackPawn;
	Bitmap* m_WhitePawn;
	const double m_ImageScaleFactor = 0.75;
	int m_chessSquareHeight = 0;
	int m_chessSquareWidth = 0;
	int m_ply = 14;
	
public:

	Presenter();

	~Presenter();

	void DisplayBoard(CDC* pdc,CRect clientRect);

	/// <summary>
	/// returns the row and col of the selected square that falls into xMousePos and yMousePos
	/// </summary>
	/// <param name="xMousePos"></param>
	/// <param name="yMousePos"></param>
	/// <returns></returns>
	std::tuple<int, int> GetSelectedSquare(int xMousePos, int  yMousePos);

	MOVE_STATUS Move(int startSquare, int targetSquare);

	CHESS_SQUARE GetSquareAtPos(int row, int col);

	void UpdateSquareAtPos(int row,int col,CHESS_SQUARE item);

	void SetBoard(ChessBoard board);

	bool ValidateMove(ChessBoard prev, ChessBoard now);

	void SetUserPlaysBlack(bool UserPlaysBlack);

	bool UserPlaysBlack();

	ChessBoard GetBoard();

	void SetPly(int ply);

	int GetPly();

	void ResetBoard();
};

