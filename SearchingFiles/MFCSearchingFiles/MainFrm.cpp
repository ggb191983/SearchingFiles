
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MFCSearchingFiles.h"
#include "resource.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "RightView.h"
#include "MFCSearchingFilesView.h"
#include "SearchToolBarCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	//ON_EN_CHANGE(IDC_CEDIT_SEARCH, OnChangeCEditSearch)
	ON_EN_CHANGE(IDC_CEDIT_SEARCH, OnChange)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}	
	
	int index = 0;
	while (m_wndToolBar.GetItemID(index) != ID_SEARCH_BUTTON) index++;

	CRect rect;
	
	//next convert that button to a seperator and get its position
	m_wndToolBar.SetButtonInfo(index, ID_SEARCH_BUTTON, TBBS_SEPARATOR, 150);
	m_wndToolBar.GetItemRect(index, &rect);

	rect.top += 2;
	rect.bottom += 200;
	
	if (!m_wndToolBar.m_EditSearch.Create(WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL |
		CBS_DROPDOWNLIST | CBS_HASSTRINGS,
		rect, &m_wndToolBar, IDC_CEDIT_SEARCH))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
	}
	m_wndToolBar.m_EditSearch.ShowWindow(SW_SHOW);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	/*
	CRect rect;
	this->GetClientRect(rect);
	HWND hWndEdit = CreateWindowEx(0L, L"Edit", NULL,
		WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP | WS_BORDER | ES_AUTOHSCROLL,
		rect.right - 200, 0, 150, 20,
		m_wndSearchPane, NULL, AfxGetInstanceHandle(), 0);

	if (!m_wndSearch.Create(WS_CHILD | WS_VISIBLE | SS_LEFTNOWORDWRAP | WS_BORDER | ES_AUTOHSCROLL, CRect(rect.right-100, 4, rect.right - 10, 20), m_wndSearchPane.GetWindow(GW_HWNDLAST), IDC_CEDIT_SEARCH))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}*/
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));	

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(100, 100), pContext) ||
		//!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CSquaresView), CSize(100, 100), pContext))
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CRightView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::OnChange()
{
	if (0x8000 & GetKeyState(VK_RETURN)){
		CString text;
		m_wndSearch.GetWindowTextW(text);
		m_wndSearch.SetWindowTextW(text); // update a label control to match typed text		
	}	
}