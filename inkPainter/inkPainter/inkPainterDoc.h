
// inkPainterDoc.h : interface of the CinkPainterDoc class
//


#pragma once
#include <gl\glaux.h>


class CinkPainterDoc : public CDocument
{
protected:
	void SetTitle(LPCTSTR lpszTitle);
	// create from serialization only
	CinkPainterDoc();
	DECLARE_DYNCREATE(CinkPainterDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CinkPainterDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:

	// 是否改变了图像 是则为1 否则为0
	BOOL m_bImageChanged;
};
