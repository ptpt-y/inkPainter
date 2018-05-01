// CtrlPane.cpp : implementation file
//

#include "stdafx.h"
#include "inkPainter.h"
#include "CtrlPane.h"
#include "afxdialogex.h"


// CCtrlPane dialog

IMPLEMENT_DYNAMIC(CCtrlPane, CDialogEx)

CCtrlPane::CCtrlPane(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CTRL, pParent)
{

}


CCtrlPane::~CCtrlPane()
{
}

BOOL CCtrlPane::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return true;
}

void CCtrlPane::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

}


BEGIN_MESSAGE_MAP(CCtrlPane, CDialogEx)
END_MESSAGE_MAP()


// CCtrlPane message handlers
