
// inkPainterView.cpp : implementation of the CinkPainterView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "inkPainter.h"
#endif

#include "inkPainterDoc.h"
#include "CtrlPane.h"
#include "inkPainterView.h"
#include "SettingSizeDlg.h"
#include "SettingColorDlg.h"
#include "bmpScreen.h"
#include "windows.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

unsigned m_nLineWidth = 10;
COLORREF m_clr = RGB(0, 0, 0);

CView* g_pView;
bool resetView = false;

// CinkPainterView

IMPLEMENT_DYNCREATE(CinkPainterView, CView)

//��Ϣӳ�亯��
BEGIN_MESSAGE_MAP(CinkPainterView, CView)
	//{{AFX_MSG(CinkPainterView)   /*�ú꽫��Ϣ��Ӧ�ͺ�����������*/
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CREATE()//����ͻ������ڵ���Ϣ
	ON_WM_DESTROY()
	ON_WM_TIMER()

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

	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

// CinkPainterView construction/destruction
BYTE tempdata[1024 * 1024 * 3];//���� ��������ʱ��
CinkPainterView::CinkPainterView()
	: m_bBrush(TRUE)
	, m_StrExePath(_T(""))
	, m_StrDBPath(_T(""))
	, m_iDrawStartPoint(0)
	, m_iPointNum(0)
	, m_fPointSize(1.0)
	, m_iSimStartPoint(0)
	, spread(TRUE)
{
	m_LeftButtonDown = false;
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
//view��ͼ�����ʱ�������ػ�
void CinkPainterView::OnDraw(CDC* pDC)
{
	CinkPainterDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
	{
		//AfxMessageBox(L"!pDoc");
		return;
	}
	//===================================
	//if (resetView)
	//{
	//	m_iSimStartPoint = m_iDrawStartPoint = m_iPointNum = 0;//��ʼ��
	//	resetView = false;
	//}
	//===================================


	if (pDoc->m_bImageChanged)
	{
		//��������!!!!
		//Ҫ�������Ŀǰ���ӿ����ݴ����������¼��ص��ӿ�
		//AfxMessageBox(L"!!! ");

			if (!m_dcCompatible.m_hDC)//�ж��Ƿ��Ѿ���������DC
	{

		m_dcCompatible.CreateCompatibleDC(pDC);
		CRect rect;
		GetClientRect(&rect);//��ȡ��ǰ�ͻ�����Ĵ�С
							 //CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
		m_dcCompatible.SelectObject(&bitmap);//������dcѡ��һ������λͼ
		m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	}

		//CRect rect;
		//GetClientRect(&rect);//��ȡ��ǰ�ͻ�����Ĵ�С
		////CBitmap bitmap;
		////bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
		////m_dcCompatible.SelectObject(&bitmap);//������dcѡ��һ������λͼ
		////m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
		//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcCompatible, 0, 0, SRCCOPY);
		////AfxMessageBox(L"???");



		////glBindTexture(GL_TEXTURE_2D, m_texture[1]);//�����ɵ��������ư󶨵�ָ����������
		////m_ImageWidth = pDoc->m_ImageWidth;
		////m_ImageHeight = pDoc->m_ImageHeight;

		////��pDoc->m_pImageָ��ָ���ͼƬ�Ĳ�����Ϊ2D����
		////�ڳ�����ֻҪ���ϸı�ָ��ָ���ͼƬ�����Զ���������
		////glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_ImageWidth, m_ImageHeight, GL_RGB, GL_UNSIGNED_BYTE, pDoc->m_pImage);
		//pDoc->m_bImageChanged = FALSE;

		//===========================���ܿ��е�ʵ����̫�˷��ڴ����ܲ���============
		//OnSave();
		//CRect    rect;     //�ͻ�����
		//CDC*    pDC = GetDC();//��ȡ��ͼ���豸�ı�ָ��
		//CDC    dcComp; //�����ڴ��豸�ı�����
		//			   //�õ��ͻ����ߴ�
		//GetClientRect(&rect);
		////��װm_MemBitmap����
		//bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
		//bitmap.m_hObject = (HBITMAP)::LoadImage(NULL, _T("Material/texture.bmp"), IMAGE_BITMAP, 500, 400, LR_LOADFROMFILE);
		////bitmap.LoadImage("Material/texture.bmp");
		////�����ڴ��豸�ı�����
		//dcComp.CreateCompatibleDC(pDC);
		////�ڴ��豸������λͼ����ѡ��
		//dcComp.SelectObject(&bitmap);
		////��λ���亯����ʾ����
		//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcComp, 0, 0, SRCCOPY);
		////�����ڴ��豸�ı�����
		//dcComp.DeleteDC();
		////�ͷ��豸�ı�ָ��
		//ReleaseDC(pDC);
		//DrawWithOpenGL();

	}
	DrawWithOpenGL();
	SwapBuffers(m_hDC);
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

//void CinkPainterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: add extra initialization before printing
//}
//
//void CinkPainterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
//{
//	// TODO: add cleanup after printing
//}

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


void CinkPainterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	//�ѵ�ǰ����ĵ��λ�ø�����m_point  
	m_LeftButtonDown = TRUE;
	m_fPointSize = 1;
	CView::OnLButtonDown(nFlags, point);
}

void CinkPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClientDC dc(this);

	CinkPainterDoc* pDoc = GetDocument();
	m_LeftButtonDown = FALSE;
	pDoc->m_bImageChanged = TRUE;

	//if (!m_dcCompatible.m_hDC)//�ж��Ƿ��Ѿ���������DC
	//{
	//	//AfxMessageBox(L"set brush ");

	//	m_dcCompatible.CreateCompatibleDC(&dc);
	//	CRect rect;
	//	GetClientRect(&rect);//��ȡ��ǰ�ͻ�����Ĵ�С
	//						 //CBitmap bitmap;
	//	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Width());
	//	m_dcCompatible.SelectObject(&bitmap);//������dcѡ��һ������λͼ
	//	m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
	//}
		//CRect rect;
		//GetClientRect(&rect);//��ȡ��ǰ�ͻ�����Ĵ�С
		////CBitmap bitmap;
		//bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Width());
		//m_dcCompatible.SelectObject(&bitmap);//������dcѡ��һ������λͼ
		//m_dcCompatible.BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);
	

	CView::OnLButtonUp(nFlags, point);
}



//����ƶ�����ͼ��  
void CinkPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	float c = -0.4;//ƫ�������Ƴ���
	if (m_fPointSize > 0)
	{
		float dx = (CPoint(m_MousePos - point)).x;//ǰ������λ��x�Ĳ�ֵ
		float dy = (CPoint(m_MousePos - point)).y;//ǰ������λ��y�Ĳ�ֵ
		float l = sqrt(dx*dx + dy*dy);//ǰ������λ�õ�ֱ�߾���

		for (int i = 0; i<int(l) + 1; i++)
		{
			int pNum = m_iPointNum % N;

			m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i);
			m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i);

			m_ColorPoint[pNum].size = m_fPointSize;

			m_ColorPoint[pNum].color[0] = GetRValue(m_clr);
			m_ColorPoint[pNum].color[1] = GetGValue(m_clr);
			m_ColorPoint[pNum].color[2] = GetBValue(m_clr);

			m_ColorPoint[pNum].life = 40;

			//������Ⱦ�Ĵ�ϸ
			if (thinkness == 0)
				m_ColorPoint[pNum].size *= 1.2;
			else if (thinkness ==1)
				m_ColorPoint[pNum].size *= 2.0;
			else if (thinkness == 2)
				m_ColorPoint[pNum].size *= 3.0;

			m_ColorPoint[pNum].color[0] -= 10;                     //�ı�����ɫ��ʹīɫ��ǳ         
			m_ColorPoint[pNum].color[1] -= 10;
			m_ColorPoint[pNum].color[2] -= 10;

			m_iPointNum++;

		}


		//ÿ���������ǰ�󣬵��С�����仯    
		if (m_LeftButtonDown)
		{
			m_fPointSize -= (l - 5) / 10;

			//���Ƶ��С�ķ�Χ	
			if (m_fPointSize > m_nLineWidth)
				m_fPointSize = m_nLineWidth;
			if (m_fPointSize < 2)
				m_fPointSize = 2;

		}
	}

	m_MousePos = point;
	CView::OnMouseMove(nFlags, point);
}




//���ڴ�С
void CinkPainterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_iWindowWidth = cx;
	m_iWindowHeight = cy;
	if (m_iWindowWidth < 10)
		m_iWindowWidth = 10;
	if (m_iWindowHeight < 10)
		m_iWindowHeight = 10;
	glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);//�趨�ӿڴ�С
	glMatrixMode(GL_PROJECTION);//�л���ͶӰ����
	glLoadIdentity();
	glOrtho(-m_iWindowWidth / 2, m_iWindowWidth / 2, -m_iWindowHeight / 2, m_iWindowHeight / 2, -100, 100);
	glMatrixMode(GL_MODELVIEW);//�л���ģ���ӿھ���

}

//��������
int CinkPainterView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}
	g_pView = (CView*)this;

	m_hWnd = GetSafeHwnd();
	m_hDC = ::GetDC(m_hWnd);

	if (SetWindowPixelFormat(m_hDC) == FALSE)
		return 0;
	if (CreateViewGLContext(m_hDC) == FALSE)
		return 0;

	LoadTextures();//��������
	SetTimer(0, 1, NULL);//��ʱ��

	return 0;
}

void CinkPainterView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here
	if (wglGetCurrentContext() != NULL)
		wglMakeCurrent(NULL, NULL);

	if (m_hGLContext != NULL)                //!!m_hGLcontent
	{
		wglDeleteContext(m_hGLContext);
		m_hGLContext = NULL;
	}
}

void CinkPainterView::OnTimer(UINT nIDEvent)
{
	if (!m_LeftButtonDown)
	{
		m_fPointSize -= ( 0.3*m_fPointSize);//ÿ��ˢ��ʱ�ʴ���С�����仯
		if (m_fPointSize < 0)
			m_fPointSize = 0;
	}

	if (spread)
	{
		Spread();
	}
	InvalidateRect(CRect(0, 0, 1, 1));
	//Invalidate(0);
	CView::OnTimer(nIDEvent);

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
	dlg.m_cc.Flags |= CC_RGBINIT | CC_FULLOPEN;
	dlg.m_cc.rgbResult = m_clr;
	if (IDOK == dlg.DoModal())
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
	Sleep(10);
	CString showPath = _T("Save Success!\n\nPath: ") + m_StrDBPath;
	Sleep(10);
	AfxMessageBox(showPath);

}


// ��������
void CinkPainterView::LoadTextures()
{
	AUX_RGBImageRec *TextureImage;
	TextureImage = auxDIBImageLoad(L"Material/texture.bmp");//����bmpͼ
	//TextureImage = auxDIBImageLoad(L"Material/texture2.bmp");//����bmpͼ

	glGenTextures(1, &m_texture[0]);// Ϊ��0��λͼ��������
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//������glGenTextures�������ɵ���������ְ󶨵���Ӧ��Ŀ��������
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		TextureImage->sizeX, TextureImage->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);//�������λͼ�ļ�(*.bmp)ת����������ͼ
	//�����˲� ʹ�����Բ�ֵ��������ͼ���е�ÿ��ͼ�������ֵ��Ȼ��������ͼ���������Բ�ֵ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//�ͷ�����
	if (TextureImage)
	{
		if (TextureImage->data)
		{
			free(TextureImage->data);
		}
		free(TextureImage);
	}
	//AfxMessageBox(L"test");
	TextureImage = auxDIBImageLoad(L"Material/Picture1.bmp");//����bmpͼ
	glGenTextures(1, &m_texture[1]);
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);                    //��mipmap������ʹ�����ڽ�����
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		1024, 1024, GL_RGB,
		GL_UNSIGNED_BYTE, tempdata);//����ֱ����𽥼�С��2*2�Ķ���ͼ�񣬲���ԭͼ���ŵ���ӽ�2���ݴη��ĳߴ���

}


// ��ͼ
void CinkPainterView::DrawWithOpenGL()
{
	int pNum;//�����Ŀ
			 //glClearColor(0, 0, 0, 0);//������ɫ
	glClearColor(1, 1, 1, 1);//������ɫ

							 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CRect rect;
	GetClientRect(&rect);//��ȡ��ǰ�ͻ�����Ĵ�С
	CDC*    pDC = GetDC();//��ȡ��ͼ���豸�ı�ָ��
	m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//���ö�ά����ӳ��

	glEnable(GL_BLEND);//���û��ģʽ

					   //if (m_bBrush)
					   //{
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);//�ڶ��ֻ��ģʽ
												 //}
												 //else
												 //glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);//�ڶ��ֻ��ģʽ

	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//Ϊ�������Ļ����������

	glBegin(GL_QUADS);//�ı���
	for (int i = m_iDrawStartPoint; i < m_iSimStartPoint; i++)
	{
		pNum = i % N;//��ֹ����Խ��

					 //glColor3ub(GetRValue(m_clr), GetGValue(m_clr), GetBValue(m_clr));
		glColor3ub(255 - GetRValue(m_clr), 255 - GetGValue(m_clr), 255 - GetBValue(m_clr));
		glTexCoord2f(0, 0);//��ǰ��������
		glVertex2f(m_ColorPoint[pNum].x - m_ColorPoint[pNum].size, m_ColorPoint[pNum].y - m_ColorPoint[pNum].size);
		glTexCoord2f(0, 1);
		glVertex2f(m_ColorPoint[pNum].x - m_ColorPoint[pNum].size, m_ColorPoint[pNum].y + m_ColorPoint[pNum].size);
		glTexCoord2f(1, 1);
		glVertex2f(m_ColorPoint[pNum].x + m_ColorPoint[pNum].size, m_ColorPoint[pNum].y + m_ColorPoint[pNum].size);
		glTexCoord2f(1, 0);
		glVertex2f(m_ColorPoint[pNum].x + m_ColorPoint[pNum].size, m_ColorPoint[pNum].y - m_ColorPoint[pNum].size);
	}
	glEnd();

	//========================
	//glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 512 - m_iWindowWidth / 2, 512 - m_iWindowHeight / 2, 0, 0, m_iWindowWidth, m_iWindowHeight);
	m_iDrawStartPoint = m_iSimStartPoint;
	//Sleep(100);
	//========================



	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//Ϊ�������Ļ����������

	glBegin(GL_QUADS);//�ı���
	for (int i = m_iSimStartPoint; i < m_iPointNum; i++)
	{
		pNum = i % N;//��ֹ����Խ��
		glColor3ub(255 - GetRValue(m_clr), 255 - GetGValue(m_clr), 255 - GetBValue(m_clr));
		//glColor3ub(GetRValue(m_clr), GetGValue(m_clr), GetBValue(m_clr));
		glTexCoord2f(0, 0);//��ǰ��������
		glVertex2f(m_ColorPoint[pNum].x - m_ColorPoint[pNum].size, m_ColorPoint[pNum].y - m_ColorPoint[pNum].size);
		glTexCoord2f(0, 1);
		glVertex2f(m_ColorPoint[pNum].x - m_ColorPoint[pNum].size, m_ColorPoint[pNum].y + m_ColorPoint[pNum].size);
		glTexCoord2f(1, 1);
		glVertex2f(m_ColorPoint[pNum].x + m_ColorPoint[pNum].size, m_ColorPoint[pNum].y + m_ColorPoint[pNum].size);
		glTexCoord2f(1, 0);
		glVertex2f(m_ColorPoint[pNum].x + m_ColorPoint[pNum].size, m_ColorPoint[pNum].y - m_ColorPoint[pNum].size);
	}
	glEnd();

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);//����
	//glBlendFunc(GL_ONE, GL_ONE);

	//glColor3ub(255, 255, 255);//����
	////glColor3ub(0, 0, 0);//����
	//glBegin(GL_QUADS);
	//glVertex2f(-m_iWindowWidth / 2, -m_iWindowHeight / 2);
	//glVertex2f(m_iWindowWidth / 2, -m_iWindowHeight / 2);
	//glVertex2f(m_iWindowWidth / 2, m_iWindowHeight / 2);
	//glVertex2f(-m_iWindowWidth / 2, m_iWindowHeight / 2);
	//glEnd();
	//glDisable(GL_BLEND);
}


// ��ʼ��OpenGL,�������ƻ�����RC����ʹ֮��Ϊ��ǰ���ƻ���
BOOL CinkPainterView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);//����RC

	if (m_hGLContext == NULL)//���ʧ��
		return FALSE;

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)//ѡΪ��ǰRCʧ��
		return FALSE;

	return TRUE;
}


// �������ظ�ʽ
BOOL CinkPainterView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;//����ͼ�ṹ��

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);//˵�������ݽṹ�ĳ���
	pixelDesc.nVersion = 1;//˵�������ݽṹ�İ汾

						   //ָ�����ػ�������ԣ�֧��window��֧��OpenGL�����ش�Ϊ˫���桢������ṹ
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;//ָ��RGBAģʽ
	pixelDesc.cColorBits = 32;//��ɫ��������ɫλƽ�����
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;      //�ۻ�����λƽ����� 
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;//ģ�建��λ��
	pixelDesc.cStencilBits = 8;//ģ�建��λ��
	pixelDesc.cAuxBuffers = 0;//�����������
	pixelDesc.iLayerType = PFD_MAIN_PLANE;
	pixelDesc.bReserved = 0;
	pixelDesc.dwLayerMask = 0;
	pixelDesc.dwVisibleMask = 0;
	pixelDesc.dwDamageMask = 0;

	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);
	if (m_GLPixelIndex == 0) // Choose default
	{
		m_GLPixelIndex = 1;
		if (DescribePixelFormat(hDC, m_GLPixelIndex,
			sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
			return FALSE;
	}

	if (!SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc))
		return FALSE;

	return TRUE;
}


// ��ɢЧ��
void CinkPainterView::Spread()
{
	for (int i = m_iSimStartPoint; i < m_iPointNum; i++)
	{
		int pNum = i % N;
		m_ColorPoint[pNum].size *= 1.01;
		m_ColorPoint[pNum].color[0] -= 0.001;
		m_ColorPoint[pNum].color[1] -= 0.001;
		m_ColorPoint[pNum].color[2] -= 0.001;
		m_ColorPoint[pNum].life--;
		if (m_ColorPoint[pNum].life <= 0)
			m_iSimStartPoint = i + 1;
	}
}

//
//CBitmap CinkPainterView::getBitmap()
//{
//	//// TODO: �ڴ���������������
//	//bmpScreen Bmp;
//	//CRect rect;
//	//GetClientRect(&rect);
//	//ClientToScreen(&rect);//P:�õ��ͻ���ת��Ϊ�����Ļ������ϵ  
//
//	//					  ////////////////////////ȷ��һ���ļ�����λ��  
//	//CString  sPath;
//	//GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//�õ������·��  
//	//sPath.ReleaseBuffer();
//	//int nPos = sPath.ReverseFind('\\');
//	//m_StrExePath = sPath.Left(nPos);
//	//sPath = m_StrExePath;
//	//m_StrDBPath = sPath + _T("\\SavePicture\\OurDrawPic.bmp");  //�õ�����Ŀ¼�µ����ݿ������·��  
//	//															//Bmp.screenShot(rect,0,0,"ScreenPic.bmp");  
//	//USES_CONVERSION;
//	//char* p = T2A(m_StrDBPath);
//
//
//
//	//////////////////��Ҫʵ�ֺ���  
//	//Bmp.screenShot(rect, 0, 0, p); ///�ڶ�����������û��
//	//Sleep(10);
//	//CString showPath = _T("Save Success!\n\nPath: ") + m_StrDBPath;
//	//AfxMessageBox(showPath);
//
//	return 0;
//}


BOOL CinkPainterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CView::OnEraseBkgnd(pDC);
	return true;
}
