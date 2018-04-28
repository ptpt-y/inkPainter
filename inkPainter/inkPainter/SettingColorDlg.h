#pragma once


// CSettingColorDlg 对话框

class CSettingColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingColorDlg)

public:
	CSettingColorDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingColorDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_COLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	COLORREF m_clr;
};
