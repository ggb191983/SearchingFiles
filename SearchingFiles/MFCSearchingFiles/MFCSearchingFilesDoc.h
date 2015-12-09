
// MFCSearchingFilesDoc.h : interface of the CMFCSearchingFilesDoc class
//


#pragma once
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSquaresDoc : public CDocument
{
protected: // create from serialization only
	CSquaresDoc();
	DECLARE_DYNCREATE(CSquaresDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	void SetSquare(int i, int j, COLORREF color);
	COLORREF GetSquare(int i, int j);
	COLORREF GetCurrentColor();
	virtual ~CSquaresDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	COLORREF m_clrCurrentColor;
	COLORREF m_clrGrid[4][4];
	//{{AFX_MSG(CSquaresDoc)
	afx_msg void OnColorRed();
	afx_msg void OnColorYellow();
	afx_msg void OnColorGreen();
	afx_msg void OnColorCyan();
	afx_msg void OnColorBlue();
	afx_msg void OnColorWhite();
	afx_msg void OnUpdateColorRed(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorYellow(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorGreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorCyan(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorBlue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorWhite(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP();

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
