#pragma once
#include "afxdockablepane.h"
#include "CtrlPane.h"

class CCtrlPaneToolBar : public CMFCToolBar
{
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*)GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CMyDockablePane :
	public CDockablePane
{
public:
	CMyDockablePane();
	~CMyDockablePane();
	void AdjustLayout();

	CCtrlPane m_CtrlPane;
	CCtrlPaneToolBar m_wndToolBar;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

};

