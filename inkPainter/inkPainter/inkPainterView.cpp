
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
BYTE m_alpha = 100;// 点的不透明度
int m_spread = 1;
BOOL m_eraser = false;

CView* g_pView;

// CinkPainterView

IMPLEMENT_DYNCREATE(CinkPainterView, CView)

//消息映射函数
BEGIN_MESSAGE_MAP(CinkPainterView, CView)
	//{{AFX_MSG(CinkPainterView)   /*用宏将消息响应和函数关联起来*/
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_CREATE()//捕获客户区窗口的消息
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
BYTE tempdata[1024 * 1024 * 3];//缓存 加载纹理时用
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
//view中图像更改时，进行重绘
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
		//把目前的客户区内容存下来再重新加载到视口

		if (!m_dcCompatible.m_hDC)//判断是否已经创建兼容DC
		{

			m_dcCompatible.CreateCompatibleDC(pDC);
			CRect rect;
			GetClientRect(&rect);//获取当前客户区域的大小
			bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
			m_dcCompatible.SelectObject(&bitmap);//给兼容dc选择一个兼容位图
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
	//把当前点击的点的位置赋给点m_point  
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



//鼠标移动绘制图形  
void CinkPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	float c;//偏移量控制常数
	if (m_fPointSize > 0)
	{
		float dx = (CPoint(m_MousePos - point)).x;//前后两个位置x的差值
		float dy = (CPoint(m_MousePos - point)).y;//前后两个位置y的差值
		float l = sqrt(dx*dx + dy*dy);//前后两个位置的直线距离

									  //int i = 0;
		for (int i = 0; i<int(l) + 1; i++)
		{
			int pNum = m_iPointNum % m_num;
			if (m_spread == 0) {
				/*------------------------------------扩散效果函数 1---------------------------------------*/
				c = -0.4;//偏移量控制常数
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i);
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i);
			}
			if (m_spread == 1 || m_spread == 4) {
				/*------------------------------------扩散效果函数 2---------------------------------------*/
				c = -0.4;//偏移量控制常数
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i)*100.2 / 10;
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i) / 10;
			}
			if (m_spread == 2) {
				/*------------------------------------扩散效果函数 3---------------------------------------*/
				c = 1.05;//偏移量控制常数
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i)*100.2 / 10;
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i)*100.2 / 10;
			}
			if (m_spread == 3) {
				/*------------------------------------扩散效果函数 3---------------------------------------*/
				c = -0.4;//偏移量控制常数
				m_ColorPoint[pNum].x = m_MousePos.x - m_iWindowWidth / 2 + dx / l*i*pow(c, i) * 300 / 10;
				m_ColorPoint[pNum].y = -m_MousePos.y + m_iWindowHeight / 2 + dy / l*i*pow(c, i) * 300 / 10;
			}

			m_ColorPoint[pNum].size = m_fPointSize;

			m_ColorPoint[pNum].color[0] = 255 - GetRValue(m_clr);
			m_ColorPoint[pNum].color[1] = 255 - GetGValue(m_clr);
			m_ColorPoint[pNum].color[2] = 255 - GetBValue(m_clr);


			m_ColorPoint[pNum].life = 40;

			m_ColorPoint[pNum].color[0] *= 0.1;                     //改变点的颜色，使墨色较浅         
			m_ColorPoint[pNum].color[1] *= 0.1;
			m_ColorPoint[pNum].color[2] *= 0.1;

			m_iPointNum++;

		}


		//每次鼠标落下前后，点大小发生变化    
		if (m_LeftButtonDown)
		{
			m_fPointSize -= (l - 5) / 10;

			//控制点大小的范围	
			if (m_fPointSize > m_nLineWidth)
				m_fPointSize = m_nLineWidth;
			if (m_fPointSize < 2)
				m_fPointSize = 2;

		}
	}

	m_MousePos = point;
	CView::OnMouseMove(nFlags, point);
}




//窗口大小
void CinkPainterView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	m_iWindowWidth = cx;
	m_iWindowHeight = cy;
	if (m_iWindowWidth < 10)
		m_iWindowWidth = 10;
	if (m_iWindowHeight < 10)
		m_iWindowHeight = 10;
	glViewport(0, 0, m_iWindowWidth, m_iWindowHeight);//设定视口大小
	glMatrixMode(GL_PROJECTION);//切换到投影矩阵
	glLoadIdentity();
	glOrtho(-m_iWindowWidth / 2, m_iWindowWidth / 2, -m_iWindowHeight / 2, m_iWindowHeight / 2, -100, 100);
	glMatrixMode(GL_MODELVIEW);//切换到模型视口矩阵

}

//创建窗口
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

	LoadTextures();//载入纹理
	SetTimer(0, 1, NULL);//定时器

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
		m_fPointSize -= (0.3*m_fPointSize);//松开左键时笔触过渡的效果
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
	// TODO: 在此添加命令处理程序代码
	bmpScreen Bmp;
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);//P:得到客户区转化为相对屏幕的坐标系  
	CString  sPath; //确定一个文件保存位置
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//得到程序的路径  
	sPath.ReleaseBuffer();
	int nPos = sPath.ReverseFind('\\');
	m_StrExePath = sPath.Left(nPos);
	sPath = m_StrExePath;
	m_StrDBPath = sPath + _T("\\SavePicture\\OurDrawPic.bmp");  //得到程序目录下的数据库的完整路径  
																//Bmp.screenShot(rect,0,0,"ScreenPic.bmp");  
	USES_CONVERSION;
	char* p = T2A(m_StrDBPath);

	//主要实现函数  
	Bmp.screenShot(rect, 0, 0, p); ///第二第三个参数没用
	Sleep(10);
	CString showPath = _T("Save Success!\n\nPath: ") + m_StrDBPath;
	Sleep(10);
	AfxMessageBox(showPath);

}


// 载入纹理
void CinkPainterView::LoadTextures()
{
	AUX_RGBImageRec *TextureImage;
	TextureImage = auxDIBImageLoad(L"Material/texture.bmp");//载入bmp图
	glGenTextures(1, &m_texture[0]);// 为第0个位图创建纹理
	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//将调用glGenTextures函数生成的纹理的名字绑定到对应的目标纹理上
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		TextureImage->sizeX, TextureImage->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);//将载入的位图文件(*.bmp)转换成纹理贴图
													  //线形滤波 使用线性插值计算两个图像中的每个图像的纹素值，然后在两个图像间进行线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//释放纹理
	if (TextureImage)
	{
		if (TextureImage->data)
		{
			free(TextureImage->data);
		}
		free(TextureImage);
	}

	TextureImage = auxDIBImageLoad(L"Material/texture.bmp");//载入bmp图
	glGenTextures(1, &m_texture[1]);// 为第0个位图创建纹理
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);//将调用glGenTextures函数生成的纹理的名字绑定到对应的目标纹理上
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		TextureImage->sizeX, TextureImage->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);//将载入的位图文件(*.bmp)转换成纹理贴图
													  //线形滤波 使用线性插值计算两个图像中的每个图像的纹素值，然后在两个图像间进行线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//释放纹理
	if (TextureImage)
	{
		if (TextureImage->data)
		{
			free(TextureImage->data);
		}
		free(TextureImage);
	}

	TextureImage = auxDIBImageLoad(L"Material/texture3.bmp");//载入bmp图
	glGenTextures(1, &m_texture[2]);
	glBindTexture(GL_TEXTURE_2D, m_texture[2]);//将调用glGenTextures函数生成的纹理的名字绑定到对应的目标纹理上
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		TextureImage->sizeX, TextureImage->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);//将载入的位图文件(*.bmp)转换成纹理贴图
													  //线形滤波 使用线性插值计算两个图像中的每个图像的纹素值，然后在两个图像间进行线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//释放纹理
	if (TextureImage)
	{
		if (TextureImage->data)
		{
			free(TextureImage->data);
		}
		free(TextureImage);
	}

	TextureImage = auxDIBImageLoad(L"Material/texture6.bmp");//载入bmp图
	glGenTextures(1, &m_texture[3]);
	glBindTexture(GL_TEXTURE_2D, m_texture[3]);//将调用glGenTextures函数生成的纹理的名字绑定到对应的目标纹理上
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		TextureImage->sizeX, TextureImage->sizeY,
		GL_RGB, GL_UNSIGNED_BYTE, TextureImage->data);//将载入的位图文件(*.bmp)转换成纹理贴图
													  //线形滤波 使用线性插值计算两个图像中的每个图像的纹素值，然后在两个图像间进行线性插值
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	//释放纹理
	if (TextureImage)
	{
		if (TextureImage->data)
		{
			free(TextureImage->data);
		}
		free(TextureImage);
	}
}


// 绘图
void CinkPainterView::DrawWithOpenGL()
{
	int pNum;//点的数目
	glClearColor(1, 1, 1, 1);//背景颜色

							 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CRect rect;
	GetClientRect(&rect);//获取当前客户区域的大小
	CDC*    pDC = GetDC();//获取视图的设备文本指针
	m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//启用二维纹理映射

	glEnable(GL_BLEND);//启用混合模式
	glEnable(GL_ALPHA_TEST);

	if (m_eraser)
		glBlendFunc(GL_ONE, GL_ONE);
	else
		glBlendFunc(GL_ONE_MINUS_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR);//用目标颜色乘以（1-源颜色）

	if (m_spread == 0)
		glBindTexture(GL_TEXTURE_2D, m_texture[0]);//为接下来的画笔添加纹理
	if (m_spread == 1 || m_spread == 4)
		glBindTexture(GL_TEXTURE_2D, m_texture[1]);//为接下来的画笔添加纹理
	if (m_spread == 2)
		glBindTexture(GL_TEXTURE_2D, m_texture[2]);//为接下来的画笔添加纹理
	if (m_spread == 3)
		glBindTexture(GL_TEXTURE_2D, m_texture[3]);//为接下来的画笔添加纹理

	glBegin(GL_QUADS);//四边形

					  //int i = m_iDrawStartPoint;
	for (int i = m_iDrawStartPoint; i < m_iSimStartPoint; i++)
	{
		//pNum = i % N;//防止数组越界
		pNum = i % m_num;//防止数组越界

		glColor4ub(m_ColorPoint[pNum].color[0], m_ColorPoint[pNum].color[1], m_ColorPoint[pNum].color[2], m_alpha);

		glTexCoord2f(0, 0);//当前纹理坐标
		glVertex2f(m_ColorPoint[pNum].x - m_ColorPoint[pNum].size, m_ColorPoint[pNum].y - m_ColorPoint[pNum].size);
		glTexCoord2f(0, 1);
		glVertex2f(m_ColorPoint[pNum].x - m_ColorPoint[pNum].size, m_ColorPoint[pNum].y + m_ColorPoint[pNum].size);
		glTexCoord2f(1, 1);
		glVertex2f(m_ColorPoint[pNum].x + m_ColorPoint[pNum].size, m_ColorPoint[pNum].y + m_ColorPoint[pNum].size);
		glTexCoord2f(1, 0);
		glVertex2f(m_ColorPoint[pNum].x + m_ColorPoint[pNum].size, m_ColorPoint[pNum].y - m_ColorPoint[pNum].size);
	}
	glEnd();

	m_iDrawStartPoint = m_iSimStartPoint;//为了只保留最后画的那一笔

	if (m_spread == 0)
		glBindTexture(GL_TEXTURE_2D, m_texture[0]);//为接下来的画笔添加纹理
	if (m_spread == 1|| m_spread == 4)
		glBindTexture(GL_TEXTURE_2D, m_texture[1]);//为接下来的画笔添加纹理
	if (m_spread == 2)
		glBindTexture(GL_TEXTURE_2D, m_texture[2]);//为接下来的画笔添加纹理
	if (m_spread == 3)
		glBindTexture(GL_TEXTURE_2D, m_texture[3]);//为接下来的画笔添加纹理

	glBegin(GL_QUADS);//四边形
	for (int i = m_iSimStartPoint; i < m_iPointNum; i++)
	{
		//pNum = i % N;//防止数组越界
		pNum = i % m_num;//防止数组越界
		glColor4ub(m_ColorPoint[pNum].color[0], m_ColorPoint[pNum].color[1], m_ColorPoint[pNum].color[2], m_alpha);
		glTexCoord2f(0, 0);//当前纹理坐标
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


// 初始化OpenGL,创建绘制环境（RC）并使之成为当前绘制环境
BOOL CinkPainterView::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(hDC);//创建RC

	if (m_hGLContext == NULL)//如果失败
		return FALSE;

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)//选为当前RC失败
		return FALSE;

	return TRUE;
}


// 设置像素格式
BOOL CinkPainterView::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;//像素图结构体

	pixelDesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);//说明该数据结构的长度
	pixelDesc.nVersion = 1;//说明该数据结构的版本

						   //指定像素缓存的属性：支持window、支持OpenGL。像素存为双缓存、立方体结构
	pixelDesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_STEREO_DONTCARE;

	pixelDesc.iPixelType = PFD_TYPE_RGBA;//指定RGBA模式
	pixelDesc.cColorBits = 32;//颜色缓存中颜色位平面个数
	pixelDesc.cRedBits = 8;
	pixelDesc.cRedShift = 16;
	pixelDesc.cGreenBits = 8;
	pixelDesc.cGreenShift = 8;
	pixelDesc.cBlueBits = 8;
	pixelDesc.cBlueShift = 0;
	pixelDesc.cAlphaBits = 0;
	pixelDesc.cAlphaShift = 0;
	pixelDesc.cAccumBits = 64;      //累积缓存位平面个数 
	pixelDesc.cAccumRedBits = 16;
	pixelDesc.cAccumGreenBits = 16;
	pixelDesc.cAccumBlueBits = 16;
	pixelDesc.cAccumAlphaBits = 0;
	pixelDesc.cDepthBits = 32;//模板缓存位数
	pixelDesc.cStencilBits = 8;//模板缓存位数
	pixelDesc.cAuxBuffers = 0;//辅助缓存个数
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


// 扩散效果
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CView::OnEraseBkgnd(pDC);
	return true;//禁用windows的背景擦除
}

