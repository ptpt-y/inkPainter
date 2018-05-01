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
	ON_WM_DESTROY()
END_MESSAGE_MAP()


int CMyDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	CRect temprect;
	GetClientRect(&temprect);

	if (!m_CtrlPane.Create(IDD_CTRL, this))
	{
		TRACE0("δ�ܴ����Ի��򴰿�\n");
		return -1;      // δ�ܴ���
	}
	m_CtrlPane.SetWindowPos(this, 0, 0, temprect.Width(), temprect.Height(), SWP_NOZORDER | SWP_NOMOVE);
	m_CtrlPane.ShowWindow(SW_SHOW);
	return 0;
}


void CMyDockablePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// ѡ��ؼ�Ӧ��������������  
	/*---------------------------------------------------------------------------------------*/
	if (GetSafeHwnd() == NULL)
	{
		return;
	}

	if (m_CtrlPane.GetSafeHwnd() != NULL)
	{
		CRect rectClient;
		GetClientRect(rectClient);
		m_CtrlPane.SetWindowPos(NULL, rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), SWP_NOACTIVATE | SWP_NOZORDER);
	}
	/*---------------------------------------------------------------------------------------*/
}

void CMyDockablePane::OnDestroy()
{
	CDockablePane::OnDestroy();
	m_CtrlPane.DestroyWindow();
}
