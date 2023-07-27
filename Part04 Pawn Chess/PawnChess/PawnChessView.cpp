
// PawnChessView.cpp : implementation of the CPawnChessView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PawnChess.h"
#endif

#include "PawnChessDoc.h"
#include "PawnChessView.h"


#include <gdiplus.h>
#include <tuple>
#include "CSettingsDlg.h"

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPawnChessView

IMPLEMENT_DYNCREATE(CPawnChessView, CView)

BEGIN_MESSAGE_MAP(CPawnChessView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_NEW_GAME, &CPawnChessView::OnNewGame)
	ON_COMMAND(ID_USER_PLAYS_BLACK, &CPawnChessView::OnUserPlaysBlack)
	ON_UPDATE_COMMAND_UI(ID_USER_PLAYS_BLACK, &CPawnChessView::OnUpdateCommandUI)
	ON_COMMAND(ID_GAME_SETTINGS, &CPawnChessView::OnSettingsClicked)
END_MESSAGE_MAP()

// CPawnChessView construction/destruction

CPawnChessView::CPawnChessView() noexcept
{
	// TODO: add construction code here

	 m_bDragging=FALSE;

}

CPawnChessView::~CPawnChessView()
{
	
}

BOOL CPawnChessView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	

	return CView::PreCreateWindow(cs);
}

// CPawnChessView drawing

void CPawnChessView::OnDraw(CDC* pdc)
{

	CRect client;
	GetClientRect(&client);

	m_Presenter.DisplayBoard(pdc,client);

	return;

	CPawnChessDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CPawnChessView diagnostics

#ifdef _DEBUG
void CPawnChessView::AssertValid() const
{
	CView::AssertValid();
}

void CPawnChessView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}



CPawnChessDoc* CPawnChessView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPawnChessDoc)));
	return (CPawnChessDoc*)m_pDocument;
}
#endif //_DEBUG


/// <summary>
/// check if this is a winning position
/// </summary>
/// <param name="pos"></param>
/// <returns></returns>
MOVE_STATUS CPawnChessView::CheckWinner(ChessBoard pos)
{
	MOVE_STATUS winStatus = MOVE_OK;

	if (PawnChessEngine::DetectWinLoss(pos, false) == SYSTEM_WINS)
	{
		winStatus = SYSTEM_WINS;
	}
	else if (PawnChessEngine::DetectWinLoss(pos, true) == USER_WINS)
	{
		winStatus = USER_WINS;
	}

	return winStatus;
}




// CPawnChessView message handlers


void CPawnChessView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	m_bDragging = TRUE;
	
	SetCapture();
	SetCursor(::LoadCursor(NULL, IDC_HAND));

	CPoint mousePos;

	GetCursorPos(&mousePos);

	ScreenToClient(&mousePos);

	 m_userSelStartSquare= m_Presenter.GetSelectedSquare(mousePos.x, mousePos.y);

	CView::OnLButtonDown(nFlags, point);
}


void CPawnChessView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	MOVE_STATUS winResult = MOVE_OK; // Lazy way of tracking this

	CPoint mousePos;

	GetCursorPos(&mousePos);

	ScreenToClient(&mousePos);

	 m_userSelEndSquare = m_Presenter.GetSelectedSquare(mousePos.x, mousePos.y);
	
	 if (m_userSelEndSquare != m_userSelStartSquare)
	 {
		 auto item = m_Presenter.GetSquareAtPos(std::get<0>(m_userSelStartSquare), std::get<1>(m_userSelStartSquare));

		 auto prev_pos = m_Presenter.GetBoard();

		 m_Presenter.UpdateSquareAtPos(std::get<0>(m_userSelStartSquare), std::get<1>(m_userSelStartSquare), EMPTY);

		 m_Presenter.UpdateSquareAtPos(std::get<0>(m_userSelEndSquare), std::get<1>(m_userSelEndSquare),item);

		 auto current_pos = m_Presenter.GetBoard();


		 if (!m_Presenter.ValidateMove(prev_pos, current_pos))
		 {
			 //illegal move ,
			 CWnd::MessageBox(_T("Illegal move!"));

			 //revert
			 m_Presenter.SetBoard(prev_pos);
			 m_bDragging = FALSE;
			 return;

		 }
		 else
		 {
			 //do minmax here

			 winResult = CheckWinner(current_pos);

			 if (!winResult)
			 {
				 int depthToSearch = m_Presenter.GetPly();

				 PawnChessEngine::MinMaxEx(current_pos, false, depthToSearch, depthToSearch, -INFINITY32, INFINITY32, PawnChessEngine::EvaluatePosition);

				 m_Presenter.SetBoard(PawnChessEngine::ReplyMove);

				 MOVE_STATUS replyMoveResult = PawnChessEngine::DetectWinLoss(PawnChessEngine::ReplyMove, false);
				 if (replyMoveResult == SYSTEM_WINS)
				 {
					 winResult = SYSTEM_WINS;
				 }
				 else if (replyMoveResult == USER_WINS)
				 {
					 winResult = USER_WINS;
				 }

				 winResult = CheckWinner(PawnChessEngine::ReplyMove);

			 }

		 }

	 }


	m_bDragging = FALSE;
	ReleaseCapture();
	Invalidate(FALSE);

	if (winResult == SYSTEM_WINS)
	{
		CWnd::MessageBox(_T("System Wins!"));
	}
	if (winResult == USER_WINS)
	{
		CWnd::MessageBox(_T("User Wins!"));
	}
	

	CView::OnLButtonUp(nFlags, point);
}

void CPawnChessView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	if (m_bDragging == TRUE)
	{
		CPoint mousePos;

		GetCursorPos(&mousePos);

		ScreenToClient(&mousePos);
		
	}


	CView::OnMouseMove(nFlags, point);
}


void CPawnChessView::OnNewGame()
{
	m_Presenter.ResetBoard();

	auto status = m_Presenter.UserPlaysBlack();

	if (status)
	{
		//the machine is now white so it has to move first.

		auto current_pos = m_Presenter.GetBoard();

		int depthToSearch = m_Presenter.GetPly();

		PawnChessEngine::MinMaxEx(current_pos, false, depthToSearch, depthToSearch, -INFINITY32, INFINITY32, PawnChessEngine::EvaluatePosition);

		m_Presenter.SetBoard(PawnChessEngine::ReplyMove);

	}
	

	Invalidate(FALSE);
}


void CPawnChessView::OnUserPlaysBlack()
{
	auto ret =	CWnd::MessageBox(_T("This will reset the game"),_T("Swap sides?"), MB_YESNO | MB_DEFBUTTON2 | MB_ICONQUESTION);

	if (ret == IDYES)
	{
		auto status= m_Presenter.UserPlaysBlack();

		m_Presenter.SetUserPlaysBlack(!status);

		if (!status)
		{
			//the machine is now playing white so it has to move first.

			auto current_pos = m_Presenter.GetBoard();

			int depthToSearch = m_Presenter.GetPly();

			PawnChessEngine::MinMaxEx(current_pos, false, depthToSearch, depthToSearch, -INFINITY32, INFINITY32, PawnChessEngine::EvaluatePosition);

			m_Presenter.SetBoard(PawnChessEngine::ReplyMove);

		}

		Invalidate(FALSE);
	}
}


void CPawnChessView::OnUpdateCommandUI(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_Presenter.UserPlaysBlack()?1:0);
}


void CPawnChessView::OnSettingsClicked()
{
	// TODO: Add your command handler code here
	CSettingsDlg dialog;

	//get ply
	dialog.SetPly(m_Presenter.GetPly());

	dialog.DoModal();

	auto ply = dialog.GetPly();

	if (ply > 0)
	{
		//set ply
		m_Presenter.SetPly(ply);

	}


}
