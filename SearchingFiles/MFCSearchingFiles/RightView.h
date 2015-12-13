#pragma once


// RightView view
typedef struct tagITEMINFO {
	CString     strFileName;
	DWORD       nFileSizeLow;
	FILETIME    ftLastWriteTime;
} ITEMINFO;


class CRightView : public CListView
{
	DECLARE_DYNCREATE(CRightView)

public:
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,
		LPARAM lParamSort);

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	int Refresh(LPCTSTR pszPath);
	CSquaresDoc* GetDocument();

protected:
	CRightView();           // protected constructor used by dynamic creation
	virtual ~CRightView();
	virtual void OnInitialUpdate(); // called first time after construct
	
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	CString m_strPath;
	void FreeItemMemory();
	BOOL AddItem(int nIndex, WIN32_FIND_DATA* pfd);
	CImageList m_ilSmall;
	CImageList m_ilLarge;
	//{{AFX_MSG(CFileView)
	afx_msg void OnDestroy();
	afx_msg void OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnViewLargeIcons();
	afx_msg void OnViewSmallIcons();
	afx_msg void OnViewList();
	afx_msg void OnViewDetails();
	afx_msg void OnUpdateViewLargeIcons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewSmallIcons(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewList(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewDetails(CCmdUI* pCmdUI);
	afx_msg void OnFileNewDirectory();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in FileView.cpp
inline CSquaresDoc* CRightView::GetDocument(){
	return (CSquaresDoc*)m_pDocument;
}
#endif
