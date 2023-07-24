#include "pch.h"
#include "Presenter.h"
#include <tuple>

//constructor
Presenter::Presenter()
{
    //set the chess square colors
    Color darkSquare(100, 100, 100);

    Color lightSquare(200, 200, 200);

    m_lightBrush = new SolidBrush(lightSquare);

    m_darkBrush = new SolidBrush(darkSquare);

    m_WhitePawn = new Bitmap(_T("wp.png"), FALSE);

    m_BlackPawn = new Bitmap(_T("bp.png"), FALSE);

    m_UserIsWhite = true;

     ResetBoard();

}


//destructor
Presenter::~Presenter()
{
    delete m_lightBrush;

    delete m_darkBrush;

    delete m_WhitePawn;

    delete m_BlackPawn;
}

void Presenter::DisplayBoard(CDC* pdc,CRect clientRect)
{

    Graphics g(*pdc);

    g.SetCompositingMode(CompositingModeSourceOver);
    g.SetCompositingQuality(CompositingQualityHighSpeed);
    g.SetPixelOffsetMode(PixelOffsetModeHighSpeed);
    g.SetSmoothingMode(SmoothingModeHighSpeed);
    g.SetInterpolationMode(InterpolationModeHighQuality);
   
   
  

    m_chessSquareHeight = clientRect.Height() / ROWS;

    m_chessSquareWidth = clientRect.Width() /COLS;

    CSize size(m_chessSquareWidth, m_chessSquareHeight);

    bool colorToggle = false;
    

        for (int row = 0;row < ROWS;row++)
        {
            colorToggle = !colorToggle;

            for (int col = 0;col < COLS;col++)
            {
                g.FillRectangle(&(colorToggle ? *m_lightBrush : *m_darkBrush), col * m_chessSquareWidth, row * m_chessSquareHeight, m_chessSquareWidth, m_chessSquareHeight);
                
                colorToggle = !colorToggle;

                auto status = m_displayArray[5-row][col];

                switch (status) 
                {
                    case USER_PAWN:
                        m_UserIsWhite? g.DrawImage(m_WhitePawn, col * m_chessSquareWidth + 20, row * m_chessSquareHeight, m_chessSquareWidth * m_ImageScaleFactor, m_chessSquareHeight * m_ImageScaleFactor):
                            g.DrawImage(m_BlackPawn, col * m_chessSquareWidth + 20, row * m_chessSquareHeight, m_chessSquareWidth * m_ImageScaleFactor, m_chessSquareHeight * m_ImageScaleFactor);
                        break;

                    case SYSTEM_PAWN:
                        m_UserIsWhite? g.DrawImage(m_BlackPawn, col * m_chessSquareWidth + 20, row  * m_chessSquareHeight, m_chessSquareWidth * m_ImageScaleFactor, m_chessSquareHeight * m_ImageScaleFactor): 
                            g.DrawImage(m_WhitePawn, col * m_chessSquareWidth + 20, row * m_chessSquareHeight, m_chessSquareWidth * m_ImageScaleFactor, m_chessSquareHeight * m_ImageScaleFactor);
                        break;
                   
                }
            }
        }

}

//returns the square in row col format referenced to the 2d display array
std::tuple<int,int> Presenter::GetSelectedSquare(int xMousePos, int yMousePos)
{
   
    int row = (m_chessSquareHeight*ROWS -  yMousePos) / m_chessSquareHeight;

    int col = xMousePos/ m_chessSquareWidth ;

    return std::make_tuple(row, col);
}

MOVE_STATUS Presenter::Move(int startSquare, int targetSquare)
{
    return MOVE_STATUS();
}

CHESS_SQUARE Presenter::GetItemAtPos(int row, int col)
{
    return m_displayArray[row][col];
}

void Presenter::SetItemAtPos(int row, int col, CHESS_SQUARE item)
{
    m_displayArray[row][col] = item;
}

void Presenter::SetBoard(ChessBoard board)
{
    int row = 0;
    int col = 0;
    uint64_t bitCounter = 0x01;


    for (int i = 0;i < (ROWS * COLS); i++)
    {
        m_displayArray[row][col] = EMPTY;

        if ((bitCounter & board.UserPawns) != 0)
        {
            m_displayArray[row][col] = USER_PAWN;
        }

        if ((bitCounter & board.SystemPawns) != 0)
        {
            m_displayArray[row][col] = SYSTEM_PAWN;
        }

        col++;

        bitCounter = bitCounter << 1;

        if (col >= COLS)
        {
            col = 0;
            row++;
        }
    }

}

bool Presenter::ValidateMove(ChessBoard now, ChessBoard proposed)
{
    return PawnChessEngine::ValidateMove(now, proposed,true);
}

ChessBoard Presenter::GetBoard()
{
    ChessBoard ret{};

    uint64_t bitCounter = 0x01;
    

    for (int row = 0; row < ROWS;row++)
    {
        for(int col=0 ; col<COLS; col++)
        {
            if (m_displayArray[row][col] == SYSTEM_PAWN)
            {
                ret.SystemPawns = ret.SystemPawns | bitCounter;
            }
            else if (m_displayArray[row][col] == USER_PAWN)
            {
                ret.UserPawns = ret.UserPawns | bitCounter;
            }

            bitCounter = bitCounter << 1;
        }
    }
         

    return ret;
}



void Presenter::ResetBoard()
{
    //clear the board first
    for (int row = 0; row < ROWS;row++)
    {
        for (int col = 0;col < COLS;col++)
        {
            m_displayArray[row][col] = EMPTY;
        }
    }

    //the human player starts from the bottom row , so set the white there
  
        m_displayArray[0][0] = USER_PAWN;
        m_displayArray[0][1] = USER_PAWN;
        m_displayArray[0][2] = USER_PAWN;
        m_displayArray[0][3] = USER_PAWN;
        m_displayArray[0][4] = USER_PAWN;
        m_displayArray[0][5] = USER_PAWN;

        m_displayArray[5][0] = SYSTEM_PAWN;
        m_displayArray[5][1] = SYSTEM_PAWN;
        m_displayArray[5][2] = SYSTEM_PAWN;
        m_displayArray[5][3] = SYSTEM_PAWN;
        m_displayArray[5][4] = SYSTEM_PAWN;
        m_displayArray[5][5] = SYSTEM_PAWN;
   

}


