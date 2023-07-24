#include "pch.h"
#include "Presenter.h"


//constructor
Presenter::Presenter()
{

    Color darkSquare(100, 100, 100);

    Color lightSquare(200, 200, 200);

    m_lightBrush = new SolidBrush(lightSquare);

    m_darkBrush = new SolidBrush(darkSquare);

    m_WhitePawn = new Image(_T("wp.png"), TRUE);

    m_BlackPawn = new Image(_T("bp.png"), TRUE);

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


    int sqHeight = clientRect.Height() / 6;

    int sqWidth = clientRect.Width() /6;

    CSize size(sqWidth, sqHeight);

    bool colorToggle = false;
    

        for (int row = 0;row < 6;row++)
        {
            colorToggle = !colorToggle;

            for (int col = 0;col < 6;col++)
            {
                g.FillRectangle(&(colorToggle ? *m_lightBrush : *m_darkBrush), col * sqWidth, row * sqHeight, sqWidth, sqHeight);
                

                colorToggle = !colorToggle;

                auto status = m_displayArray[5-row][col];

                switch (status) 
                {
                    case WHITE_PAWN:
                        g.DrawImage(m_WhitePawn, col * sqWidth + 20, row * sqHeight, sqWidth * m_ImageScaleFactor, sqHeight * m_ImageScaleFactor);
                        break;

                    case BLACK_PAWN:
                        g.DrawImage(m_BlackPawn, col * sqWidth + 20, row  * sqHeight, sqWidth * m_ImageScaleFactor, sqHeight * m_ImageScaleFactor);
                        break;
                   
                }
            }
        }

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
    //clear the board first
    for (int row = 0; row < 6;row++)
    {
        for (int col = 0;col < 6;col++)
        {
            m_displayArray[row][col] = EMPTY;
        }
    }

    //the human player starts from the bottom row , so set the white there
    if (m_UserIsWhite)
    {
        m_displayArray[0][0] = WHITE_PAWN;
        m_displayArray[0][1] = BLACK_PAWN;
        m_displayArray[0][2] = WHITE_PAWN;
        m_displayArray[0][3] = WHITE_PAWN;
        m_displayArray[0][4] = WHITE_PAWN;
        m_displayArray[0][5] = WHITE_PAWN;

        m_displayArray[5][0] = BLACK_PAWN;
        m_displayArray[5][1] = BLACK_PAWN;
        m_displayArray[5][2] = BLACK_PAWN;
        m_displayArray[5][3] = BLACK_PAWN;
        m_displayArray[5][4] = BLACK_PAWN;
        m_displayArray[5][5] = WHITE_PAWN;
    }
    else
    {
        m_displayArray[0][0] = BLACK_PAWN;
        m_displayArray[0][1] = BLACK_PAWN;
        m_displayArray[0][2] = BLACK_PAWN;
        m_displayArray[0][3] = BLACK_PAWN;
        m_displayArray[0][4] = BLACK_PAWN;
        m_displayArray[0][5] = BLACK_PAWN;

        m_displayArray[5][0] = WHITE_PAWN;
        m_displayArray[5][1] = WHITE_PAWN;
        m_displayArray[5][2] = WHITE_PAWN;
        m_displayArray[5][3] = WHITE_PAWN;
        m_displayArray[5][4] = WHITE_PAWN;
        m_displayArray[5][5] = WHITE_PAWN;
    }

}


