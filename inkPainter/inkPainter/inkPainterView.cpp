
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
#include "SettingSizeDlg.h"
#include "SettingColorDlg.h"
#include "bmpScreen.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CinkPainterView

IMPLEMENT_DYNCREATE(CinkPainterView, CView)

BEGIN_MESSAGE_MAP(CinkPainterView, CView)
	//{{AFX_MSG(CinkPainterView)   /*�ú꽫��Ϣ��Ӧ�ͺ�����������*/
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
	ON_COMMAND(ID_SAVE, &CinkPainterView::OnSave)
END_MESSAGE_MAP()

// CinkPainterView construction/destruction

CinkPainterView::CinkPainterView()
	: m_nLineWidth(0)
	, m_StrExePath(_T(""))
	, m_StrDBPath(_T(""))
{
	// TODO: add construction code here
	m_clr = RGB(255, 0, 0);
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

//m_point���浱�û��������ʱ�����λ��  
CPoint m_point;
//m_click=true��ʾ����� false��ʾ����ͷ�  
bool m_click;
void CinkPainterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//�ѵ�ǰ����ĵ��λ�ø�����m_point  
	m_point = point;
	m_click = true;
	CView::OnLButtonDown(nFlags, point);
}

void CinkPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_click = false;
	CView::OnLButtonUp(nFlags, point);
}

//����ƶ�����ͼ��  
void CinkPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	//���廭�ʲ�ѡ��  
	CDC *p = GetDC();
	CPen pen(PS_SOLID, m_nLineWidth, m_clr);//���Ը�pen������
	p->SelectObject(pen);

	//��갴�½��л���  
	if (m_click == true) {
		p->MoveTo(m_point);
		p->LineTo(point);
		m_point = point;
	}
	ReleaseDC(p);
	CView::OnMouseMove(nFlags, point);
}

//���ʴ�С����
void CinkPainterView::OnSettingSize()
{
	//ģ̬�Ի��� ���OK��Ի�������
	CSettingSizeDlg sizeDlg;
	sizeDlg.m_nLineWidth = m_nLineWidth;
	if (IDOK == sizeDlg.DoModal())
	{
		m_nLineWidth = sizeDlg.m_nLineWidth;
	}
}

//������ɫ����
void CinkPainterView::OnSettingColor()
{
	CColorDialog dlg;
	dlg.m_cc.Flags |= CC_RGBINIT|CC_FULLOPEN;
	dlg.m_cc.rgbResult = m_clr;
	if(IDOK == dlg.DoModal())
	{
		m_clr = dlg.m_cc.rgbResult;
	}
}

//���ʱ�ˢ����
void CinkPainterView::OnSettingBrush()
{
	AfxMessageBox(L"set brush ");
}

//�������� ��͸���� ����
void CinkPainterView::OnSettingOpacity()
{
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


void CinkPainterView::OnSave()
{
	// TODO: �ڴ���������������
	bmpScreen Bmp;
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);//P:�õ��ͻ���ת��Ϊ�����Ļ������ϵ  

						  ////////////////////////ȷ��һ���ļ�����λ��  
	CString  sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//�õ������·��  
	sPath.ReleaseBuffer();
	int nPos = sPath.ReverseFind('\\');
	m_StrExePath = sPath.Left(nPos);
	sPath = m_StrExePath;
	m_StrDBPath = sPath + _T("\\SavePicture\\OurDrawPic.bmp");  //�õ�����Ŀ¼�µ����ݿ������·��  
															   //Bmp.screenShot(rect,0,0,"ScreenPic.bmp");  
	USES_CONVERSION;
	char* p = T2A(m_StrDBPath);



	////////////////��Ҫʵ�ֺ���  
	Bmp.screenShot(rect, 0, 0, p); ///�ڶ�����������û��
	CString showPath = _T("Save Success!\n\nPath: ") + m_StrDBPath;
	AfxMessageBox(showPath);

}
