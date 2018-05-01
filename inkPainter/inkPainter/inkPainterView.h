
// inkPainterView.h : interface of the CinkPainterView class
//

#pragma once
//extern unsigned int m_nLineWidth;

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
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

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

public:
	afx_msg void OnSave();
	CString m_StrExePath;
	CString m_StrDBPath;
};

#ifndef _DEBUG  // debug version in inkPainterView.cpp
inline CinkPainterDoc* CinkPainterView::GetDocument() const
   { return reinterpret_cast<CinkPainterDoc*>(m_pDocument); }
#endif

