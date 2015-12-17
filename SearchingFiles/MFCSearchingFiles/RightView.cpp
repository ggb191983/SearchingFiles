// RightView.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSearchingFiles.h"
#include "MFCSearchingFilesDoc.h"
#include "RightView.h"
#include "PathDialog.h"


// RightView

IMPLEMENT_DYNCREATE(CRightView, CListView)

BEGIN_MESSAGE_MAP(CRightView, CListView)
	//{{AFX_MSG_MAP(CFileView)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetDispInfo)
	ON_NOTIFY_REFLECT(LVN_COLUMNCLICK, OnColumnClick)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDoubleClick)
	//ON_COMMAND(ID_VIEW_LARGE_ICONS, OnViewLargeIcons)
	//ON_COMMAND(ID_VIEW_SMALL_ICONS, OnViewSmallIcons)
	ON_COMMAND(ID_VIEW_LIST, OnViewList)
	ON_COMMAND(ID_VIEW_DETAILS, OnViewDetails)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_LARGE_ICONS, OnUpdateViewLargeIcons)
	//ON_UPDATE_COMMAND_UI(ID_VIEW_SMALL_ICONS, OnUpdateViewSmallIcons)
	ON_UPDATE_COMMAND_UI(ID_VIEW_LIST, OnUpdateViewList)
	ON_UPDATE_COMMAND_UI(ID_VIEW_DETAILS, OnUpdateViewDetails)
	ON_COMMAND(ID_FILE_NEW, OnFileNewDirectory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CRightView::CRightView()
{

}

CRightView::~CRightView()
{
}

BOOL CRightView::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CListView::PreCreateWindow(cs))
		return FALSE;

	cs.style &= ~LVS_TYPEMASK;
	cs.style |= LVS_REPORT;
	return TRUE;
}


// RightView diagnostics

#ifdef _DEBUG
void CRightView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CRightView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// RightView message handlers

void CRightView::OnDraw(CDC* pDC)
{
	CSquaresDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

void CRightView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	//
	// Populate the list view with items.
	//
	TCHAR szPath[MAX_PATH];
	::GetCurrentDirectory(sizeof(szPath) / sizeof(TCHAR), szPath);

	//
	// Initialize the image list.
	//
	//m_ilLarge.Create(IDB_LARGEDOC, 32, 1, RGB(255, 0, 255));
	//m_ilSmall.Create(IDB_SMALLDOC, 16, 1, RGB(255, 0, 255));

	HIMAGELIST  hSystemSmallImageList, hSystemLargeImageList;
	SHFILEINFO  ssfi, lsfi;
	
	//get a handle to the system small icon list   
	hSystemSmallImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)szPath,
													   0,    
													   &ssfi,
													   sizeof(SHFILEINFO),   
													   SHGFI_SYSICONINDEX | SHGFI_SMALLICON); 

	m_ilSmall.Attach(hSystemSmallImageList);

	hSystemLargeImageList = (HIMAGELIST)SHGetFileInfo((LPCTSTR)szPath,
														0,
														&lsfi,
														sizeof(SHFILEINFO),
														SHGFI_SYSICONINDEX | SHGFI_ICON);

	m_ilLarge.Attach(hSystemLargeImageList);

	GetListCtrl().SetImageList(&m_ilLarge, LVSIL_NORMAL);
	GetListCtrl().SetImageList(&m_ilSmall, LVSIL_SMALL);

	//
	// Add columns to the list view.
	//
	GetListCtrl().InsertColumn(0, _T("File Name"), LVCFMT_LEFT, 192);
	GetListCtrl().InsertColumn(1, _T("Size"), LVCFMT_RIGHT, 96);
	GetListCtrl().InsertColumn(2, _T("Last Modified"), LVCFMT_CENTER, 128);
	
	Refresh(szPath);
}

CSquaresDoc* CRightView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSquaresDoc)));
	return (CSquaresDoc*)m_pDocument;
}

///////////////////////////////////////////////////////////////////////////
// CFileView message handlers

int CRightView::GetIconIndex(const CString& csFileName)  //full path and file name
{
	SHFILEINFO    sfi;

	SHGetFileInfo(
		(LPCTSTR)csFileName,
		0,
		&sfi,
		sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX | SHGFI_SMALLICON);

	return sfi.iIcon;
}

int CRightView::Refresh(LPCTSTR pszPath)
{
	CString strPath = pszPath;
	if (strPath.Right(1) != _T("\\"))
		strPath += _T("\\");
	strPath += _T("*.*");

	HANDLE hFind;
	WIN32_FIND_DATA fd;
	int nCount = 0;

	if ((hFind = ::FindFirstFile(strPath, &fd)) != INVALID_HANDLE_VALUE) {
		//
		// Delete existing items (if any).
		//
		GetListCtrl().DeleteAllItems();

		//
		// Show the path name in the frame window's title bar.
		//
		TCHAR szFullPath[MAX_PATH];
		::GetFullPathName(pszPath, sizeof(szFullPath) / sizeof(TCHAR),
			szFullPath, NULL);
		m_strPath = szFullPath;

		CString strTitle = _T("WinDir - ");
		strTitle += szFullPath;
		AfxGetMainWnd()->SetWindowText(strTitle);

		//
		// Add items representing files to the list view.
		//
		if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			AddItem(nCount++, &fd);

		while (::FindNextFile(hFind, &fd)) {
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				if (!AddItem(nCount++, &fd))
					break;
		}
		::FindClose(hFind);
	}
	return nCount;
}

BOOL CRightView::AddItem(int nIndex, WIN32_FIND_DATA *pfd)
{
	//
	// Allocate a new ITEMINFO structure and initialize it with information
	// about the item.
	//
	ITEMINFO* pItem;
	try {
		pItem = new ITEMINFO;
	}
	catch (CMemoryException* e) {
		e->Delete();
		return FALSE;
	}
	
	pItem->strFileName = pfd->cFileName;
	pItem->nFileSizeLow = pfd->nFileSizeLow;
	pItem->ftLastWriteTime = pfd->ftLastWriteTime;

	//
	// Add the item to the list view.
	//
	LV_ITEM lvi;
	lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
	lvi.iItem = nIndex;
	lvi.iSubItem = 0;
	lvi.iImage = GetIconIndex(m_strPath + _T("\\") + pfd->cFileName);
	lvi.pszText = LPSTR_TEXTCALLBACK;
	lvi.lParam = (LPARAM)pItem;

	if (GetListCtrl().InsertItem(&lvi) == -1)
		return FALSE;

	return TRUE;
}

void CRightView::FreeItemMemory()
{
	m_ilSmall.Detach();
	m_ilLarge.Detach();

	int nCount = GetListCtrl().GetItemCount();
	if (nCount) {
		for (int i = 0; i < nCount; i++)
			delete (ITEMINFO*)GetListCtrl().GetItemData(i);
	}
}

void CRightView::OnDestroy()
{
	FreeItemMemory();
	CListView::OnDestroy();
}

void CRightView::OnGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
	CString string;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	if (pDispInfo->item.mask & LVIF_TEXT) {
		ITEMINFO* pItem = (ITEMINFO*)pDispInfo->item.lParam;

		switch (pDispInfo->item.iSubItem) {

		case 0: // File name.
			::lstrcpy(pDispInfo->item.pszText, pItem->strFileName);			
			break;

		case 1: // File size.
			string.Format(_T("%u"), pItem->nFileSizeLow);
			::lstrcpy(pDispInfo->item.pszText, string);
			break;

		case 2: // Date and time.
			CTime time(pItem->ftLastWriteTime);

			BOOL pm = FALSE;
			int nHour = time.GetHour();
			if (nHour == 0)
				nHour = 12;
			else if (nHour == 12)
				pm = TRUE;
			else if (nHour > 12) {
				nHour -= 12;
				pm = TRUE;
			}

			string.Format(_T("%d/%0.2d/%0.2d (%d:%0.2d%c)"),
				time.GetMonth(), time.GetDay(), time.GetYear() % 100,
				nHour, time.GetMinute(), pm ? _T('p') : _T('a'));
			::lstrcpy(pDispInfo->item.pszText, string);
			break;
		}
	}
	*pResult = 0;
}

void CRightView::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	GetListCtrl().SortItems(CompareFunc, pNMListView->iSubItem);
	*pResult = 0;
}

int CALLBACK CRightView::CompareFunc(LPARAM lParam1, LPARAM lParam2,
	LPARAM lParamSort)
{
	ITEMINFO* pItem1 = (ITEMINFO*)lParam1;
	ITEMINFO* pItem2 = (ITEMINFO*)lParam2;
	int nResult;

	switch (lParamSort) {

	case 0: // File name.
		nResult = pItem1->strFileName.CompareNoCase(pItem2->strFileName);
		break;

	case 1: // File size.
		nResult = pItem1->nFileSizeLow - pItem2->nFileSizeLow;
		break;

	case 2: // Date and time.
		nResult = ::CompareFileTime(&pItem1->ftLastWriteTime,
			&pItem2->ftLastWriteTime);
		break;
	}
	return nResult;
}

void CRightView::OnViewLargeIcons()
{
	ModifyStyle(LVS_TYPEMASK, LVS_ICON);
}

void CRightView::OnViewSmallIcons()
{
	ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
}

void CRightView::OnViewList()
{
	ModifyStyle(LVS_TYPEMASK, LVS_LIST);
}

void CRightView::OnViewDetails()
{
	ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
}

void CRightView::OnUpdateViewLargeIcons(CCmdUI* pCmdUI)
{
	DWORD dwCurrentStyle = GetStyle() & LVS_TYPEMASK;
	pCmdUI->SetRadio(dwCurrentStyle == LVS_ICON);
}

void CRightView::OnUpdateViewSmallIcons(CCmdUI* pCmdUI)
{
	DWORD dwCurrentStyle = GetStyle() & LVS_TYPEMASK;
	pCmdUI->SetRadio(dwCurrentStyle == LVS_SMALLICON);
}

void CRightView::OnUpdateViewList(CCmdUI* pCmdUI)
{
	DWORD dwCurrentStyle = GetStyle() & LVS_TYPEMASK;
	pCmdUI->SetRadio(dwCurrentStyle == LVS_LIST);
}

void CRightView::OnUpdateViewDetails(CCmdUI* pCmdUI)
{
	DWORD dwCurrentStyle = GetStyle() & LVS_TYPEMASK;
	pCmdUI->SetRadio(dwCurrentStyle == LVS_REPORT);
}

void CRightView::OnFileNewDirectory()
{
	CPathDialog dlg;
	dlg.m_strPath = m_strPath;
	if (dlg.DoModal() == IDOK)
		Refresh(dlg.m_strPath);
}

void CRightView::OnDoubleClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	DWORD dwPos = ::GetMessagePos();
	CPoint point((int)LOWORD(dwPos), (int)HIWORD(dwPos));
	GetListCtrl().ScreenToClient(&point);

	int nIndex;
	if ((nIndex = GetListCtrl().HitTest(point)) != -1) {
		CString string = GetListCtrl().GetItemText(nIndex, 0);
		TRACE(_T("%s was double-clicked\n"), string);
	}
	*pResult = 0;
}
