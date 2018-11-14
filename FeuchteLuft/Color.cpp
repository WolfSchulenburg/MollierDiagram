#include "stdafx.h"
#include "Color.h"

CColor::CColor() : m_crRedGreenBlue(0)
{
  // Empty.
}

CColor::CColor(const CColor& color) : m_crRedGreenBlue(color.m_crRedGreenBlue)
{
  // Empty.
}

CColor::CColor(const COLORREF crRedGreenBlue) : m_crRedGreenBlue(crRedGreenBlue)
{
	// Empty.
}

CColor::operator COLORREF () const
{
	return m_crRedGreenBlue;
}

CColor& CColor::operator=(const CColor& color)
{
	if (this != &color)
	{
		m_crRedGreenBlue = color.m_crRedGreenBlue;
	}

	return *this;
}

void CColor::Serialize(CArchive& archive)
{
	if (archive.IsStoring())
	{
		archive << m_crRedGreenBlue;
	}

	if (archive.IsLoading())
	{
		archive >> m_crRedGreenBlue;
	}
}

CColor CColor::Inverse() const
{
	int iRed = GetRValue(m_crRedGreenBlue);
	int iGreen = GetGValue(m_crRedGreenBlue);
	int iBlue = GetBValue(m_crRedGreenBlue);

	return CColor(RGB(255 - iRed, 255 - iGreen, 255 - iBlue));
}
