
// MFCSearchingFilesDoc.cpp : implementation of the CMFCSearchingFilesDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MFCSearchingFiles.h"
#endif

#include "MFCSearchingFilesDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

///////////////////////////////////////////////////////////////////////////
// CSquaresDoc

IMPLEMENT_DYNCREATE(CSquaresDoc, CDocument)

BEGIN_MESSAGE_MAP(CSquaresDoc, CDocument)
	//{{AFX_MSG_MAP(CSquaresDoc)
	ON_COMMAND(ID_COLOR_RED, OnColorRed)
	ON_COMMAND(ID_COLOR_YELLOW, OnColorYellow)
	ON_COMMAND(ID_COLOR_GREEN, OnColorGreen)
	ON_COMMAND(ID_COLOR_CYAN, OnColorCyan)
	ON_COMMAND(ID_COLOR_BLUE, OnColorBlue)
	ON_COMMAND(ID_COLOR_WHITE, OnColorWhite)
	ON_UPDATE_COMMAND_UI(ID_COLOR_RED, OnUpdateColorRed)
	ON_UPDATE_COMMAND_UI(ID_COLOR_YELLOW, OnUpdateColorYellow)
	ON_UPDATE_COMMAND_UI(ID_COLOR_GREEN, OnUpdateColorGreen)
	ON_UPDATE_COMMAND_UI(ID_COLOR_CYAN, OnUpdateColorCyan)
	ON_UPDATE_COMMAND_UI(ID_COLOR_BLUE, OnUpdateColorBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_WHITE, OnUpdateColorWhite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


// CMFCSearchingFilesDoc construction/destruction

CSquaresDoc::CSquaresDoc()
{
	// TODO: add one-time construction code here

}

CSquaresDoc::~CSquaresDoc()
{
}

BOOL CSquaresDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	for (int i = 0; i<4; i++)
		for (int j = 0; j<4; j++)
			m_clrGrid[i][j] = RGB(255, 255, 255);

	m_clrCurrentColor = RGB(255, 0, 0);
	return TRUE;
}



///////////////////////////////////////////////////////////////////////////
// CSquaresDoc serialization

void CSquaresDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		for (int i = 0; i<4; i++)
			for (int j = 0; j<4; j++)
				ar << m_clrGrid[i][j];
		ar << m_clrCurrentColor;
	}
	else
	{
		for (int i = 0; i<4; i++)
			for (int j = 0; j<4; j++)
				ar >> m_clrGrid[i][j];
		ar >> m_clrCurrentColor;
	}
}


// CMFCSearchingFilesDoc serialization

COLORREF CSquaresDoc::GetCurrentColor()
{
	return m_clrCurrentColor;
}

COLORREF CSquaresDoc::GetSquare(int i, int j)
{
	ASSERT(i >= 0 && i <= 3 && j >= 0 && j <= 3);
	return m_clrGrid[i][j];
}

void CSquaresDoc::SetSquare(int i, int j, COLORREF color)
{
	ASSERT(i >= 0 && i <= 3 && j >= 0 && j <= 3);
	m_clrGrid[i][j] = color;
	SetModifiedFlag(TRUE);
	UpdateAllViews(NULL);
}

void CSquaresDoc::OnColorRed()
{
	m_clrCurrentColor = RGB(255, 0, 0);
}

void CSquaresDoc::OnColorYellow()
{
	m_clrCurrentColor = RGB(255, 255, 0);
}

void CSquaresDoc::OnColorGreen()
{
	m_clrCurrentColor = RGB(0, 255, 0);
}

void CSquaresDoc::OnColorCyan()
{
	m_clrCurrentColor = RGB(0, 255, 255);
}

void CSquaresDoc::OnColorBlue()
{
	m_clrCurrentColor = RGB(0, 0, 255);
}

void CSquaresDoc::OnColorWhite()
{
	m_clrCurrentColor = RGB(255, 255, 255);
}

void CSquaresDoc::OnUpdateColorRed(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_clrCurrentColor == RGB(255, 0, 0));
}

void CSquaresDoc::OnUpdateColorYellow(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_clrCurrentColor == RGB(255, 255, 0));
}

void CSquaresDoc::OnUpdateColorGreen(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_clrCurrentColor == RGB(0, 255, 0));
}

void CSquaresDoc::OnUpdateColorCyan(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_clrCurrentColor == RGB(0, 255, 255));
}

void CSquaresDoc::OnUpdateColorBlue(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_clrCurrentColor == RGB(0, 0, 255));
}

void CSquaresDoc::OnUpdateColorWhite(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(m_clrCurrentColor == RGB(255, 255, 255));
}


#ifdef SHARED_HANDLERS

// Support for thumbnails
void CSquaresDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CSquaresDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"
	strSearchContent = _T("point;rectangle;circle;ole object;");
	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CSquaresDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CMFCSearchingFilesDoc diagnostics

#ifdef _DEBUG
void CSquaresDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSquaresDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMFCSearchingFilesDoc commands
