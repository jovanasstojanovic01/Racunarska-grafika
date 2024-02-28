
// RGLAb1View.h : interface of the CRGLAb1View class
//

#pragma once


class CRGLAb1View : public CView
{
protected: // create from serialization only
	CRGLAb1View() noexcept;
	DECLARE_DYNCREATE(CRGLAb1View);
	
// Attributes
public:
	CRGLAb1Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	void DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle);
	//afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	void WritePixel(CDC* pDC, int x, int y, COLORREF value);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRGLAb1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool drawGrid;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in RGLAb1View.cpp
inline CRGLAb1Doc* CRGLAb1View::GetDocument() const
   { return reinterpret_cast<CRGLAb1Doc*>(m_pDocument); }
#endif

