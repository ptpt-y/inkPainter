#pragma once


// CSettingColorDlg �Ի���

class CSettingColorDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingColorDlg)

public:
	CSettingColorDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingColorDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_COLOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	COLORREF m_clr;
};
