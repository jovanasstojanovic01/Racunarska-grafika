
// RGLab3View.h : interface of the CRGLab3View class
//
#pragma once
#include "DImage.h"


class CRGLab3View : public CView
{
protected: // create from serialization only
	CRGLab3View() noexcept;
	DECLARE_DYNCREATE(CRGLab3View)

// Attributes
public:
	CRGLab3Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	void CopyDC(CDC* src, CDC* dst);
	void DrawPuzzle(CDC* pDC);
	void BkTransparent(CDC* pDC, DImage *img, bool blue);
	void DrawPuzzlePart(CDC* pDC, DImage* img, float x, float y, bool mx, bool my, float angle, bool blue);
	void ToGrey(CBitmap* bm);
	void ToBlue(CBitmap* bm);
	void Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void Scale(CDC* pDC, float sX, float sY, bool rightMultiply);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	double tmp;
	virtual ~CRGLab3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in RGLab3View.cpp
inline CRGLab3Doc* CRGLab3View::GetDocument() const
   { return reinterpret_cast<CRGLab3Doc*>(m_pDocument); }
#endif

