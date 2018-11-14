#include "stdafx.h"
#include "VectorChar.h"


CVectorChar::CVectorChar(int iASCII, float fWidth, int iOverShot, int iUnderShot) 
	: m_iASCII(iASCII), m_fWidth(fWidth), m_iOverShot(iOverShot), m_iUnderShot(iUnderShot)
{
}


CVectorChar::~CVectorChar()
{
	m_arrStrokes.RemoveAll();
}


void CVectorChar::AddStroke(float x1, float y1, float x2, float y2)
{
	Stroke tmpStroke;
	tmpStroke.x1 = x1;
	tmpStroke.y1 = y1;
	tmpStroke.x2 = x2;
	tmpStroke.y2 = y2;
	m_arrStrokes.Add(tmpStroke);
}


int CVectorChar::StrokeCount()
{
	return 0;
}


