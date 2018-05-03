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
extern int m_spread;
extern BOOL m_eraser;

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
	afx_msg void OnClickedRadio2Spread0();
	afx_msg void OnRadio3Spread1();
	afx_msg void OnRadio4Spread2();
	afx_msg void OnRadio5Spread3();
	afx_msg void OnRadio1Eraser();
	DECLARE_MESSAGE_MAP()

public:

	UINT penSize;
	CMFCColorButton ctrlClrBtn;
	BYTE pic_alpha;
	int ctrlSpread;
	BOOL eraser;

};
