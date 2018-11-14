#pragma once
#include "stdafx.h"

class CColor
{
public:
	CColor(void);
	CColor(const COLORREF crRedGreenBlue);
	CColor(const CColor& color);
	
	operator COLORREF() const;
	
	CColor& operator=(const CColor& color);

	void Serialize(CArchive& archive);

	CColor Inverse() const;

private:
	COLORREF m_crRedGreenBlue;
};

static CColor LIGHT_GRAY = RGB(80, 80, 80);
static CColor GRAY = RGB(128, 128, 128);
static CColor WHITE = RGB(255, 255, 255);
static CColor BLACK = RGB(0, 0, 0);
