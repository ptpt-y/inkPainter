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
	, ctrlSpread(0)
{
	//��������и����ؼ��ĳ�ʼ��
	pic_alpha = 120;
	ctrlSpread = 1;
	penSize = m_nLineWidth;
	ctrlClrBtn.SetColor(m_clr);
	eraser = false;
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
	DDX_Radio(pDX, IDC_RADIO2_SPREAD0, ctrlSpread);
}

void CCtrlPane::OnOK() {
	// TODO: Add extra validation here	
	UpdateData(TRUE);

	m_nLineWidth = penSize;//���ʴ�С����
	m_alpha = pic_alpha;//���ʲ�͸��������
	m_clr = ctrlClrBtn.GetColor();//������ɫ����
	m_spread = ctrlSpread;	//������ɢЧ������
	m_eraser = eraser;
	if (m_clr == -1) {
		m_clr = ctrlClrBtn.GetAutomaticColor();
	}

	UpdateData(FALSE);
	//CDialog::OnOK(); // This will close the dialog and DoModal will return.
}

BEGIN_MESSAGE_MAP(CCtrlPane, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO2_SPREAD0, &CCtrlPane::OnClickedRadio2Spread0)
	ON_COMMAND(IDC_RADIO3_SPREAD1, &CCtrlPane::OnRadio3Spread1)
	ON_COMMAND(IDC_RADIO4_SPREAD2, &CCtrlPane::OnRadio4Spread2)
	ON_COMMAND(IDC_RADIO5_SPREAD3, &CCtrlPane::OnRadio5Spread3)
	ON_COMMAND(IDC_RADIO1_ERASER, &CCtrlPane::OnRadio1Eraser)
END_MESSAGE_MAP()


// CCtrlPane message handlers

//��ͬ��Ⱦ��ɢģʽ���趨
void CCtrlPane::OnClickedRadio2Spread0()
{
	ctrlSpread = 0;
	eraser = false;
}


void CCtrlPane::OnRadio3Spread1()
{
	ctrlSpread = 1;
	eraser = false;
}


void CCtrlPane::OnRadio4Spread2()
{
	ctrlSpread = 2;
	eraser = false;
}


void CCtrlPane::OnRadio5Spread3()
{
	ctrlSpread = 3;
	eraser = false;
}

void CCtrlPane::OnRadio1Eraser()
{
	ctrlSpread = 4;
	eraser = true;
	ctrlClrBtn.SetColor(RGB(0,0, 0));
}
