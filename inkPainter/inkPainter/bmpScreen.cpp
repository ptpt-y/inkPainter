#include "stdafx.h"
#include "bmpScreen.h"


bmpScreen::bmpScreen()
{
}


bmpScreen::~bmpScreen()
{
}
//***********************************************  
//*******************��ͼ��������***************/  
//************************************************/  
/*
1.  CDC dc;
dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);  //����dc

2.   memDC.CreateCompatibleDC(&dc);  //�����뵱ǰDC���ݵ��ڴ�DC

3.   memBitmap.CreateCompatibleBitmap(&dc, rect.Width(),rect.Height());  //����һ��ָ����С��λͼ

4.   oldmemBitmap = memDC.SelectObject(&memBitmap); // ����λͼѡ�뵽�ڴ�DC��   memDC

5.   memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc,left, top, SRCCOPY); // ��ͼ��dc������һ��Ŀ��memDC��

������ BMP�ļ���ȡ��BMPͼ���ļ����ֳ�4�����֣�λͼ�ļ�ͷ��Bitmap File Header����λͼ��Ϣͷ��Bitmap Info Header������ɫ��Color Map����λͼ���ݣ���ͼ�����ݣ�Data Bits��Data Body������
*/


void bmpScreen::screenShot(CRect rect, int left, int top, char *name)

{
	CBitmap*  m_pBitmap;                                                     // �������Ա  
																			 //CFrameWnd* pMainFrame = (CFrameWnd*)AfxGetMainWnd();                     // ��ý�ͼ���ڵ�ָ�룬Ĭ��Ϊ�����ڣ����Ը���Ϊ�����Ĵ��ڡ�  
																			 //   CPaintDC   dc(pMainFrame);   

																			 //Ϊ��Ļ�����豸������  
	CDC dc;
	dc.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);


	m_pBitmap = new   CBitmap;
	m_pBitmap->CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());




	CDC   memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBitmap, *oldmemBitmap;                                        // ��������Ļ���ݵ�bitmap  

	memBitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());

	oldmemBitmap = memDC.SelectObject(&memBitmap);//��memBitmapѡ���ڴ�DC  
	memDC.BitBlt(0, 0, rect.Width(), rect.Height(), &dc, rect.left, rect.top, SRCCOPY);  //P:rect.left, rect.topԭλͼ�����Ļ������  
																						 //memDC.BitBlt(0, 0, rect.Width(),rect.Height(), &dc,0, 0, SRCCOPY);    
	BITMAP bmp;
	memBitmap.GetBitmap(&bmp);                                               // ���λͼ��Ϣ   ///P:�õ�������λͼmemBitmap����Ϣ��bmp��  

	FILE *fp = fopen(name, "wb+");
	
	if (fp == NULL)
	{
		AfxMessageBox(L"Open Failed");
	}

	BITMAPINFOHEADER bih = { 0 };                                              // λͼ��Ϣͷ  
	bih.biBitCount = bmp.bmBitsPixel;                                        // ÿ�������ֽڴ�С  
	bih.biCompression = BI_RGB;
	bih.biHeight = bmp.bmHeight;                                             // �߶�  
	bih.biPlanes = 1;
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biSizeImage = bmp.bmWidthBytes * bmp.bmHeight;                       // ͼ�����ݴ�С  
	bih.biWidth = bmp.bmWidth;                                               // ���  

	BITMAPFILEHEADER bfh = { 0 };                                              // λͼ�ļ�ͷ  
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);     // ��λͼ���ݵ�ƫ����  
	bfh.bfSize = bfh.bfOffBits + bmp.bmWidthBytes * bmp.bmHeight;            // �ļ��ܵĴ�С  
	bfh.bfType = (WORD)0x4d42;

	fwrite(&bfh, 1, sizeof(BITMAPFILEHEADER), fp);                           //д��λͼ�ļ�ͷ  

	fwrite(&bih, 1, sizeof(BITMAPINFOHEADER), fp);                           //д��λͼ��Ϣͷ  

	byte * p = new byte[bmp.bmWidthBytes * bmp.bmHeight];                    //�����ڴ汣��λͼ����  

	GetDIBits(memDC.m_hDC, (HBITMAP)memBitmap.m_hObject, 0, rect.Height(), p,
		(LPBITMAPINFO)&bih, DIB_RGB_COLORS);                                    //��ȡλͼ����  

	fwrite(p, 1, bmp.bmWidthBytes * bmp.bmHeight, fp);                       //д��λͼ����  
	delete[] p;
	if (fp!=NULL)
		fclose(fp);
	memDC.SelectObject(oldmemBitmap);
	memDC.DeleteDC();

}