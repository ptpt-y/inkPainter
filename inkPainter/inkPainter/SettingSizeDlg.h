#pragma once
#include "resource.h"

// CSettingSizeDlg 对话框

class CSettingSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingSizeDlg)

public:
	CSettingSizeDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingSizeDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_SIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nLineWidth;
};
