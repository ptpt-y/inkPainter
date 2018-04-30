
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

struct ColorPoint//点的结构体
{
	float x, y;//坐标
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
	//{{AFX_MSG(CinkPainterView)    /*用宏将消息响应和函数关联起来*/
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);//创建窗口
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);                       //定时器

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

	// 载入纹理
	void LoadTextures();
	// 绘图
	void DrawWithOpenGL();

	// 初始化OpenGL,创建绘制环境（RC）并使之成为当前绘制环境
	BOOL CreateViewGLContext(HDC hDC);
	// 设置像素格式
	BOOL SetWindowPixelFormat(HDC hDC);

	// 客户区窗口句柄
	HWND m_hwnd;
	// 设备上下文
	HDC m_hDC;
	// 绘制环境
	HGLRC m_hGLContext;

	//纹理数组
	GLuint m_texture[10];
	// 两种混合方案
	BOOL m_bBrush;

	// 开始画的点
	int m_iDrawStartPoint;
	// 点的数目
	int m_iPointNum;
	// 点的数组
	ColorPoint m_ColorPoint[N];

	//窗口大小
	int m_iWindowWidth, m_iWindowHeight;

	bool	m_LeftButtonDown;                   //判断左键是否落下
	CPoint	m_LeftDownPos;                      //左键落下位置
	CPoint	m_MousePos;                         //鼠标位置

	int m_ImageWidth;                           //纹理宽
	int m_ImageHeight;                          //纹理高
	int m_GLPixelIndex;                         //像素格式索引

	float m_fPointSize;// 点的大小

	int mode = 1;                               //改变渲染模式时使用到的参数
	float m_Color[3];                           //笔刷颜色
	int thinkness = 0;                          //改变粗细使用到的参数
	
	int m_iSimStartPoint;//常数0 模拟起始点

	bool spread;//表示是否开启扩散 若是则为1 否则为0

																					
	void Spread();	// 扩散效果	

private:
	// 兼容DC
	CDC m_dcCompatible;
public:
	// 兼容位图
	CBitmap bitmap;
	CBitmap getBitmap();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in inkPainterView.cpp
inline CinkPainterDoc* CinkPainterView::GetDocument() const
   { return reinterpret_cast<CinkPainterDoc*>(m_pDocument); }
#endif

