
// MFCSearchingFilesView.h : interface of the CMFCSearchingFilesView class
//
#include "MFCSearchingFilesDoc.h"
#pragma once


class CSquaresView : public CView
{
protected: // create from serialization only
	CSquaresView();
	DECLARE_DYNCREATE(CSquaresView)

// Attributes
public:
	CSquaresDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// Implementation
public:
	virtual ~CSquaresView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MFCSearchingFilesView.cpp
inline CMFCSearchingFilesDoc* CMFCSearchingFilesView::GetDocument() const
   { return reinterpret_cast<CMFCSearchingFilesDoc*>(m_pDocument); }
#endif

