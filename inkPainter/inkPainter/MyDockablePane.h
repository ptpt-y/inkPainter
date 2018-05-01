#pragma once
#include "afxdockablepane.h"
#include "CtrlPane.h"
class CMyDockablePane :
	public CDockablePane
{
public:
	CMyDockablePane();
	~CMyDockablePane();
	CCtrlPane m_CtrlPane;


	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

