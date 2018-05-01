
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

	ON_COMMAND(ID_SETTINGS_SIZE, &CinkPainterView::OnSettingSize)
	ON_COMMAND(ID_SETTINGS_COLOR, &CinkPainterView::OnSettingColor)
	ON_COMMAND(ID_SETTINGS_BRUSH, &CinkPainterView::OnSettingBrush)
	ON_COMMAND(ID_SETTINGS_OPACITY, &CinkPainterView::OnSettingOpacity)
	ON_COMMAND(ID_SAVE, &CinkPainterView::OnSave)

	ON_WM_ERASEBKGND()

END_MESSAGE_MAP()

// CinkPainterView construction/destruction
BYTE tempdata[1024 * 1024 * 3];//缓存 加载纹理时用
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
//view中图像更改时，进行重绘
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
	//	m_iSimStartPoint = m_iDrawStartPoint = m_iPointNum = 0;//初始化
	//	resetView = false;
	//}
	//===================================


	if (pDoc->m_bImageChanged)
	{
		//就是这里!!!!
		//要在这里把目前的视口内容存下来再重新加载到视口
		//AfxMessageBox(L"!!! ");

			if (!m_dcCompatible.m_hDC)//判断是否已经创建兼容DC
	{

		m_dcCompatible.CreateCompatibleDC(pDC);
		CRect rect;
		GetClientRect(&rect);//获取当前客户区域的大小
							 //CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
		m_dcCompatible.SelectObject(&bitmap);//给兼容dc选择一个兼容位图
		m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	}

		//CRect rect;
		//GetClientRect(&rect);//获取当前客户区域的大小
		////CBitmap bitmap;
		////bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
		////m_dcCompatible.SelectObject(&bitmap);//给兼容dc选择一个兼容位图
		////m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
		//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcCompatible, 0, 0, SRCCOPY);
		////AfxMessageBox(L"???");



		////glBindTexture(GL_TEXTURE_2D, m_texture[1]);//将生成的纹理名称绑定到指定的纹理上
		////m_ImageWidth = pDoc->m_ImageWidth;
		////m_ImageHeight = pDoc->m_ImageHeight;

		////将pDoc->m_pImage指针指向的图片的部分作为2D纹理
		////在程序中只要不断改变指针指向的图片就能自动更新纹理
		////glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_ImageWidth, m_ImageHeight, GL_RGB, GL_UNSIGNED_BYTE, pDoc->m_pImage);
		//pDoc->m_bImageChanged = FALSE;

		//===========================可能可行但实在是太浪费内存了跑不动============
		//OnSave();
		//CRect    rect;     //客户区域
		//CDC*    pDC = GetDC();//获取视图的设备文本指针
		//CDC    dcComp; //声明内存设备文本对象
		//			   //得到客户区尺寸
		//GetClientRect(&rect);
		////组装m_MemBitmap对象
		//bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Width());
		//bitmap.m_hObject = (HBITMAP)::LoadImage(NULL, _T("Material/texture.bmp"), IMAGE_BITMAP, 500, 400, LR_LOADFROMFILE);
		////bitmap.LoadImage("Material/texture.bmp");
		////创建内存设备文本对象
		//dcComp.CreateCompatibleDC(pDC);
		////内存设备环境将位图对象选入
		//dcComp.SelectObject(&bitmap);
		////用位传输函数显示出来
		//pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dcComp, 0, 0, SRCCOPY);
		////析构内存设备文本对象
		//dcComp.DeleteDC();
		////释放设备文本指针
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

	//if (!m_dcCompatible.m_hDC)//判断是否已经创建兼容DC
	//{
	//	//AfxMessageBox(L"set brush ");

	//	m_dcCompatible.CreateCompatibleDC(&dc);
	//	CRect rect;
	//	GetClientRect(&rect);//获取当前客户区域的大小
	//						 //CBitmap bitmap;
	//	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Width());
	//	m_dcCompatible.SelectObject(&bitmap);//给兼容dc选择一个兼容位图
	//	m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
	//}
		//CRect rect;
		//GetClientRect(&rect);//获取当前客户区域的大小
		////CBitmap bitmap;
		//bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Width());
		//m_dcCompatible.SelectObject(&bitmap);//给兼容dc选择一个兼容位图
		//m_dcCompatible.BitBlt(0,0,rect.Width(),rect.Height(),&dc,0,0,SRCCOPY);
	

	CView::OnLButtonUp(nFlags, point);
}



//鼠标移动绘制图形  
void CinkPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	float c = -0.4;//偏移量控制常数
	if (m_fPointSize > 0)
	{
		float dx = (CPoint(m_MousePos - point)).x;//前后两个位置x的差值
		float dy = (CPoint(m_MousePos - point)).y;//前后两个位置y的差值
		float l = sqrt(dx*dx + dy*dy);//前后两个位置的直线距离

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

			//画笔晕染的粗细
			if (thinkness == 0)
				m_ColorPoint[pNum].size *= 1.2;
			else if (thinkness ==1)
				m_ColorPoint[pNum].size *= 2.0;
			else if (thinkness == 2)
				m_ColorPoint[pNum].size *= 3.0;

			m_ColorPoint[pNum].color[0] -= 10;                     //改变点的颜色，使墨色较浅         
			m_ColorPoint[pNum].color[1] -= 10;
			m_ColorPoint[pNum].color[2] -= 10;

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
		m_fPointSize -= ( 0.3*m_fPointSize);//每次刷新时笔触大小发生变化
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

//画笔大小设置
void CinkPainterView::OnSettingSize()
{
	//模态对话框 点击OK后对话框被销毁
	CSettingSizeDlg sizeDlg;
	sizeDlg.m_nLineWidth = m_nLineWidth;
	if (IDOK == sizeDlg.DoModal())
	{
		m_nLineWidth = sizeDlg.m_nLineWidth;
	}
}

//画笔颜色设置
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

//画笔笔刷设置
void CinkPainterView::OnSettingBrush()
{
	AfxMessageBox(L"set brush ");
}

//画笔轻重 不透明度 设置
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
	// TODO: 在此添加命令处理程序代码
	bmpScreen Bmp;
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);//P:得到客户区转化为相对屏幕的坐标系  

						  ////////////////////////确定一个文件保存位置  
	CString  sPath;
	GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//得到程序的路径  
	sPath.ReleaseBuffer();
	int nPos = sPath.ReverseFind('\\');
	m_StrExePath = sPath.Left(nPos);
	sPath = m_StrExePath;
	m_StrDBPath = sPath + _T("\\SavePicture\\OurDrawPic.bmp");  //得到程序目录下的数据库的完整路径  
															   //Bmp.screenShot(rect,0,0,"ScreenPic.bmp");  
	USES_CONVERSION;
	char* p = T2A(m_StrDBPath);



	////////////////主要实现函数  
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
	//TextureImage = auxDIBImageLoad(L"Material/texture2.bmp");//载入bmp图

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
	//AfxMessageBox(L"test");
	TextureImage = auxDIBImageLoad(L"Material/Picture1.bmp");//载入bmp图
	glGenTextures(1, &m_texture[1]);
	glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);                    //在mipmap基层上使用最邻近过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
		1024, 1024, GL_RGB,
		GL_UNSIGNED_BYTE, tempdata);//定义分辨率逐渐减小到2*2的多重图像，并将原图缩放到最接近2的幂次方的尺寸上

}


// 绘图
void CinkPainterView::DrawWithOpenGL()
{
	int pNum;//点的数目
			 //glClearColor(0, 0, 0, 0);//背景颜色
	glClearColor(1, 1, 1, 1);//背景颜色

							 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CRect rect;
	GetClientRect(&rect);//获取当前客户区域的大小
	CDC*    pDC = GetDC();//获取视图的设备文本指针
	m_dcCompatible.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);//启用二维纹理映射

	glEnable(GL_BLEND);//启用混合模式

					   //if (m_bBrush)
					   //{
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);//第二种混合模式
												 //}
												 //else
												 //glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);//第二种混合模式

	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//为接下来的画笔添加纹理

	glBegin(GL_QUADS);//四边形
	for (int i = m_iDrawStartPoint; i < m_iSimStartPoint; i++)
	{
		pNum = i % N;//防止数组越界

					 //glColor3ub(GetRValue(m_clr), GetGValue(m_clr), GetBValue(m_clr));
		glColor3ub(255 - GetRValue(m_clr), 255 - GetGValue(m_clr), 255 - GetBValue(m_clr));
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

	//========================
	//glBindTexture(GL_TEXTURE_2D, m_texture[1]);
	//glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 512 - m_iWindowWidth / 2, 512 - m_iWindowHeight / 2, 0, 0, m_iWindowWidth, m_iWindowHeight);
	m_iDrawStartPoint = m_iSimStartPoint;
	//Sleep(100);
	//========================



	glBindTexture(GL_TEXTURE_2D, m_texture[0]);//为接下来的画笔添加纹理

	glBegin(GL_QUADS);//四边形
	for (int i = m_iSimStartPoint; i < m_iPointNum; i++)
	{
		pNum = i % N;//防止数组越界
		glColor3ub(255 - GetRValue(m_clr), 255 - GetGValue(m_clr), 255 - GetBValue(m_clr));
		//glColor3ub(GetRValue(m_clr), GetGValue(m_clr), GetBValue(m_clr));
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
	//glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);//反相
	//glBlendFunc(GL_ONE, GL_ONE);

	//glColor3ub(255, 255, 255);//背景
	////glColor3ub(0, 0, 0);//背景
	//glBegin(GL_QUADS);
	//glVertex2f(-m_iWindowWidth / 2, -m_iWindowHeight / 2);
	//glVertex2f(m_iWindowWidth / 2, -m_iWindowHeight / 2);
	//glVertex2f(m_iWindowWidth / 2, m_iWindowHeight / 2);
	//glVertex2f(-m_iWindowWidth / 2, m_iWindowHeight / 2);
	//glEnd();
	//glDisable(GL_BLEND);
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
//	//// TODO: 在此添加命令处理程序代码
//	//bmpScreen Bmp;
//	//CRect rect;
//	//GetClientRect(&rect);
//	//ClientToScreen(&rect);//P:得到客户区转化为相对屏幕的坐标系  
//
//	//					  ////////////////////////确定一个文件保存位置  
//	//CString  sPath;
//	//GetModuleFileName(NULL, sPath.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);//得到程序的路径  
//	//sPath.ReleaseBuffer();
//	//int nPos = sPath.ReverseFind('\\');
//	//m_StrExePath = sPath.Left(nPos);
//	//sPath = m_StrExePath;
//	//m_StrDBPath = sPath + _T("\\SavePicture\\OurDrawPic.bmp");  //得到程序目录下的数据库的完整路径  
//	//															//Bmp.screenShot(rect,0,0,"ScreenPic.bmp");  
//	//USES_CONVERSION;
//	//char* p = T2A(m_StrDBPath);
//
//
//
//	//////////////////主要实现函数  
//	//Bmp.screenShot(rect, 0, 0, p); ///第二第三个参数没用
//	//Sleep(10);
//	//CString showPath = _T("Save Success!\n\nPath: ") + m_StrDBPath;
//	//AfxMessageBox(showPath);
//
//	return 0;
//}


BOOL CinkPainterView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//return CView::OnEraseBkgnd(pDC);
	return true;
}
