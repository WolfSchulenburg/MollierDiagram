#pragma once
#include <afx.h>

#include "Color.h"

class CContext : public CObject
{
private:
	CDC* pDC;
	CRect clientRect;
	CBrush brush, *pBrushOld;
	CPen pen, *pPenOld;
	CArray<CPen*, CPen*> listPens;

public:
	CContext(CView* pView, CDC* pDC);
	~CContext();

	void SelectPen(int n);
	void AddPen(int penStyle, int width, CColor color);

	int Width() { return clientRect.Width(); }
	int Height() { return clientRect.Height(); }

	void MoveTo(int x, int y);
	void LineTo(int x, int y);

	void Rectangle(int x, int y, int w, int h);		// draw rectangle
	void Frame(int lb, int rb, int tb, int bb);	// draw a frame with borders around

	void HorCenterText(CRect rectLabel, CString aText);
	void VertCenterText(int x, int y, CString aText);
};

