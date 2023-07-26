
// PawnChessView.h : interface of the CPawnChessView class
//

#pragma once
#include "Presenter.h"
#include <tuple>


class CPawnChessView : public CView
{
protected: // create from serialization only
	CPawnChessView() noexcept;
	DECLARE_DYNCREATE(CPawnChessView)

// Attributes
public:
	CPawnChessDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CPawnChessView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	Presenter m_Presenter;
	BOOL m_bDragging;
	std::tuple<int,int> m_userSelStartSquare;
	std::tuple<int, int> m_userSelEndSquare;


protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNewGame();
	afx_msg void OnUserPlaysBlack();
	afx_msg void OnUpdateCommandUI(CCmdUI* pCmdUI);
	afx_msg void OnSettingsClicked();
};



#ifndef _DEBUG  // debug version in PawnChessView.cpp
inline CPawnChessDoc* CPawnChessView::GetDocument() const
   { return reinterpret_cast<CPawnChessDoc*>(m_pDocument); }
#endif

