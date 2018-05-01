#include "stdafx.h"
#include "MyDockablePane.h"
#include "SettingSizeDlg.h"




CMyDockablePane::CMyDockablePane()
{

}


CMyDockablePane::~CMyDockablePane()
{
}


BEGIN_MESSAGE_MAP(CMyDockablePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


int CMyDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// 创建组合:
	//CMFCButton sizeDlgBtn;
	//sizeDlgBtn.Create(L"OK", WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(150, 70, 190, 90), this, 1);
	const DWORD dwViewStyle = WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_BORDER | CBS_SORT | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

	if (!m_CtrlPane.Create(IDD_CTRL, this))
	{
		TRACE0("未能创建对话框窗口\n");
		return -1;      // 未能创建
	}
	m_CtrlPane.ShowWindow(SW_SHOW);
	return 0;
}


void CMyDockablePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码  

	// 选项卡控件应覆盖整个工作区  
	/*---------------------------------------------------------------------------------------*/
	m_CtrlPane.SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	m_CtrlPane.ShowWindow(SW_SHOW);
	/*---------------------------------------------------------------------------------------*/
}
