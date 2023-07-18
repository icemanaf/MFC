
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPawnChessView

IMPLEMENT_DYNCREATE(CPawnChessView, CView)

BEGIN_MESSAGE_MAP(CPawnChessView, CView)
END_MESSAGE_MAP()

// CPawnChessView construction/destruction

CPawnChessView::CPawnChessView() noexcept
{
	// TODO: add construction code here

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

	pdc->DrawText(_T("Rectangle with 1 call to CDC::Rectangle()"), -1, &client,
		DT_BOTTOM | DT_SINGLELINE | DT_CENTER);


	//CPen penBlack;
	//penBlack.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	//CPen* pOldPen = pdc->SelectObject(&penBlack);

	CRect rect(10, 10, 100, 100); // left, top, right, bottom
	CBrush brush(RGB(204, 102, 0));   // black color
	pdc->FillRect(&rect, &brush);
	
	rect.MoveToX(200);

	pdc->FillRect(&rect, &brush);


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
