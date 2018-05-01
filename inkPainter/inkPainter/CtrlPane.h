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

};
