
// RGLab3View.cpp : implementation of the CRGLab3View class
//
#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "RGLab3.h"
#endif

#include "RGLab3Doc.h"
#include "RGLab3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "DImage.h"


// CRGLab3View

IMPLEMENT_DYNCREATE(CRGLab3View, CView)

BEGIN_MESSAGE_MAP(CRGLab3View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CRGLab3View construction/destruction

CRGLab3View::CRGLab3View() noexcept
{
	// TODO: add construction code here
	this->tmp = 25;
}

CRGLab3View::~CRGLab3View()
{
}

BOOL CRGLab3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CRGLab3View drawing

void CRGLab3View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply) {
	XFORM Xform;

	Xform.eDx = dX;
	Xform.eDy = dY;
	Xform.eM11 = (FLOAT)1.0;
	Xform.eM12 = (FLOAT)0.0;
	Xform.eM21 = (FLOAT)0.0;
	Xform.eM22 = (FLOAT)1.0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&Xform, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}

void CRGLab3View::Scale(CDC* pDC, float sX, float sY, bool rightMultiply) {
	XFORM Xform;

	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;
	Xform.eM11 = sX;
	Xform.eM12 = (FLOAT)0.0;
	Xform.eM21 = (FLOAT)0.0;
	Xform.eM22 = sY;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&Xform, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}

void CRGLab3View::Rotate(CDC* pDC, float angle, bool rightMultiply) {
	XFORM Xform;

	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;
	Xform.eM11 = cos(angle);
	Xform.eM12 = sin(angle);
	Xform.eM21 = -sin(angle);
	Xform.eM22 = cos(angle);

	if (rightMultiply)
		pDC->ModifyWorldTransform(&Xform, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}

void CRGLab3View::Mirror(CDC* pDC, bool mx, bool my, bool rightMultiply) {
	XFORM Xform;

	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;
	Xform.eM11 = mx ? (FLOAT)-1.0 : (FLOAT)1.0;
	Xform.eM12 = (FLOAT)0.0;
	Xform.eM21 = (FLOAT)0.0;
	Xform.eM22 = my ? (FLOAT)-1.0 : (FLOAT)1.0;

	if (rightMultiply)
		pDC->ModifyWorldTransform(&Xform, MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}

void CRGLab3View::BkTransparent(CDC* pDC, DImage *img, bool blue)
{
	int w = img->Width();
	int h = img->Height();
	CBitmap bmpImage;
	CBitmap bmpMask;
	bmpImage.CreateCompatibleBitmap(pDC, w, h);


	bmpMask.CreateBitmap(w, h, 1, 1, NULL);
	CDC* pSrcDC = new CDC();
	pSrcDC->CreateCompatibleDC(pDC);
	CDC* pMaskDC = new CDC();
	pMaskDC->CreateCompatibleDC(pDC);
	CBitmap* pOldSrcBmp=pSrcDC->SelectObject(&bmpImage);
	CBitmap* pOldMaskBmp=pMaskDC->SelectObject(&bmpMask);

	CBitmap *bmpColor = img->GetBitmap();

	if (blue)
	{
		ToBlue(bmpColor);
	}
	else
	{
		ToGrey(bmpColor);
	}
	DImage newImg(*bmpColor);

	newImg.Draw(pSrcDC, CRect(0, 0, w, h), CRect(0, 0, w, h));

	COLORREF clrTopLeft = pSrcDC->GetPixel(0, 0);
	COLORREF clrSaveBk = pSrcDC->SetBkColor(clrTopLeft);
	pMaskDC->BitBlt(0, 0, w, h, pSrcDC, 0, 0, SRCCOPY);
	COLORREF clrSaveDstText = pSrcDC->SetTextColor(RGB(255, 255, 255));
	pSrcDC->SetBkColor(RGB(0, 0, 0));
	pSrcDC->BitBlt(0, 0, w, h, pMaskDC, 0, 0, SRCAND);
	
	pDC->BitBlt(0, 0, w, h, pMaskDC, 0, 0, SRCAND);
	pDC->BitBlt(0, 0, w, h, pSrcDC, 0, 0, SRCPAINT);

	pMaskDC->SetTextColor(clrSaveDstText);
	pSrcDC->SetBkColor(clrSaveBk);
	pSrcDC->SelectObject(pOldSrcBmp);
	pMaskDC->SelectObject(pOldMaskBmp);
	bmpImage.DeleteObject();
	bmpMask.DeleteObject();
	pMaskDC->DeleteDC();
	pSrcDC->DeleteDC();
	
	delete pSrcDC;
	delete pMaskDC;
}

void CRGLab3View::DrawPuzzlePart(CDC* pDC, DImage* img, float x, float y, bool mx, bool my, float angle, bool blue)
{
	Translate(pDC, x, y, false);
	Mirror(pDC, mx, my, false);
	Rotate(pDC, angle, false);
	BkTransparent(pDC, img, blue);
}

void CRGLab3View::ToGrey(CBitmap* bm)
{
	BITMAP bmp;
	bm->GetBitmap(&bmp);
	int brBajtova = bmp.bmBitsPixel / 8;
	DWORD dwCount = bmp.bmWidth * bmp.bmHeight * brBajtova;

	BYTE* lpBits = new BYTE[dwCount];

	bm->GetBitmapBits(dwCount, lpBits);

	for (int i = 0; i < bmp.bmHeight; i++)
	{
		for (int j = 0; j < bmp.bmWidth; j++)
		{
			BYTE blue = lpBits[(i * bmp.bmWidth + j) * brBajtova];
			BYTE green = lpBits[(i * bmp.bmWidth + j) * brBajtova + 1];
			BYTE red = lpBits[(i * bmp.bmWidth + j) * brBajtova + 2];
			if (!(blue == lpBits[0] && green == lpBits[1] && red == lpBits[2]))
			{
				float B = (FLOAT)blue;
				float G = (FLOAT)green;
				float R = (FLOAT)red;
				int gr = 64 + (R + G + B) / 3;
				if (gr > 255)gr = 255;
				lpBits[(i * bmp.bmWidth + j) * brBajtova] = (char)gr;
				lpBits[(i * bmp.bmWidth + j) * brBajtova + 1] = (char)gr;
				lpBits[(i * bmp.bmWidth + j) * brBajtova + 2] = (char)gr;
			}
		}
	}

	bm->SetBitmapBits(dwCount, lpBits);

	delete[] lpBits;
}

void CRGLab3View::ToBlue(CBitmap* bm)
{
	BITMAP bmp;
	bm->GetBitmap(&bmp);
	int brBajtova = bmp.bmBitsPixel / 8;
	DWORD dwCount = bmp.bmWidth * bmp.bmHeight* brBajtova;

	BYTE* lpBits = new BYTE[dwCount];

	bm->GetBitmapBits(dwCount, lpBits);

	for (int i = 0; i < bmp.bmHeight; i++)
	{
		for (int j = 0; j < bmp.bmWidth; j++)
		{
			BYTE blue = lpBits[(i * bmp.bmWidth + j) * brBajtova];
			BYTE green = lpBits[(i * bmp.bmWidth + j) * brBajtova+1];
			BYTE red = lpBits[(i * bmp.bmWidth + j) * brBajtova+2];
			if (!(blue == lpBits[0] && green == lpBits[1] && red == lpBits[2]))
			{
				lpBits[(i * bmp.bmWidth + j) * brBajtova] += 50;
				lpBits[(i * bmp.bmWidth + j) * brBajtova + 1] = 0;
				lpBits[(i * bmp.bmWidth + j) * brBajtova + 2] = 0;
			}
		}
	}

	bm->SetBitmapBits(dwCount, lpBits);

	delete[] lpBits;
}

void CRGLab3View::DrawPuzzle(CDC* pDC)
{
	CPen* oldPen;
	CPen grey(PS_SOLID, 1, RGB(218, 218, 218));
	oldPen = pDC->SelectObject(&grey);

	for (int i = this->tmp; i <= 500; i += this->tmp)
	{
		pDC->MoveTo(0, i);
		pDC->LineTo(500, i);
		pDC->MoveTo(i, 0);
		pDC->LineTo(i, 500);
	}
	pDC->SelectObject(oldPen);

	DImage img1, img2, img3, img4, img5, img6, img7, img8, img9;
	img1.Load(CString("./res/1.bmp"));
	img2.Load(CString("./res/2.bmp"));
	img3.Load(CString("./res/3.bmp"));
	img4.Load(CString("./res/4.bmp"));
	img5.Load(CString("./res/5.bmp"));
	img6.Load(CString("./res/6.bmp"));
	img7.Load(CString("./res/7.bmp"));
	img8.Load(CString("./res/8.bmp"));
	img9.Load(CString("./res/9.bmp"));

	int prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	DWORD dw = GetLastError();

	BOOL b2;
	XFORM XoldForm2;
	b2 = pDC->GetWorldTransform(&XoldForm2);
	BOOL b;
	XFORM XoldForm;
	b = pDC->GetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 13 * this->tmp, false);
	DrawPuzzlePart(pDC, &img8,10.2*this->tmp,0, false, true, 3.14-0.25, false);

	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 13 * this->tmp, false);
	DrawPuzzlePart(pDC, &img7, 16.5*this->tmp,0.63*this->tmp, false, true, 3.14-0.345, false);
	
	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 7 * this->tmp, false);
	DrawPuzzlePart(pDC, &img6, 21.55*this->tmp,-0.75*this->tmp, false, true, -3.14-0.125, false);
	
	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 7 * this->tmp, false);
	DrawPuzzlePart(pDC, &img1, 11.355*this->tmp,9.888*this->tmp, false, true, 3.14/2+0.768, false);
	
	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 7 * this->tmp, false);
	DrawPuzzlePart(pDC, &img4, 8.25*this->tmp,17.25*this->tmp, false, true, 3.14/6-0.01, false);
	
	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 7 * this->tmp, false);
	DrawPuzzlePart(pDC, &img9, 23.1*this->tmp,8.3*this->tmp, false, true, 3.14/2+3.14/3-0.065, false);
	
	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 7 * this->tmp, false);
	DrawPuzzlePart(pDC, &img2, 6.65*this->tmp,22.95*this->tmp, false, true, 3.14/3+0.07, false);
	
	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 7 * this->tmp, false);
	DrawPuzzlePart(pDC, &img5, 11.15*this->tmp, 8.75*this->tmp, false, true, -3.14/2-3.14/3+0.058, true);
	
	pDC->SetWorldTransform(&XoldForm);
	//Translate(pDC, 7 * this->tmp, 7 * this->tmp, false);
	DrawPuzzlePart(pDC, &img3, 12.6*this->tmp, 9.39*this->tmp, false, true, -3.14/2-0.36, false);

	pDC->SetWorldTransform(&XoldForm2);
	pDC->SetGraphicsMode(prevMode);
	
}


void CRGLab3View::OnDraw(CDC* pDC)
{
	CRGLab3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	int dc = pDC->SaveDC();

	CRect clientWindow;
	GetClientRect(&clientWindow);
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, clientWindow.Width(), clientWindow.Height());
	MemDC.SelectObject(&bmp);
	MemDC.BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), pDC, 0, 0, WHITENESS);
	
	DrawPuzzle(&MemDC);

	pDC->BitBlt(0, 0, clientWindow.Width(), clientWindow.Height(), &MemDC, 0, 0, SRCCOPY);
	MemDC.DeleteDC();
	delete MemDC;

	pDC->RestoreDC(dc);
}


// CRGLab3View printing

BOOL CRGLab3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRGLab3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRGLab3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CRGLab3View diagnostics

#ifdef _DEBUG
void CRGLab3View::AssertValid() const
{
	CView::AssertValid();
}

void CRGLab3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CRGLab3Doc* CRGLab3View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGLab3Doc)));
	return (CRGLab3Doc*)m_pDocument;
}
#endif //_DEBUG


// CRGLab3View message handlers


void CRGLab3View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	if (nChar == 65)
	{
		Invalidate(FALSE);
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CRGLab3View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
}
