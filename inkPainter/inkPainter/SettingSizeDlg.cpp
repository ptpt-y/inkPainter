// SettingSizeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "inkPainter.h"
#include "SettingSizeDlg.h"
#include "afxdialogex.h"


// CSettingSizeDlg 对话框

IMPLEMENT_DYNAMIC(CSettingSizeDlg, CDialogEx)

CSettingSizeDlg::CSettingSizeDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SETTING_SIZE, pParent)
	, m_nLineWidth(10)
{

}

CSettingSizeDlg::~CSettingSizeDlg()
{
}

void CSettingSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_LINE_WIDTH, m_nLineWidth);
}




BEGIN_MESSAGE_MAP(CSettingSizeDlg, CDialogEx)
END_MESSAGE_MAP()


// CSettingSizeDlg 消息处理程序
