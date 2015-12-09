
// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "MFCSearchingFiles.h"

#include "MFCSearchingFilesDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Image indexes
#define ILI_HARD_DISK       0
#define ILI_FLOPPY          1
#define ILI_CD_ROM          2
#define ILI_NET_DRIVE       3
#define ILI_CLOSED_FOLDER   4
#define ILI_OPEN_FOLDER     5

// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)	
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	
	if (!CTreeView::PreCreateWindow(cs))
		return FALSE;

	cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS;
	return TRUE;
}

void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetTreeCtrl().
	//
	// Initialize the image list.
	//
	m_ilDrives.Create(IDB_DRIVEIMAGES, 16, 1, RGB(255, 0, 255));
	
	GetTreeCtrl().SetImageList(&m_ilDrives, TVSIL_NORMAL);	
	//
	// Populate the tree view with drive items.
	//
	AddDrives();

	//
	// Show the folders on the current drive.
	//
	TCHAR szPath[MAX_PATH];
	::GetCurrentDirectory(sizeof(szPath) / sizeof(TCHAR), szPath);
	CString strPath = szPath;
	strPath = strPath.Left(3);

	HTREEITEM hItem = GetTreeCtrl().GetNextItem(NULL, TVGN_ROOT);
	while (hItem != NULL) {
		if (GetTreeCtrl().GetItemText(hItem) == strPath)
			break;
		hItem = GetTreeCtrl().GetNextSiblingItem(hItem);
	}

	if (hItem != NULL) {
		GetTreeCtrl().Expand(hItem, TVE_EXPAND);
		GetTreeCtrl().Select(hItem, TVGN_CARET);
	}

}

// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CSquaresDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSquaresDoc)));
	return (CSquaresDoc*)m_pDocument;
}
#endif //_DEBUG


// CLeftView message handlers

int CLeftView::AddDrives()
{
	int nPos = 0;
	int nDrivesAdded = 0;
	CString string = _T("?:\\");

	DWORD dwDriveList = ::GetLogicalDrives();

	while (dwDriveList) {
		if (dwDriveList & 1) {
			string.SetAt(0, _T('A') + nPos);
			if (AddDriveItem(string))
				nDrivesAdded++;
		}
		dwDriveList >>= 1;
		nPos++;
	}
	return nDrivesAdded;
}

BOOL CLeftView::AddDriveItem(LPCTSTR pszDrive)
{
	CString string;
	HTREEITEM hItem;

	UINT nType = ::GetDriveType(pszDrive);

	switch (nType) {

	case DRIVE_REMOVABLE:
		hItem = GetTreeCtrl().InsertItem(pszDrive, ILI_FLOPPY,
			ILI_FLOPPY);
		GetTreeCtrl().InsertItem(_T(""), ILI_CLOSED_FOLDER,
			ILI_CLOSED_FOLDER, hItem);
		break;

	case DRIVE_FIXED:
	case DRIVE_RAMDISK:
		hItem = GetTreeCtrl().InsertItem(pszDrive, ILI_HARD_DISK,
			ILI_HARD_DISK);
		SetButtonState(hItem, pszDrive);
		break;

	case DRIVE_REMOTE:
		hItem = GetTreeCtrl().InsertItem(pszDrive, ILI_NET_DRIVE,
			ILI_NET_DRIVE);
		SetButtonState(hItem, pszDrive);
		break;

	case DRIVE_CDROM:
		hItem = GetTreeCtrl().InsertItem(pszDrive, ILI_CD_ROM,
			ILI_CD_ROM);
		GetTreeCtrl().InsertItem(_T(""), ILI_CLOSED_FOLDER,
			ILI_CLOSED_FOLDER, hItem);
		break;

	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CLeftView::SetButtonState(HTREEITEM hItem, LPCTSTR pszPath)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	BOOL bResult = FALSE;

	CString strPath = pszPath;
	if (strPath.Right(1) != _T("\\"))
		strPath += _T("\\");
	strPath += _T("*.*");

	if ((hFind = ::FindFirstFile(strPath, &fd)) == INVALID_HANDLE_VALUE)
		return bResult;

	do {
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			CString strComp = (LPCTSTR)&fd.cFileName;
			if ((strComp != _T(".")) && (strComp != _T(".."))) {
				GetTreeCtrl().InsertItem(_T(""), ILI_CLOSED_FOLDER,
					ILI_CLOSED_FOLDER, hItem);
				bResult = TRUE;
				break;
			}
		}
	} while (::FindNextFile(hFind, &fd));

	::FindClose(hFind);
	return bResult;
}

void CLeftView::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	CString string = GetPathFromItem(hItem);

	*pResult = FALSE;

	if (pNMTreeView->action == TVE_EXPAND) {
		DeleteFirstChild(hItem);
		if (AddDirectories(hItem, string) == 0)
			*pResult = TRUE;
	}
	else { // pNMTreeView->action == TVE_COLLAPSE
		DeleteAllChildren(hItem);
		if (GetTreeCtrl().GetParentItem(hItem) == NULL)
			GetTreeCtrl().InsertItem(_T(""), ILI_CLOSED_FOLDER,
				ILI_CLOSED_FOLDER, hItem);
		else
			SetButtonState(hItem, string);
	}
}

CString CLeftView::GetPathFromItem(HTREEITEM hItem)
{
	CString strResult = GetTreeCtrl().GetItemText(hItem);

	HTREEITEM hParent;
	while ((hParent = GetTreeCtrl().GetParentItem(hItem)) != NULL) {
		CString string = GetTreeCtrl().GetItemText(hParent);
		if (string.Right(1) != _T("\\"))
			string += _T("\\");
		strResult = string + strResult;
		hItem = hParent;
	}
	return strResult;
}

void CLeftView::DeleteFirstChild(HTREEITEM hItem)
{
	HTREEITEM hChildItem;
	if ((hChildItem = GetTreeCtrl().GetChildItem(hItem)) != NULL)
		GetTreeCtrl().DeleteItem(hChildItem);
}

void CLeftView::DeleteAllChildren(HTREEITEM hItem)
{
	HTREEITEM hChildItem;
	if ((hChildItem = GetTreeCtrl().GetChildItem(hItem)) == NULL)
		return;

	do {
		HTREEITEM hNextItem =
			GetTreeCtrl().GetNextSiblingItem(hChildItem);
		GetTreeCtrl().DeleteItem(hChildItem);
		hChildItem = hNextItem;
	} while (hChildItem != NULL);
}

int CLeftView::AddDirectories(HTREEITEM hItem, LPCTSTR pszPath)
{
	HANDLE hFind;
	WIN32_FIND_DATA fd;
	HTREEITEM hNewItem;

	int nCount = 0;

	CString strPath = pszPath;
	if (strPath.Right(1) != _T("\\"))
		strPath += _T("\\");
	strPath += _T("*.*");

	if ((hFind = ::FindFirstFile(strPath, &fd)) == INVALID_HANDLE_VALUE) {
		if (GetTreeCtrl().GetParentItem(hItem) == NULL)
			GetTreeCtrl().InsertItem(_T(""), ILI_CLOSED_FOLDER,
				ILI_CLOSED_FOLDER, hItem);
		return 0;
	}

	do {
		if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
			CString strComp = (LPCTSTR)&fd.cFileName;
			if ((strComp != _T(".")) && (strComp != _T(".."))) {
				hNewItem =
					GetTreeCtrl().InsertItem((LPCTSTR)&fd.cFileName,
						ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, hItem);

				CString strNewPath = pszPath;
				if (strNewPath.Right(1) != _T("\\"))
					strNewPath += _T("\\");

				strNewPath += (LPCTSTR)&fd.cFileName;
				SetButtonState(hNewItem, strNewPath);
				nCount++;
			}
		}
	} while (::FindNextFile(hFind, &fd));

	::FindClose(hFind);
	return nCount;
}
