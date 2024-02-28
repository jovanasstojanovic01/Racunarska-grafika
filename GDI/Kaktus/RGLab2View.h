
// RGLab2View.h : interface of the CRGLab2View class
//

#pragma once


class CRGLab2View : public CView
{
protected: // create from serialization only
	CRGLab2View() noexcept;
	DECLARE_DYNCREATE(CRGLab2View)

// Attributes
public:
	
	CRGLab2Doc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CRGLab2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void DrawRotatedElements(CDC* pDC, float angle, float scale/*, float gore, float levo*/);
	void DrawSeparate(CDC* pDC);
	double tmp;
	double globalAngle;
	double localAngle;
	bool chart;
	void DrawFigure(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in RGLab2View.cpp
inline CRGLab2Doc* CRGLab2View::GetDocument() const
   { return reinterpret_cast<CRGLab2Doc*>(m_pDocument); }
#endif

