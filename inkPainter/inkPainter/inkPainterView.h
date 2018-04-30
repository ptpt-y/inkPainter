
// inkPainterView.h : interface of the CinkPainterView class
//

#pragma once
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glaux.h>
#include "afxwin.h"
#pragma comment( lib, "opengl32.lib")
#pragma comment( lib, "glu32.lib")
#pragma comment( lib, "glaux.lib")

#define N 50000

struct ColorPoint//��Ľṹ��
{
	float x, y;//����
	float size;
	GLfloat color[3];
	int life;
};

class CinkPainterView : public CView
{
protected: // create from serialization only
	CinkPainterView();
	DECLARE_DYNCREATE(CinkPainterView)

// Attributes
public:
	CinkPainterDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	//virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	//virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CinkPainterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CinkPainterView)    /*�ú꽫��Ϣ��Ӧ�ͺ�����������*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//��������
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);                       //��ʱ��

	//{{AFX_MSG 
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//menu setting
	afx_msg void OnSettingSize();
	afx_msg void OnSettingColor();
	afx_msg void OnSettingBrush();
	afx_msg void OnSettingOpacity();
	DECLARE_MESSAGE_MAP()
private:
	unsigned int m_nLineWidth;
	COLORREF m_clr;
	BYTE r;
	BYTE g;
	BYTE b;
public:
	afx_msg void OnSave();
	CString m_StrExePath;
	CString m_StrDBPath;

	// ��������
	void LoadTextures();
	// ��ͼ
	void DrawWithOpenGL();

	// ��ʼ��OpenGL,�������ƻ�����RC����ʹ֮��Ϊ��ǰ���ƻ���
	BOOL CreateViewGLContext(HDC hDC);
	// �������ظ�ʽ
	BOOL SetWindowPixelFormat(HDC hDC);

	// �ͻ������ھ��
	HWND m_hwnd;
	// �豸������
	HDC m_hDC;
	// ���ƻ���
	HGLRC m_hGLContext;

	//��������
	GLuint m_texture[10];
	// ���ֻ�Ϸ���
	BOOL m_bBrush;

	// ��ʼ���ĵ�
	int m_iDrawStartPoint;
	// �����Ŀ
	int m_iPointNum;
	// �������
	ColorPoint m_ColorPoint[N];

	//���ڴ�С
	int m_iWindowWidth, m_iWindowHeight;

	bool	m_LeftButtonDown;                   //�ж�����Ƿ�����
	CPoint	m_LeftDownPos;                      //�������λ��
	CPoint	m_MousePos;                         //���λ��

	int m_ImageWidth;                           //�����
	int m_ImageHeight;                          //�����
	int m_GLPixelIndex;                         //���ظ�ʽ����

	float m_fPointSize;// ��Ĵ�С

	int mode = 1;                               //�ı���Ⱦģʽʱʹ�õ��Ĳ���
	float m_Color[3];                           //��ˢ��ɫ
	int thinkness = 0;                          //�ı��ϸʹ�õ��Ĳ���
	
	int m_iSimStartPoint;//����0 ģ����ʼ��

	bool spread;//��ʾ�Ƿ�����ɢ ������Ϊ1 ����Ϊ0

																					
	void Spread();	// ��ɢЧ��	

private:
	// ����DC
	CDC m_dcCompatible;
public:
	// ����λͼ
	CBitmap bitmap;
	CBitmap getBitmap();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in inkPainterView.cpp
inline CinkPainterDoc* CinkPainterView::GetDocument() const
   { return reinterpret_cast<CinkPainterDoc*>(m_pDocument); }
#endif

