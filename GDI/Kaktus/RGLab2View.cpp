#include "pch.h"
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "RGLab2.h"
#endif
#include "RGLab2Doc.h"
#include "RGLab2View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE(CRGLab2View, CView)

BEGIN_MESSAGE_MAP(CRGLab2View, CView)
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

CRGLab2View::CRGLab2View() noexcept{
	this->chart = false;
	this->tmp = 25;
	this->globalAngle = 0;
	this->localAngle = 3.14/4*3;
}

CRGLab2View::~CRGLab2View(){}

BOOL CRGLab2View::PreCreateWindow(CREATESTRUCT& cs){
	return CView::PreCreateWindow(cs);
}

void Translate(CDC* pDC, float dX, float dY, bool rightMultiply){
	XFORM Xform;

	Xform.eDx = dX;
	Xform.eDy = dY;
	Xform.eM11 = (FLOAT)1.0;
	Xform.eM12 = (FLOAT)0.0;
	Xform.eM21 = (FLOAT)0.0;
	Xform.eM22 = (FLOAT)1.0;

	if(rightMultiply)
		pDC->ModifyWorldTransform(&Xform,MWT_RIGHTMULTIPLY);
	else
		pDC->ModifyWorldTransform(&Xform, MWT_LEFTMULTIPLY);
}

void Scale(CDC* pDC, float sX, float sY, bool rightMultiply){
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

void Rotate(CDC* pDC, float angle, bool rightMultiply){
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

void CRGLab2View::DrawSeparate(CDC* pDC){
	HENHMETAFILE svetliKaktus;
	XFORM form;
	
	svetliKaktus = GetEnhMetaFile(CString("../Metafiles/cactus_part_light.emf"));

	Rotate(pDC, this->localAngle , false);
	Translate(pDC, 0, -0.5 * this->tmp, false);
	//Scale(pDC, 1, 1, false);
	
	pDC->PlayMetaFile(svetliKaktus, CRect(-1.2 * this->tmp, -2.5 * this->tmp, 1.2 * this->tmp, 0.5 * this->tmp));

	DeleteEnhMetaFile(svetliKaktus);
}

void CRGLab2View::DrawRotatedElements(CDC* pDC, float angle, float scale/*, float gore, float levo*/) {
	HENHMETAFILE tamniKaktus;
	tamniKaktus = GetEnhMetaFile(CString("../Metafiles/cactus_part.emf"));
	
	Rotate(pDC, angle, false);
	//Translate(pDC, levo, gore, false);
	Scale(pDC, scale, 1, false);

	pDC->PlayMetaFile(tamniKaktus, CRect(-1.2 * this->tmp, -2.5 * this->tmp, 1.2 * this->tmp, 0.5 * this->tmp));

	DeleteEnhMetaFile(tamniKaktus);
}

void CRGLab2View::DrawFigure(CDC* pDC){
	HENHMETAFILE tamniKaktus, svetliKaktus;
	CPoint org;
	XFORM XoldForm;
	BOOL b;

	CPen crna(PS_SOLID, 1, RGB(0, 0, 0));
	CBrush zelenaCetka(RGB(0, 204, 43));
	CRect rect(-1.2 * this->tmp, 2.5 * this->tmp, 1.2 * this->tmp, -0.4 * this->tmp);

	tamniKaktus = GetEnhMetaFile(CString("../Metafiles/cactus_part.emf"));
	svetliKaktus = GetEnhMetaFile(CString("../Metafiles/cactus_part_light.emf"));
	
	org=pDC->SetWindowOrg(-10 * this->tmp, -3.5 * this->tmp);
	Rotate(pDC, this->globalAngle, false);
	pDC->PlayMetaFile(svetliKaktus, rect);
	
	b = pDC->GetWorldTransform(&XoldForm);
	Scale(pDC, 0.4, 1, false);
	Translate(pDC, 0, 2.9 * this->tmp, false);
	pDC->PlayMetaFile(tamniKaktus, rect);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, 0.25 * this->tmp, 2.75 * this->tmp, false);
	DrawRotatedElements(pDC, 3.14 / 4 * 3, 0.4);// , 0, -0.8);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, -0.25 * this->tmp, 2.75 * this->tmp, false);
	DrawRotatedElements(pDC, -3.14 / 4*3, 0.4);//, 0, 0.8);
	
	pDC->SetWorldTransform(&XoldForm);
	Scale(pDC, 0.7, 1, false);
	Translate(pDC, -2.9 * this->tmp, 5 * this->tmp, false);
	pDC->PlayMetaFile(tamniKaktus, rect);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, -2.5 * this->tmp, 4.5 * this->tmp, false);
	DrawRotatedElements(pDC, 3.14 / 2*3, 0.7);// 0.5, 0);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, 0.25 * this->tmp, 6 * this->tmp, false);
	DrawRotatedElements(pDC, 3.14 / 4 * 3, 0.7);//, 0.5, -1);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, -0.25 * this->tmp, 6 * this->tmp, false);
	DrawRotatedElements(pDC, -3.14 / 4 * 3, 0.7);//, 0.5, -1);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, -2.4 * this->tmp, 8.25 * this->tmp, false);
	DrawRotatedElements(pDC, -3.14 / 4 * 3, 1);//, 0.5, -1);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, 2.4 * this->tmp, 8.25 * this->tmp, false);
	DrawRotatedElements(pDC, 3.14 / 4 * 3, 1);//, 0.5, -1);
		
	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, 2 * this->tmp, 4.6 * this->tmp, false);

	DrawSeparate(pDC);

	pDC->SetWorldTransform(&XoldForm);
	pDC->SelectObject(&crna);
	pDC->SelectObject(&zelenaCetka);
	pDC->Ellipse(-0.5 * this->tmp, 3 * this->tmp, 0.5 * this->tmp, 2 * this->tmp);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, -2 * this->tmp, 2 * this->tmp, false);
	pDC->Ellipse(-0.5 * this->tmp, 3 * this->tmp, 0.5 * this->tmp, 2 * this->tmp);
	
	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, 2 * this->tmp, 2 * this->tmp, false);
	pDC->Ellipse(-0.5 * this->tmp, 3 * this->tmp, 0.5 * this->tmp, 2 * this->tmp);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, -2 * this->tmp, 5.3 * this->tmp, false);
	pDC->Ellipse(-0.5 * this->tmp, 3 * this->tmp, 0.5 * this->tmp, 2 * this->tmp);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, 0 * this->tmp, 3 * this->tmp, false);
	pDC->Ellipse(-0.5 * this->tmp, 3 * this->tmp, 0.5 * this->tmp, 2 * this->tmp);

	pDC->SetWorldTransform(&XoldForm);
	Translate(pDC, 2 * this->tmp, 5.3 * this->tmp, false);
	pDC->Ellipse(-0.5 * this->tmp, 3 * this->tmp, 0.5 * this->tmp, 2 * this->tmp);


	DeleteEnhMetaFile(tamniKaktus);
	DeleteEnhMetaFile(svetliKaktus);
	pDC->SetWindowOrg(org);
}

void CRGLab2View::OnDraw(CDC* pDC){
	CRGLab2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	
	CPen* oldPen;
	CPen bela(PS_SOLID, 1, RGB(255, 255, 255));
	CPen crna(PS_SOLID, 1, RGB(0, 0, 0));

	CBrush* oldBrush;
	CBrush plavaCetka(RGB(135, 206, 233));
	CBrush zelenaCetka(RGB(0, 204, 43));
	CBrush orandzastaCetka(RGB(222, 148, 38));

	XFORM XoldForm, Xform;
	DWORD dw;
	BOOL b;
	COLORREF fntClrOld;
	CString msg;
	CFont font, * oldFont;
	LOGFONT lf;
	int oldMode;
	int prevMode;

	POINT pts[8] = {
		CPoint(8.5 * this->tmp, 0 * this->tmp),
		CPoint(11.5 * this->tmp,0 * this->tmp),
		CPoint(12 * this->tmp,2 * this->tmp),
		CPoint(12.5 * this->tmp,2 * this->tmp),
		CPoint(12.5 * this->tmp,2.8 * this->tmp),
		CPoint(7.5 * this->tmp,2.8 * this->tmp),
		CPoint(7.5 * this->tmp,2 * this->tmp),
		CPoint(8 * this->tmp,2 * this->tmp) };

	oldPen = pDC->SelectObject(&bela);
	oldBrush = pDC->SelectObject(&plavaCetka);
	pDC->Rectangle(0, 0, 500, 500);
	
	
	prevMode = pDC->SetGraphicsMode(GM_ADVANCED);
	dw = GetLastError();
	b = pDC->GetWorldTransform(&XoldForm);
	Xform.eDx = (FLOAT)0.0;
	Xform.eDy = (FLOAT)0.0;
	Xform.eM11 = (FLOAT)1.0;
	Xform.eM12 = (FLOAT)0.0;
	Xform.eM21 = (FLOAT)0.0;
	Xform.eM22 = (FLOAT)1.0;
	pDC->SetWorldTransform(&Xform);

	DrawFigure(pDC);
	b = pDC->SetWorldTransform(&XoldForm);
	pDC->SetGraphicsMode(prevMode);

	pDC->SelectObject(&crna);
	pDC->SelectObject(&zelenaCetka);
	pDC->Ellipse(9.5 * this->tmp, 2.5 * this->tmp, 10.5 * this->tmp, 3.5 * this->tmp);
	
	pDC->SelectObject(&orandzastaCetka);
	pDC->Polygon(pts, 8);

	fntClrOld = pDC->SetTextColor(RGB(245, 245, 63));
	oldMode = pDC->SetBkMode(TRANSPARENT);
	msg = L"18417 Jovana Stojanovic";
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = this->tmp;
	lf.lfEscapement = 1800;
	wcscpy_s(lf.lfFaceName, 32, L"Arial");
	font.CreateFontIndirect(&lf);
	oldFont = pDC->SelectObject(&font);
	pDC->TextOut(19 * this->tmp,19* this->tmp, msg);
	
	pDC->SelectObject(&bela);
	if(this->chart)
		for (int i = this->tmp; i <= 500; i += this->tmp)
		{
			pDC->MoveTo(0, i);
			pDC->LineTo(500, i);
			pDC->MoveTo(i, 0);
			pDC->LineTo(i, 500);
		}
	
	pDC->SelectObject(oldFont);
	pDC->SetTextColor(fntClrOld);
	pDC->SelectObject(&oldBrush);
	pDC->SelectObject(oldPen);
}

BOOL CRGLab2View::OnPreparePrinting(CPrintInfo* pInfo){
	return DoPreparePrinting(pInfo);
}

void CRGLab2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
}

void CRGLab2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/){
}

#ifdef _DEBUG
void CRGLab2View::AssertValid() const{
	CView::AssertValid();
}

void CRGLab2View::Dump(CDumpContext& dc) const{
	CView::Dump(dc);
}

CRGLab2Doc* CRGLab2View::GetDocument() const {
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRGLab2Doc)));
	return (CRGLab2Doc*)m_pDocument;
}
#endif //_DEBUG

void CRGLab2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){
	if (nChar == 65){
		this->chart = !this->chart;
		Invalidate();
	}
	if (nChar == 66){
		this->globalAngle -= 3.14/6;
		Invalidate();
	}
	if (nChar == 67){
		this->globalAngle += 3.14 / 6;
		Invalidate();
	}
	if (nChar == 68){
		this->localAngle -= 3.14 / 6;
		Invalidate();
	}
	if (nChar == 69){
		this->localAngle += 3.14 / 6;
		Invalidate();
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
