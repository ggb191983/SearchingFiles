
// LeftView.h : interface of the CLeftView class
//


#pragma once

class CSquaresDoc;

class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView);

// Attributes
public:
	CSquaresDoc* GetDocument();

// Operations
public:

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	BOOL AddDriveItem(LPCTSTR pszDrive);
	int AddDirectories(HTREEITEM hItem, LPCTSTR pszPath);
	void DeleteAllChildren(HTREEITEM hItem);
	void DeleteFirstChild(HTREEITEM hItem);
	CString GetPathFromItem(HTREEITEM hItem);
	BOOL SetButtonState(HTREEITEM hItem, LPCTSTR pszPath);
	int AddDrives();
	CImageList m_ilDrives;

	//{{AFX_MSG(CDriveView)
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CSquaresDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CSquaresDoc*>(m_pDocument); }
#endif

