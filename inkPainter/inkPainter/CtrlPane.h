#pragma once
#include "resource.h"
#include "stdafx.h"
#include "inkPainterDoc.h"
#include "inkPainterView.h"
#define WM_DIALOGOK (WM_USER+1)

// CCtrlPane dialog
extern unsigned int m_nLineWidth;
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
	virtual void OnOK() {
		// TODO: Add extra validation here	
		UpdateData(TRUE);
		CString str;
		GetDlgItem(IDC_EDIT_CTRLSIZE)->GetWindowText(str);
		int i = _ttoi(str);
		m_nLineWidth =  (unsigned int)i;
		//MessageBox((LPTSTR)ch);
		//AfxMessageBox(ch);
		//m_nLineWidth = penSize;
		UpdateData(FALSE);
		//CDialog::OnOK(); // This will close the dialog and DoModal will return.
	}
	// Generated message map functions
	DECLARE_MESSAGE_MAP()

public:

};
