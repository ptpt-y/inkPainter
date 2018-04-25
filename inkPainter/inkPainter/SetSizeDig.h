#pragma once


// CSetSizeDig dialog

class CSetSizeDig : public CDialogEx
{
	DECLARE_DYNAMIC(CSetSizeDig)

public:
	CSetSizeDig(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetSizeDig();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
