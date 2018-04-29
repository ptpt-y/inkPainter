#include "stdafx.h"
#include "bmpScreen.h"


bmpScreen::bmpScreen()
{
}


bmpScreen::~bmpScreen()
{
}
//***********************************************  
//*******************截图操作汇总***************/  
//************************************************/  
/*
1.  CDC dc;
dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);  //创建dc

2.   memDC.CreateCompatibleDC(&dc);  //创建与当前DC兼容的内存DC

3.   memBitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());  //创建一块指定大小的位图

4.   oldmemBitmap = memDC.SelectObject(&memBitmap); // 将该位图选入到内存DC中   memDC

5.   memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc,left, top, SRCCOPY); // 将图形dc拷贝到一个目标memDC中

最后进行 BMP文件存取（BMP图像文件被分成4个部分：位图文件头（Bitmap File Header）、位图信息头（Bitmap Info Header）、颜色表（Color Map）和位图数据（即图像数据，Data Bits或Data Body）。）
*/


void bmpScreen::screenShot(CRect rect, int left, int top, char *name)

{
	CBitmap*  m_pBitmap;                                                     // 加入类成员  
																			 //CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();                     // 获得截图窗口的指针，默认为主窗口，可以更改为其他的窗口。  
																			 //   CPaintDC   dc(pMainFrame);   

																			 //为屏幕创建设备描述表  
	CDC dc;
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);


	m_pBitmap = new   CBitmap;
	m_pBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());




	CDC   memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBitmap, *oldmemBitmap;                                        // 建立和屏幕兼容的bitmap  

	memBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	oldmemBitmap = memDC.SelectObject(&memBitmap);//将memBitmap选入内存DC  
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, rect.left, rect.top, SRCCOPY);  //P:rect.left, rect.top原位图相对屏幕的坐标  
																						 //memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc,0, 0, SRCCOPY);    
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);                                               // 获得位图信息   ///P:得到创建的位图memBitmap的信息到bmp中  

	FILE *fp = fopen(name, "wb+");
	
	if (fp == NULL)
	{
		AfxMessageBox(L"Open Failed");
	}

	BITMAPINFOHEADER bih = { 0 };                                              // 位图信息头  
	bih.biBitCount = bmp.bmBitsPixel;                                        // 每个像素字节大小  
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;                                             // 高度  
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;                       // 图像数据大小  
	bih.biWidth = bmp.bmWidth;                                               // 宽度  

	BITMAPFILEHEADER bfh = { 0 };                                              // 位图文件头  
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);     // 到位图数据的偏移量  
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;            // 文件总的大小  
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);                           //写入位图文件头  

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);                           //写入位图信息头  

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];                    //申请内存保存位图数据  

	GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, rect.Height(), p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);                                    //获取位图数据  

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);                       //写入位图数据  
	delete[] p;
	if (fp!=NULL)
		fclose(fp);
	memDC.SelectObject(oldmemBitmap);
	memDC.DeleteDC();

}