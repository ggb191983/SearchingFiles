// PathDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSearchingFiles.h"
#include "PathDialog.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CPathDialog dialog

IMPLEMENT_DYNAMIC(CPathDialog, CDialog)

CPathDialog::CPathDialog(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_PATHDIALOG, pParent)
{

}

CPathDialog::~CPathDialog()
{
}

void CPathDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPathDialog, CDialog)
END_MESSAGE_MAP()


// CPathDialog message handlers
