#include "pch.h"
#include "Presenter.h"



Presenter::Presenter()
{
   
}

void Presenter::DisplayBoard(CDC* pdc)
{
}

int Presenter::GetSelectedSquare(int xMousePos, int yMousePos)
{
    return 0;
}

MOVE_STATUS Presenter::Move(int startSquare, int targetSquare)
{
    return MOVE_STATUS();
}

ChessBoard Presenter::ConvertFromArray(std::vector<int> boardArray)
{
    return ChessBoard();
}

void Presenter::ResetBoard()
{
    //clear the board
    for (int row = 0; row < 6;row++)
    {
        for (int col = 0;col < 6;col++)
        {
            m_displayArray[row][col] = EMPTY;
        }
    }

    //the human player starts from the bottom row , so set the white there
    if (m_WhiteIsUser)
    {
        m_displayArray[0][0] = WHITE_PAWN;
        m_displayArray[0][1] = WHITE_PAWN;
        m_displayArray[0][2] = WHITE_PAWN;
        m_displayArray[0][3] = WHITE_PAWN;
        m_displayArray[0][4] = WHITE_PAWN;
        m_displayArray[0][5] = WHITE_PAWN;

        m_displayArray[5][0] = BLACK_PAWN;
        m_displayArray[5][1] = BLACK_PAWN;
        m_displayArray[5][2] = BLACK_PAWN;
        m_displayArray[5][3] = BLACK_PAWN;
        m_displayArray[5][4] = BLACK_PAWN;
        m_displayArray[5][5] = BLACK_PAWN;
    }
    else
    {

    }

}


