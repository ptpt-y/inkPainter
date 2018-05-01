// SettingColorDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "inkPainter.h"
#include "SettingColorDlg.h"
#include "afxdialogex.h"


// CSettingColorDlg 对话框

IMPLEMENT_DYNAMIC(CSettingColorDlg, CDialogEx)

CSettingColorDlg::CSettingColorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING_COLOR, pParent)
{
	m_clr = RGB(255, 0, 0);
}

CSettingColorDlg::~CSettingColorDlg()
{
}

void CSettingColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingColorDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingColorDlg 消息处理程序
