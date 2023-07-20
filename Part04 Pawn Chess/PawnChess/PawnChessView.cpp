
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
END_MESSAGE_MAP()

// CPawnChessView construction/destruction

CPawnChessView::CPawnChessView() noexcept
{
	// TODO: add construction code here

	 m_bDragging=FALSE;
	 m_xPos=0;
	 m_yPos=0;

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

	
	CSize size(200, 200);
	CPoint point(m_xPos, m_yPos);

	CRect rect(point,size); // left, top, right, bottom
	CBrush brush(RGB(204, 102, 0));   // black color
	pdc->FillRect(&rect, &brush);

	Graphics g(*pdc);
	Pen      pen(Color(255, 0, 0, 255),3.0);
	g.DrawLine(&pen, 0, 0, 1000, 1000);

	

	Image im(_T("bp.png"),TRUE);
	

	g.DrawImage(&im, 500, 500, 200, 200);
	
	


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


// CPawnChessView message handlers


void CPawnChessView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	m_bDragging = TRUE;
	
	SetCapture();
	SetCursor(::LoadCursor(NULL, IDC_HAND));


	CView::OnLButtonDown(nFlags, point);
}


void CPawnChessView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	
	

	m_bDragging = FALSE;
	ReleaseCapture();



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

		m_xPos = mousePos.x;
		m_yPos = mousePos.y;

		Invalidate();
	}


	CView::OnMouseMove(nFlags, point);
}
