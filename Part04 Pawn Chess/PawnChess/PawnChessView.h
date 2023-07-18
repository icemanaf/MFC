
// PawnChessView.h : interface of the CPawnChessView class
//

#pragma once


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

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PawnChessView.cpp
inline CPawnChessDoc* CPawnChessView::GetDocument() const
   { return reinterpret_cast<CPawnChessDoc*>(m_pDocument); }
#endif

