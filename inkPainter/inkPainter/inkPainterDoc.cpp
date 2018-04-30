
// inkPainterDoc.cpp : implementation of the CinkPainterDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "inkPainter.h"
#endif
#include "inkPainterDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CView* g_pView;
// CinkPainterDoc

IMPLEMENT_DYNCREATE(CinkPainterDoc, CDocument)

BEGIN_MESSAGE_MAP(CinkPainterDoc, CDocument)
END_MESSAGE_MAP()


// CinkPainterDoc construction/destruction

CinkPainterDoc::CinkPainterDoc()
	:  m_bImageChanged(FALSE)
	, m_ImageWidth(1024)
	, m_ImageHeight(1024)
{
	// TODO: add one-time construction code here

}

CinkPainterDoc::~CinkPainterDoc()
{
}

BOOL CinkPainterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	//AfxMessageBox(L"Open ");
	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_bImageChanged = true;
	memset(m_pImage, 0, 1024 * 1024 * 3);       //3???
	//g_pView->Invalidate(0);
	g_pView->InvalidateRect(CRect(0, 0, 1, 1));
	return TRUE;
}




// CinkPainterDoc serialization

void CinkPainterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CinkPainterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CinkPainterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CinkPainterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CinkPainterDoc diagnostics

#ifdef _DEBUG
void CinkPainterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CinkPainterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CinkPainterDoc commands


// 打开文件
BOOL CinkPainterDoc::nOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	m_bImageChanged = false;
	m_TextureImage = auxDIBImageLoad(lpszPathName);
		memcpy(m_pImage,m_TextureImage->data,m_TextureImage->sizeX*m_TextureImage->sizeY*3);
	for (int i = 0; i<m_TextureImage->sizeX*m_TextureImage->sizeY * 3; i++)          //纹理  *3
	{
		m_pImage[i] = 255 - m_TextureImage->data[i];    //255
	}
	m_ImageWidth = m_TextureImage->sizeX;
	m_ImageHeight = m_TextureImage->sizeY;
	//g_pView->Invalidate(0);
	g_pView->InvalidateRect(CRect(0,0,1,1));

	return TRUE;
}
