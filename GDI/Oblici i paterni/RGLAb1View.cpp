
// RGLAb1View.cpp : implementation of the CRGLAb1View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RGLAb1.h"
#include "math.h"
#endif

#include "RGLAb1Doc.h"
#include "RGLAb1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRGLAb1View

IMPLEMENT_DYNCREATE(CRGLAb1View, CView)

BEGIN_MESSAGE_MAP(CRGLAb1View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CRGLAb1View construction/destruction

CRGLAb1View::CRGLAb1View() noexcept
{
	// TODO: add construction code here
	
	this->drawGrid = false;

}

CRGLAb1View::~CRGLAb1View()
{
}

BOOL CRGLAb1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGLAb1View drawing

void CRGLAb1View::WritePixel(CDC* pDC, int x, int y, COLORREF value)
{
	pDC->SetPixel(x, y, value);
}


void CRGLAb1View::OnDraw(CDC* pDC)//pDC je prikazni kontekst
{
	CRGLAb1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CPen backgroundPen(PS_SOLID, 0, RGB(200, 200, 200));
	CPen* oldPen = pDC->SelectObject(&backgroundPen);
	CBrush backgroundBrush(RGB(200, 200, 200));
	CBrush* oldBrush = pDC->SelectObject(&backgroundBrush);
	pDC->Rectangle(0, 0, 500, 500);

	CPen* shapesPen=new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 6, RGB(0, 0, 255));
	pDC->SelectObject(shapesPen);

	CBrush roze(RGB(255, 153, 204));
	CBrush zuta(RGB(255, 255, 0));
	CBrush narandzasta(RGB(255, 153, 51));
	CBrush ljubicasta(RGB(153, 0, 204));
	CBrush zelena(RGB(0, 255, 0));
	CBrush crvena(RGB(255, 0, 0));
	CBrush srafura(HS_DIAGCROSS, RGB(139, 177, 255));

	pDC->SelectObject(&roze);
	POINT rozeTrougaoPTS[3] = { CPoint(25,25),CPoint(475,25),CPoint(250,250) };
	pDC->Polygon(rozeTrougaoPTS, 3);

	pDC->SelectObject(&zuta);
	POINT zutTougaoPTS[3]={ CPoint(25,25),CPoint(137.5,137.5),CPoint(25,250) };
	pDC->Polygon(zutTougaoPTS, 3);

	pDC->SelectObject(&narandzasta);
	POINT narandzastKvadratPTS[4] = { CPoint(137.5,362.5),CPoint(362.5,362.5),CPoint(475,475),CPoint(250,475) };
	pDC->Polygon(narandzastKvadratPTS, 4);

	pDC->SelectObject(&ljubicasta);
	POINT ljubicastTrougaoPTS[3] = { CPoint(475,25),CPoint(475,475),CPoint(250,250) };
	pDC->Polygon(ljubicastTrougaoPTS, 3);

	pDC->SelectObject(&zelena);
	POINT zeleniTrougaoPTS[3] = { CPoint(250,250),CPoint(362.5,362.5),CPoint(137.5,362.5) };
	pDC->Polygon(zeleniTrougaoPTS, 3);

	pDC->SelectObject(&crvena);
	POINT crveniParalelogramPTS[4] = { CPoint(137.5,137.5),CPoint(250,250),CPoint(137.5,362.5),CPoint(25,250) };
	pDC->Polygon(crveniParalelogramPTS, 4);

	pDC->SelectObject(&srafura);
	POINT srafiraniTrougaoPTS[3] = { CPoint(25,250),CPoint(250,475),CPoint(25,475) };
	pDC->Polygon(srafiraniTrougaoPTS, 3);

	
	DrawRegularPolygon(pDC, 62.5, 137.5, 25, 7, 0);
	DrawRegularPolygon(pDC, 250, 112.5, 50, 5, 0);
	DrawRegularPolygon(pDC, 250, 312.5, 25, 8, 0);
	DrawRegularPolygon(pDC, 375,250, 50, 4, 0);
	DrawRegularPolygon(pDC, 87.5, 414.5, 37.5, 6, 0);

	if (this->drawGrid)
	{
		CPen olovka(PS_SOLID, 0, RGB(255, 255, 255));
		pDC->SelectObject(&olovka);
		for (int i = -1; i < 500; i += 25)
		{
			pDC->MoveTo(i, 0);
			pDC->LineTo(i, 500);
			pDC->MoveTo(0, i);
			pDC->LineTo(500, i);
		}
	}
	
	

	pDC->SelectObject(oldPen);
	pDC->SelectObject(oldBrush);
	// TODO: add draw code for native data here
}



void CRGLAb1View::DrawRegularPolygon(CDC* pDC, int cx, int cy, int r, int n, float rotAngle)
{
	CGdiObject* cetka = pDC->SelectStockObject(NULL_BRUSH);
	double ugaoNtougla = 2*3.1415 / (double)n;
	double pocetniUgao = rotAngle;
	CPoint* pts = new CPoint[n];
	for (int i = 0; i < n; i++)
	{
		CPoint tmp((int)(r * cos(pocetniUgao) + cx),
			(int)(r * sin(pocetniUgao) + cy));
		pts[i] = tmp;
		pocetniUgao += ugaoNtougla;
	}

	CPen* figuraPen = new CPen(PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_FLAT | PS_JOIN_MITER, 3, RGB(0, 0, 255));
	CPen* oldPen = pDC->SelectObject(figuraPen);
	
	pDC->Polygon(pts, n);

	pDC->SelectObject(oldPen);
}




// CRGLAb1View printing

BOOL CRGLAb1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGLAb1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGLAb1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGLAb1View diagnostics

#ifdef _DEBUG
void CRGLAb1View::AssertValid() const
{
	CView::AssertValid();
}

void CRGLAb1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGLAb1Doc* CRGLAb1View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGLAb1Doc)));
	return (CRGLAb1Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGLAb1View message handlers


void CRGLAb1View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == 65) {
		
		this->drawGrid = !this->drawGrid;
		Invalidate();	
	}


	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
