
// MFCSearchingFiles.h : main header file for the MFCSearchingFiles application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCSearchingFilesApp:
// See MFCSearchingFiles.cpp for the implementation of this class
//

class CMFCSearchingFilesApp : public CWinApp
{
public:
	CMFCSearchingFilesApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCSearchingFilesApp theApp;
