#pragma once
#include "resource.h"
#include "stdafx.h"
#include "inkPainterDoc.h"
#include "inkPainterView.h"
#include "afxcolorbutton.h"
#define WM_DIALOGOK (WM_USER+1)

// CCtrlPane dialog
extern unsigned int m_nLineWidth;
extern COLORREF m_clr;
extern BYTE m_alpha;// 点的不透明度
//extern int m_spread;//表示是否开启扩散 若是则为1 否则为0

class CCtrlPane : public CDialogEx
{
	DECLARE_DYNAMIC(CCtrlPane)

public:

	CCtrlPane(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCtrlPane();
	BOOL OnInitDialog();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTRL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK();
	// Generated message map functions
	DECLARE_MESSAGE_MAP()

public:

	UINT penSize;
	CMFCColorButton ctrlClrBtn;

	BYTE pic_alpha;
	//int spread_switch;
};
