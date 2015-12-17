#pragma once


// CSearchToolBarCtrl

class CSearchToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CSearchToolBar)
public:
	CEdit m_EditSearch;
public:
	CSearchToolBar();
	virtual ~CSearchToolBar();	
protected:
	//afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
};


