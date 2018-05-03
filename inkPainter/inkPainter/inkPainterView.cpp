
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
#include "bmpScreen.h"
#include "windows.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


unsigned m_nLineWidth = 10;
COLORREF m_clr = RGB(0, 0, 0);
BYTE m_alpha = 100;// ��Ĳ�͸����
int m_spread = 1;
BOOL m_eraser = false;

CView* g_pView;

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
	ON_COMMAND(ID_SAVE, &CinkPainterView::OnSave)

	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

// CinkPainterView construction/destruction
BYTE tempdata[1024 * 1024 * 3];//���� ��������ʱ��
CinkPainterView::CinkPainterView()
	: m_StrExePath(_T(""))
	, m_StrDBPath(_T(""))
	, m_iDrawStartPoint(0)
	, m_iPointNum(0)
	, m_fPointSize(1.0)
	, m_iSimStartPoint(0)
{
	// TODO: add construction code here

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
		return;
	}


	if (pDoc->m_bImageChanged)
	{
		//��Ŀǰ�Ŀͻ������ݴ����������¼��ص��ӿ�

		if (!m_dcCompatible.m_hDC)//�ж��Ƿ��Ѿ���������DC
		{

			m_dcCompatible.CreateCompatibleDC(pDC);
			CRect rect;
			GetClientRect(&rect);//��ȡ��ǰ�ͻ�����Ĵ�С
			bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
			m_dcCompatible.SelectObject(&bitmap);//������dcѡ��һ������λͼ
			m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
		}

	}
	DrawWithOpenGL();
	SwapBuffers(m_hDC);
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

	CView::OnLButtonUp(nFlags, point);
}



//����ƶ�����ͼ��  
void CinkPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	float c;//ƫ�������Ƴ���
	if (m_fPointSize > 0)
	{
		float dx = (CPoint(m_MousePos - point)).x;//ǰ������λ��x�Ĳ�ֵ
		float dy = (CPoint(m_MousePos - point)).y;//ǰ������λ��y�Ĳ�ֵ
		float l = sqrt(dx*dx + dy*dy);//ǰ������λ�õ�ֱ�߾���

									  //int i = 0;
		for (int i = 0; i<int(l) + 1; i++)
		{
			int pNum = m_iPointNum % m_num;
			if (m_spread == 0) {
				/*------------------------------------��ɢЧ������ 1---------------------------------------*/
				c = -0.4;//ƫ�������Ƴ���
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i);
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i);
			}
			if (m_spread == 1 || m_spread == 4) {
				/*------------------------------------��ɢЧ������ 2---------------------------------------*/
				c = -0.4;//ƫ�������Ƴ���
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i)*100.2 / 10;
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i) / 10;
			}
			if (m_spread == 2) {
				/*------------------------------------��ɢЧ������ 3---------------------------------------*/
				c = 1.05;//ƫ�������Ƴ���
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i)*100.2 / 10;
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i)*100.2 / 10;
			}
			if (m_spread == 3) {
				/*------------------------------------��ɢЧ������ 3---------------------------------------*/
				c = -0.4;//ƫ�������Ƴ���
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i) * 300 / 10;
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i) * 300 / 10;
			}

			m_ColorPoint[pNum].size = m_fPointSize;

			m_ColorPoint[pNum].color[0] = 255 - GetRValue(m_clr);
			m_ColorPoint[pNum].color[1] = 255 - GetGValue(m_clr);
			m_ColorPoint[pNum].color[2] = 255 - GetBValue(m_clr);


			m_ColorPoint[pNum].life = 40;

			m_ColorPoint[pNum].color[0] *= 0.1;                     //�ı�����ɫ��ʹīɫ��ǳ         
			m_ColorPoint[pNum].color[1] *= 0.1;
			m_ColorPoint[pNum].color[2] *= 0.1;

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
		m_fPointSize -= (0.3*m_fPointSize);//�ɿ����ʱ�ʴ����ɵ�Ч��
		if (m_fPointSize < 0)
			m_fPointSize = 0;
	}

	Spread();

	Invalidate(0);
	CView::OnTimer(nIDEvent);

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
	CString  sPath; //ȷ��һ���ļ�����λ��
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//�õ������·��  
	sPath.ReleaseBuffer();
	int nPos = sPath.ReverseFind('\\');
	m_StrExePath = sPath.Left(nPos);
	sPath = m_StrExePath;
	m_StrDBPath = sPath + _T("\\SavePicture\\OurDrawPic.bmp");  //�õ�����Ŀ¼�µ����ݿ������·��  
																//Bmp.screenShot(rect,0,0,"ScreenPic.bmp");  
	USES_CONVERSION;
	char* p = T2A(m_StrDBPath);

	//��Ҫʵ�ֺ���  
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

	TextureImage = auxDIBImageLoad(L"Material/texture.bmp");//����bmpͼ
	glGenTextures(1, &m_texture[1]);// Ϊ��0��λͼ��������
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);//������glGenTextures�������ɵ���������ְ󶨵���Ӧ��Ŀ��������
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

	TextureImage = auxDIBImageLoad(L"Material/texture3.bmp");//����bmpͼ
	glGenTextures(1, &m_texture[2]);
	glBindTexture(GL_TEXTURE_2D, m_texture[2]);//������glGenTextures�������ɵ���������ְ󶨵���Ӧ��Ŀ��������
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

	TextureImage = auxDIBImageLoad(L"Material/texture6.bmp");//����bmpͼ
	glGenTextures(1, &m_texture[3]);
	glBindTexture(GL_TEXTURE_2D, m_texture[3]);//������glGenTextures�������ɵ���������ְ󶨵���Ӧ��Ŀ��������
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
}


// ��ͼ
void CinkPainterView::DrawWithOpenGL()
{
	int pNum;//�����Ŀ
	glClearColor(1, 1, 1, 1);//������ɫ

							 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CRect rect;
	GetClientRect(&rect);//��ȡ��ǰ�ͻ�����Ĵ�С
	CDC*    pDC = GetDC();//��ȡ��ͼ���豸�ı�ָ��
	m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//���ö�ά����ӳ��

	glEnable(GL_BLEND);//���û��ģʽ
	glEnable(GL_ALPHA_TEST);

	if (m_eraser)
		glBlendFunc(GL_ONE, GL_ONE);
	else
		glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);//��Ŀ����ɫ���ԣ�1-Դ��ɫ��

	if (m_spread == 0)
		glBindTexture(GL_TEXTURE_2D, m_texture[0]);//Ϊ�������Ļ����������
	if (m_spread == 1 || m_spread == 4)
		glBindTexture(GL_TEXTURE_2D, m_texture[1]);//Ϊ�������Ļ����������
	if (m_spread == 2)
		glBindTexture(GL_TEXTURE_2D, m_texture[2]);//Ϊ�������Ļ����������
	if (m_spread == 3)
		glBindTexture(GL_TEXTURE_2D, m_texture[3]);//Ϊ�������Ļ����������

	glBegin(GL_QUADS);//�ı���

					  //int i = m_iDrawStartPoint;
	for (int i = m_iDrawStartPoint; i < m_iSimStartPoint; i++)
	{
		//pNum = i % N;//��ֹ����Խ��
		pNum = i % m_num;//��ֹ����Խ��

		glColor4ub(m_ColorPoint[pNum].color[0], m_ColorPoint[pNum].color[1], m_ColorPoint[pNum].color[2], m_alpha);

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

	m_iDrawStartPoint = m_iSimStartPoint;//Ϊ��ֻ������󻭵���һ��

	if (m_spread == 0)
		glBindTexture(GL_TEXTURE_2D, m_texture[0]);//Ϊ�������Ļ����������
	if (m_spread == 1|| m_spread == 4)
		glBindTexture(GL_TEXTURE_2D, m_texture[1]);//Ϊ�������Ļ����������
	if (m_spread == 2)
		glBindTexture(GL_TEXTURE_2D, m_texture[2]);//Ϊ�������Ļ����������
	if (m_spread == 3)
		glBindTexture(GL_TEXTURE_2D, m_texture[3]);//Ϊ�������Ļ����������

	glBegin(GL_QUADS);//�ı���
	for (int i = m_iSimStartPoint; i < m_iPointNum; i++)
	{
		//pNum = i % N;//��ֹ����Խ��
		pNum = i % m_num;//��ֹ����Խ��
		glColor4ub(m_ColorPoint[pNum].color[0], m_ColorPoint[pNum].color[1], m_ColorPoint[pNum].color[2], m_alpha);
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
		int pNum = i % m_num;
		if (m_spread == 0|| m_spread == 2|| m_spread == 3)
			m_ColorPoint[pNum].size *= 1.01;
		if (m_spread == 1 || m_spread == 4)
			m_ColorPoint[pNum].size *= 1.0;

		m_ColorPoint[pNum].life--;
		if (m_ColorPoint[pNum].life <= 0)
			m_iSimStartPoint = i + 1;
	}
}



BOOL CinkPainterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//return CView::OnEraseBkgnd(pDC);
	return true;//����windows�ı�������
}

