
// inkPainterView.cpp : implementation of the CinkPainterView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "inkPainter.h"
#endif

#include "inkPainterDoc.h"
#include "inkPainterView.h"
#include "SetSizeDig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CinkPainterView

IMPLEMENT_DYNCREATE(CinkPainterView, CView)

BEGIN_MESSAGE_MAP(CinkPainterView, CView)
	//{{AFX_MSG(CinkPainterView)   /*用宏将消息响应和函数关联起来*/
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//{{AFX_MSG 
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CinkPainterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_SETTINGS_SIZE, &CinkPainterView::OnSettingSize)
	ON_COMMAND(ID_SETTINGS_COLOR, &CinkPainterView::OnSettingColor)
	ON_COMMAND(ID_SETTINGS_BRUSH, &CinkPainterView::OnSettingBrush)
	ON_COMMAND(ID_SETTINGS_OPACITY, &CinkPainterView::OnSettingOpacity)
END_MESSAGE_MAP()

// CinkPainterView construction/destruction

CinkPainterView::CinkPainterView()
{
	// TODO: add construction code here

}

CinkPainterView::~CinkPainterView()
{
}

BOOL CinkPainterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CinkPainterView drawing

void CinkPainterView::OnDraw(CDC* /*pDC*/)
{
	CinkPainterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CinkPainterView printing


void CinkPainterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CinkPainterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CinkPainterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CinkPainterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CinkPainterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CinkPainterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

//m_point保存当用户点击界面时点击的位置  
CPoint m_point;
//m_click=true表示鼠标点击 false表示鼠标释放  
bool m_click;
void CinkPainterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//把当前点击的点的位置赋给点m_point  
	m_point = point;
	m_click = true;
	CView::OnLButtonDown(nFlags, point);
}

void CinkPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_click = false;
	CView::OnLButtonUp(nFlags, point);
}

//鼠标移动绘制图形  
void CinkPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	//定义画笔并选择  
	CDC *p = GetDC();
	CPen pen(PS_SOLID, 4, RGB(255, 0, 0));//可以改pen的设置
	p->SelectObject(pen);

	//鼠标按下进行绘制  
	if (m_click == true) {
		p->MoveTo(m_point);
		p->LineTo(point);
		m_point = point;
	}
	CView::OnMouseMove(nFlags, point);
}
//画笔大小设置
void CinkPainterView::OnSettingSize()
{
	//模态对话框 点击OK后对话框被销毁
	CSetSizeDig sizeDig;
	sizeDig.DoModal();
}
//画笔颜色设置
void CinkPainterView::OnSettingColor()
{
	/*非模态对话框（指针or设为成员）点击OK后对话框仅被覆盖(响应基类的OnOK)
	CSetSizeDig* dlg = new CSetSizeDig();
	dlg->Create(IDD_DIALOG1,this);
	dlg->ShowWindow(SW_SHOW);*/
	AfxMessageBox(L"set color ");
}

//画笔笔刷设置
void CinkPainterView::OnSettingBrush()
{
	//CDialog* dlg = new CDialog;
	//dlg->Create("set size");
	AfxMessageBox(L"set brush ");
}

//画笔轻重 不透明度 设置
void CinkPainterView::OnSettingOpacity()
{
	//CDialog* dlg = new CDialog;
	//dlg->Create("set size");
	AfxMessageBox(L"set opacity ");
}
// CinkPainterView diagnostics

#ifdef _DEBUG
void CinkPainterView::AssertValid() const
{
	CView::AssertValid();
}

void CinkPainterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CinkPainterDoc* CinkPainterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CinkPainterDoc)));
	return (CinkPainterDoc*)m_pDocument;
}
#endif //_DEBUG


// CinkPainterView message handlers
