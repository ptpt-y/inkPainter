#pragma once
#include "resource.h"

// CSettingSizeDlg �Ի���

class CSettingSizeDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingSizeDlg)

public:
	CSettingSizeDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingSizeDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_SIZE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UINT m_nLineWidth;
};
