#pragma once


// CPathDialog dialog

class CPathDialog : public CDialog
{
	DECLARE_DYNAMIC(CPathDialog)

public:
	CPathDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPathDialog();
	//enum { IDD = IDD_NEWDIR };
	CString	m_strPath;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATHDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
