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
	, pic_alpha(120)
	//, spread_switch(1)
{
	//控制面板中各个控件的初始化
	penSize = m_nLineWidth;
	ctrlClrBtn.SetColor(m_clr);
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

	DDX_Text(pDX, IDC_EDIT_CTRLSIZE, penSize);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON2, ctrlClrBtn);
	DDX_Text(pDX, IDC_EDIT2, pic_alpha);
	DDV_MinMaxByte(pDX, pic_alpha, 0, 255);
}

void CCtrlPane::OnOK() {
	// TODO: Add extra validation here	
	UpdateData(TRUE);

	m_nLineWidth = penSize;//画笔大小重置
	m_alpha = pic_alpha;
	m_clr = ctrlClrBtn.GetColor();//画笔颜色重置
								  //m_spread = spread_switch;
	if (m_clr == -1) {
		m_clr = ctrlClrBtn.GetAutomaticColor();
	}

	UpdateData(FALSE);
	//CDialog::OnOK(); // This will close the dialog and DoModal will return.
}

BEGIN_MESSAGE_MAP(CCtrlPane, CDialogEx)
END_MESSAGE_MAP()


// CCtrlPane message handlers
