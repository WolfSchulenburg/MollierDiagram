#include "stdafx.h"
#include "Context.h"

CContext::CContext(CView* pView, CDC* pDC)
{
	this->pDC = pDC;
	pView->GetClientRect(clientRect);
	
	/*
	// set graphics mode and viewport
	this->pDC->SetMapMode(MM_ISOTROPIC);
	//this->pDC->SetWindowExt(AREA_WIDTH, AREA_HEIGHT);
	// pDC->SetViewportExt(pDC->GetDeviceCaps(LOGPIXELSX),-pDC->GetDeviceCaps(LOGPIXELSY));
	this->pDC->SetViewportExt(clientRect.Width(), clientRect.Height());
	*/
	
	listPens.Add(new CPen(PS_SOLID, 2, RGB(128, 128, 128)));
	listPens.Add(new CPen(PS_SOLID, 1, RGB(128, 128, 128)));
	
	listPens.Add(new CPen(PS_SOLID, 1, RGB(0, 0, 255)));
	listPens.Add(new CPen(PS_SOLID, 2, RGB(0, 0, 255)));
	listPens.Add(new CPen(PS_SOLID, 4, RGB(0, 0, 255)));

	listPens.Add(new CPen(PS_SOLID, 1, RGB(0, 255, 0)));
	listPens.Add(new CPen(PS_SOLID, 2, RGB(0, 255, 0)));

	listPens.Add(new CPen(PS_SOLID, 2, RGB(255, 0, 0)));
	
	pen.CreatePen(PS_SOLID, 2, RGB(128, 128, 128));
	pPenOld = pDC->SelectObject(&pen);
}

CContext::~CContext()
{
	int i = 0;
	while (i < listPens.GetSize())
	{
		delete listPens.GetAt(i++);
	}

	listPens.RemoveAll();

	pDC->SelectObject(pPenOld);
	pen.DeleteObject();
}

void CContext::SelectPen(int n)
{
	pDC->SelectObject((CPen*) listPens.GetAt(n));
}

void CContext::AddPen(int penStyle, int width, CColor color)
{
	CPen *tmpPen = new CPen(penStyle, width, color);
	listPens.Add(tmpPen);
}

void CContext::MoveTo(int x, int y)
{
	pDC->MoveTo(x, y);
}

void CContext::LineTo(int x, int y)
{
	pDC->LineTo(x, y);
}

void CContext::Rectangle(int x, int y, int w, int h)
{
	pDC->MoveTo(x, y);
	pDC->LineTo(x + w, y);
	pDC->LineTo(x + w, y + h);
	pDC->LineTo(x, y + h);
	pDC->LineTo(x, y);
}

void CContext::Frame(int lb, int rb, int tb, int bb)
{
	Rectangle(lb, tb, clientRect.Width() - (lb + rb), clientRect.Height() - (tb + bb));
}

void CContext::HorCenterText(CRect rectLabel, CString aText)
{
	pDC->SetTextColor(RGB(128, 128, 128));	// the same color as axis' color
	pDC->SetBkMode(TRANSPARENT);
	int nTextHei;
	nTextHei = pDC->GetTextExtent(aText).cy; // length of horizontal label
	rectLabel.bottom = rectLabel.bottom + nTextHei;
	rectLabel.top = rectLabel.top + nTextHei;
	pDC->DrawText(aText, &rectLabel, DT_CENTER | DT_SINGLELINE);
}

void CContext::VertCenterText(int x, int y, CString aText)
{
	// draw label in vertical 
	CFont	*curFont = pDC->GetCurrentFont();
	LOGFONT	curLogFont;
	curFont->GetLogFont(&curLogFont);

	curLogFont.lfEscapement = 900; // rotate 90 degree
	CFont	newFont;
	newFont.CreateFontIndirect(&curLogFont);
	CFont* pOldFont = pDC->SelectObject(&newFont);

	CSize szLabel = pDC->GetTextExtent(aText);
	pDC->TextOut(x, y, aText);
	pDC->SelectObject(pOldFont);
}
